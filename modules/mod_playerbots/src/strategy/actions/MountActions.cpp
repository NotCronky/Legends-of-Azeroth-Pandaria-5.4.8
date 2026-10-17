/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MountActions.h"

#include "Event.h"
#include "Playerbots.h"
#include "SpellAuraDefines.h"
#include "SpellMgr.h"

namespace
{
    // Travel further than this and random bots ride
    constexpr float MOUNT_DISTANCE = 50.0f;

    bool IsFlyingMount(SpellInfo const* spellInfo) { return spellInfo->HasAura(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED); }
}

bool DismountAction::isUseful() { return bot->IsMounted() && !bot->IsInFlight(); }

bool DismountAction::Execute(Event /*event*/)
{
    bot->RemoveAurasByType(SPELL_AURA_MOUNTED);
    bot->Dismount();
    return true;
}

bool CheckMountStateAction::isPossible()
{
    return !bot->IsInCombat() && !bot->IsInWater() && !bot->GetVehicle() && !bot->HasUnitState(UNIT_STATE_CASTING);
}

bool CheckMountStateAction::WantsMount(bool& flying)
{
    flying = false;

    // Bots following a player ride when the player does
    Player* master = botAI->GetMaster();
    if (master && master != bot && master->IsInWorld() && master->GetMapId() == bot->GetMapId() &&
        botAI->HasStrategy("follow", BOT_STATE_NON_COMBAT))
    {
        flying = master->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
        return master->IsMounted();
    }

    if (botAI->HasStrategy("stay", BOT_STATE_NON_COMBAT))
        return false;

    // Not while about to fight: grinding picks mobs on the way and the attack would dismount again
    if (AI_VALUE(Unit*, "current target") || !bot->GetTarget().IsEmpty())
        return false;

    // Random bots ride to far destinations
    NewRpgInfo& info = botAI->GetRpgInfo();
    switch (info.status)
    {
        case NewRpgStatus::GO_GRIND:
            return bot->GetExactDist(info.grindPos) > MOUNT_DISTANCE;
        case NewRpgStatus::GO_INNKEEPER:
            return bot->GetExactDist(info.innKeeperPos) > MOUNT_DISTANCE;
        default:
            return false;
    }
}

bool CheckMountStateAction::Execute(Event /*event*/)
{
    bool flying = false;
    bool const wantsMount = WantsMount(flying);
    if (wantsMount == bot->IsMounted())
        return false;

    if (!wantsMount)
    {
        bot->RemoveAurasByType(SPELL_AURA_MOUNTED);
        bot->Dismount();
        return true;
    }

    // Where flying isn't allowed the cast fails and a ground mount is used instead
    return (flying && Mount(true)) || Mount(false);
}

uint32 CheckMountStateAction::FindMountSpell(bool flying)
{
    uint32 ground = 0;
    for (auto const& [spellId, playerSpell] : bot->GetSpellMap())
    {
        if (playerSpell->state == PLAYERSPELL_REMOVED || playerSpell->disabled)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo || !spellInfo->HasAura(SPELL_AURA_MOUNTED))
            continue;

        if (IsFlyingMount(spellInfo) == flying)
            return spellId;

        // Flying mounts also run on the ground
        if (!flying && !ground)
            ground = spellId;
    }

    return ground;
}

bool CheckMountStateAction::Mount(bool flying)
{
    uint32 const spellId = FindMountSpell(flying);
    if (!spellId)
        return false;

    // Mounting has a cast time
    if (bot->isMoving())
    {
        bot->StopMoving();
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->reactDelay);
    }

    if (!botAI->CastSpell(spellId, bot))
        return false;

    // Don't walk off and interrupt it
    if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
        botAI->SetNextCheckDelay(spellInfo->CalcCastTime() + sPlayerbotAIConfig->reactDelay);

    return true;
}
