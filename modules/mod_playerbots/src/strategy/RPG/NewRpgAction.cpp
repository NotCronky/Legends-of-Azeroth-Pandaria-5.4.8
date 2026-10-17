#include "NewRpgAction.h"

#include <cmath>
#include <cstdint>

#include "NewRpgStrategy.h"
#include "Helper.h"
#include "ObjectDefines.h"
#include "ObjectGuid.h"
#include "PathGenerator.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Random.h"
#include "RandomPlayerbotMgr.h"
#include "StatsAction.h"
#include "Timer.h"
#include "WorldPosition.h"
#include "World.h"

namespace
{
    // A repair NPC when the bot's gear is worn, a vendor when its bags are nearly full
    ObjectGuid FindNeededNpc(PlayerbotAI* botAI, GuidVector const& targets)
    {
        AiObjectContext* context = botAI->GetAiObjectContext();
        // Only what it can do there: pay for the repair, or has something to sell
        Player* bot = botAI->GetBot();
        uint32 npcFlag = 0;
        uint32 const repairCost = AI_VALUE(bool, "should repair") ? StatsAction::RepairCost(bot) : 0;
        if (repairCost && repairCost <= bot->GetMoney())
            npcFlag = UNIT_NPC_FLAG_REPAIR;
        else if (AI_VALUE(bool, "should sell") && AI_VALUE(bool, "has junk"))
            npcFlag = UNIT_NPC_FLAG_VENDOR;

        if (!npcFlag)
            return ObjectGuid::Empty;

        for (ObjectGuid const& guid : targets)
            if (Unit* unit = botAI->GetUnit(guid))
                if (unit->HasFlag(UNIT_FIELD_NPC_FLAGS, npcFlag))
                    return guid;

        return ObjectGuid::Empty;
    }
}

bool TellRpgStatusAction::Execute(Event event)
{
    Player* owner = event.getOwner();
    if (!owner)
        return false;

    std::string out = botAI->GetRpgInfo().ToString();
    bot->Whisper(out, LANG_UNIVERSAL, owner);
    return true;
}

bool NewRpgStatusUpdateAction::Execute(Event /*event*/)
{
    NewRpgInfo& info = botAI->GetRpgInfo();
    switch (info.status)
    {
        case NewRpgStatus::IDLE:
        {
            // IDLE -> NEAR_NPC: repair or sell first when there is somewhere to do it
            if (!FindNeededNpc(botAI, AI_VALUE(GuidVector, "possible rpg targets")).IsEmpty())
            {
                info.lastNearNpc = getMSTime();
                info.status = NewRpgStatus::NEAR_NPC;
                return true;
            }

            uint32 roll = urand(1, 100);
            // IDLE -> NEAR_NPC
            if (roll <= 30)
            {
                info.lastNearNpc = getMSTime();
                GuidVector possibleTargets = AI_VALUE(GuidVector, "possible rpg targets");
                if (!possibleTargets.empty())
                {
                    info.status = NewRpgStatus::NEAR_NPC;
                    return true;
                }
            }
            // IDLE -> GO_INNKEEPER
            else if (roll <= 45)
            {
                WorldPosition pos = SelectRandomInnKeeperPos();
                if (pos != WorldPosition() && bot->GetExactDist(pos) > 50.0f)
                {
                    info.lastGoInnKeeper = getMSTime();
                    info.status = NewRpgStatus::GO_INNKEEPER;
                    info.innKeeperPos = pos;
                    return true;
                }
            }
            // IDLE -> GO_GRIND
            else if (roll <= 90)
            {
                WorldPosition pos = SelectRandomGrindPos();
                if (pos != WorldPosition())
                {
                    info.lastGoGrind = getMSTime();
                    info.status = NewRpgStatus::GO_GRIND;
                    info.grindPos = pos;
                    return true;
                }

                // Most zones have no farming spot data yet: grind around the current position instead of resting
                info.status = NewRpgStatus::NEAR_RANDOM;
                info.lastNearRandom = getMSTime();
                return true;
            }
            // IDLE -> REST
            info.status = NewRpgStatus::REST;
            info.lastRest = getMSTime();
            bot->SetStandState(UNIT_STAND_STATE_SIT);
            return true;
        }
        case NewRpgStatus::GO_GRIND:
        {
            // GO_GRIND -> NEAR_RANDOM
            if (bot->GetExactDist(info.grindPos) < 10.0f)
            {
                info.status = NewRpgStatus::NEAR_RANDOM;
                info.lastNearRandom = getMSTime();
                info.grindPos = WorldPosition();
                return true;
            }
            // GO_GRIND -> IDLE when the destination can't be reached
            if (info.grindPos.GetMapId() != bot->GetMapId() || getMSTimeDiff(info.lastGoGrind, getMSTime()) > statusGoTimeout)
            {
                info.status = NewRpgStatus::IDLE;
                info.grindPos = WorldPosition();
                return true;
            }
            break;
        }
        case NewRpgStatus::GO_INNKEEPER:
        {
            // GO_INNKEEPER -> NEAR_NPC
            if (bot->GetExactDist(info.innKeeperPos) < 10.0f)
            {
                info.lastNearNpc = getMSTime();
                info.status = NewRpgStatus::NEAR_NPC;
                info.innKeeperPos = WorldPosition();
                return true;
            }
            // GO_INNKEEPER -> IDLE when the destination can't be reached
            if (info.innKeeperPos.GetMapId() != bot->GetMapId() ||
                getMSTimeDiff(info.lastGoInnKeeper, getMSTime()) > statusGoTimeout)
            {
                info.status = NewRpgStatus::IDLE;
                info.innKeeperPos = WorldPosition();
                return true;
            }
            break;
        }
        case NewRpgStatus::NEAR_RANDOM:
        {
            // NEAR_RANDOM -> IDLE
            if (getMSTimeDiff(info.lastNearRandom, getMSTime()) > statusNearRandomDuration)
            {
                info.status = NewRpgStatus::IDLE;
                return true;
            }
            break;
        }
        case NewRpgStatus::NEAR_NPC:
        {
            // NEAR_NPC -> IDLE
            if (getMSTimeDiff(info.lastNearNpc, getMSTime()) > statusNearNpcDuration)
            {
                info.status = NewRpgStatus::IDLE;
                info.npcPos = GuidPosition();
                return true;
            }
            break;
        }
        case NewRpgStatus::REST:
        {
            // REST -> IDLE
            if (getMSTimeDiff(info.lastRest, getMSTime()) > statusRestDuration)
            {
                info.status = NewRpgStatus::IDLE;
                bot->SetStandState(UNIT_STAND_STATE_STAND);
                return true;
            }
            break;
        }
        default:
            break;
    }
    return false;
}

WorldPosition NewRpgStatusUpdateAction::SelectRandomGrindPos()
{
    std::vector<WorldPosition> hi_prepared_locs, lo_prepared_locs;
    uint32 const level = bot->GetLevel();
    for (RandomPlayerbotMgr::farm_zone const& zone : sRandomPlayerbotMgr->GetFarmZones())
    {
        if (zone.map_id != bot->GetMapId() || zone.zone_id != bot->GetZoneId())
            continue;

        if (level < zone.min_level || level > zone.max_level || zone.team_disabled == bot->GetTeam())
            continue;

        for (RandomPlayerbotMgr::farm_spot const& spot : zone.spots)
        {
            if (level < spot.min_level || level > spot.max_level)
                continue;

            WorldPosition loc(spot.map_id, spot.x, spot.y, spot.z);
            float const distance = bot->GetExactDist(loc);
            if (distance < 500.0f)
                hi_prepared_locs.push_back(loc);

            if (distance < 2500.0f)
                lo_prepared_locs.push_back(loc);
        }
    }

    WorldPosition dest;
    if (urand(1, 100) <= 50 && !hi_prepared_locs.empty())
        dest = hi_prepared_locs[urand(0, hi_prepared_locs.size() - 1)];
    else if (!lo_prepared_locs.empty())
        dest = lo_prepared_locs[urand(0, lo_prepared_locs.size() - 1)];

    TC_LOG_DEBUG("playerbots", "[New Rpg] Bot {} select random grind pos Map:{} X:{} Y:{} Z:{} ({}+{} available)",
        bot->GetName(), dest.GetMapId(), dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(),
        hi_prepared_locs.size(), lo_prepared_locs.size() - hi_prepared_locs.size());
    return dest;
}

WorldPosition NewRpgStatusUpdateAction::SelectRandomInnKeeperPos()
{
    FactionTemplateEntry const* botFaction = bot->GetFactionTemplateEntry();
    if (!botFaction)
        return WorldPosition();

    float const range = bot->GetLevel() <= 5 ? 500.0f : 2500.0f;
    std::vector<WorldPosition> prepared_locs;
    for (RandomPlayerbotMgr::innkeeper const& innkeeper : sRandomPlayerbotMgr->GetInnkeepers())
    {
        if (innkeeper.map_id != bot->GetMapId() || innkeeper.zone_id != bot->GetZoneId())
            continue;

        FactionTemplateEntry const* faction = sFactionTemplateStore.LookupEntry(innkeeper.faction);
        if (!faction || faction->IsHostileTo(*botFaction))
            continue;

        WorldPosition loc(innkeeper.map_id, innkeeper.x, innkeeper.y, innkeeper.z);
        if (bot->GetExactDist(loc) < range)
            prepared_locs.push_back(loc);
    }

    WorldPosition dest;
    if (!prepared_locs.empty())
        dest = prepared_locs[urand(0, prepared_locs.size() - 1)];

    TC_LOG_DEBUG("playerbots", "[New Rpg] Bot {} select random inn keeper pos Map:{} X:{} Y:{} Z:{} ({} available)",
        bot->GetName(), dest.GetMapId(), dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(),
        prepared_locs.size());
    return dest;
}

bool NewRpgGoFarAwayPosAction::MoveFarTo(WorldPosition dest)
{
    if (dest == WorldPosition() || dest.GetMapId() != bot->GetMapId())
        return false;

    float dis = bot->GetExactDist(dest);
    if (dis < pathFinderDis)
        return MoveTo(dest.GetMapId(), dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), false, false,
            false, true);

    // performance optimization
    if (IsWaitingForLastMove(MovementPriority::MOVEMENT_NORMAL))
        return false;

    // Step towards the destination through a reachable point roughly in its direction
    float minDelta = M_PI;
    const float x = bot->GetPositionX();
    const float y = bot->GetPositionY();
    const float z = bot->GetPositionZ();
    float rx, ry, rz;
    bool found = false;
    int attempt = 3;
    while (--attempt)
    {
        float angle = bot->GetAngle(&dest);
        float delta = urand(1, 100) <= 75 ? (rand_norm() - 0.5) * M_PI * 0.5 : (rand_norm() - 0.5) * M_PI * 2;
        angle += delta;
        float stepDis = rand_norm() * pathFinderDis;
        float dx = x + cos(angle) * stepDis;
        float dy = y + sin(angle) * stepDis;
        float dz = z + 0.5f;
        bot->UpdateAllowedPositionZ(dx, dy, dz);
        PathGenerator path(bot);
        path.CalculatePath(dx, dy, dz);
        PathType type = path.GetPathType();

        bool canReach = type == PATHFIND_NORMAL;
        if (canReach && fabs(delta) <= minDelta)
        {
            found = true;
            const G3D::Vector3& endPos = path.GetActualEndPosition();
            rx = endPos.x;
            ry = endPos.y;
            rz = endPos.z;
            minDelta = fabs(delta);
        }
    }

    if (found)
        return MoveTo(bot->GetMapId(), rx, ry, rz, false, false, false, true);

    // don't fallback to direct move
    return false;
}

bool NewRpgGoGrindAction::Execute(Event /*event*/) { return MoveFarTo(botAI->GetRpgInfo().grindPos); }

bool NewRpgGoInnKeeperAction::Execute(Event /*event*/) { return MoveFarTo(botAI->GetRpgInfo().innKeeperPos); }

bool NewRpgMoveRandomAction::Execute(Event /*event*/)
{
    float distance = rand_norm() * moveStep;
    Map* map = bot->GetMap();
    const float x = bot->GetPositionX();
    const float y = bot->GetPositionY();
    const float z = bot->GetPositionZ();
    int attempts = 5;
    while (--attempts)
    {
        float angle = (float)rand_norm() * 2 * static_cast<float>(M_PI);
        float dx = x + distance * cos(angle);
        float dy = y + distance * sin(angle);
        float dz = z;
        if (!map->CheckCollisionAndGetValidCoords(bot, x, y, z, dx, dy, dz))
            continue;

        if (map->IsInWater(bot->GetPhaseMask(), dx, dy, dz))
            continue;

        if (MoveTo(bot->GetMapId(), dx, dy, dz, false, false, false, true))
            return true;
    }

    return false;
}

bool NewRpgMoveNpcAction::Execute(Event /*event*/)
{
    NewRpgInfo& info = botAI->GetRpgInfo();
    if (!info.npcPos)
    {
        GuidVector possibleTargets = AI_VALUE(GuidVector, "possible rpg targets");
        if (possibleTargets.empty())
            return false;

        ObjectGuid guid = FindNeededNpc(botAI, possibleTargets);
        if (guid.IsEmpty())
            guid = possibleTargets[urand(0, possibleTargets.size() - 1)];
        Unit* unit = botAI->GetUnit(guid);
        if (!unit)
            return false;

        info.npcPos = GuidPosition(unit);
        info.lastReachNpc = 0;
    }

    if (bot->GetDistance(info.npcPos) <= INTERACTION_DISTANCE)
    {
        if (!info.lastReachNpc)
        {
            info.lastReachNpc = getMSTime();
            return true;
        }

        if (getMSTimeDiff(info.lastReachNpc, getMSTime()) < stayTime)
            return false;

        info.npcPos = GuidPosition();
        info.lastReachNpc = 0;
        return true;
    }

    Unit* unit = botAI->GetUnit(info.npcPos);
    if (!unit)
    {
        info.npcPos = GuidPosition();
        return false;
    }

    float x = unit->GetPositionX();
    float y = unit->GetPositionY();
    float z = unit->GetPositionZ();
    uint32 mapId = unit->GetMapId();
    float angle = 0.f;
    if (bot->IsWithinLOS(x, y, z))
    {
        if (!unit->isMoving())
            angle = unit->GetAngle(bot) + (M_PI * irand(-25, 25) / 100.0);  // Closest 45 degrees towards the target
        else
            angle = unit->GetOrientation() + (M_PI * irand(-25, 25) / 100.0);  // 45 degrees in front of target
    }
    else
        angle = 2 * M_PI * rand_norm();  // A circle around the target.

    float rnd = rand_norm();
    x += cos(angle) * INTERACTION_DISTANCE * rnd;
    y += sin(angle) * INTERACTION_DISTANCE * rnd;
    if (!unit->GetMap()->CheckCollisionAndGetValidCoords(unit, unit->GetPositionX(), unit->GetPositionY(),
            unit->GetPositionZ(), x, y, z))
    {
        x = unit->GetPositionX();
        y = unit->GetPositionY();
        z = unit->GetPositionZ();
    }

    return MoveTo(mapId, x, y, z, false, false, false, true);
}
