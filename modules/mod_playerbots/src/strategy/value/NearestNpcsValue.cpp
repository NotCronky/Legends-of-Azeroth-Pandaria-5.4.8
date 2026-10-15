/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "NearestNpcsValue.h"

#include "CellImpl.h"
#include "GridObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Playerbots.h"

void NearestNpcsValue::FindUnits(std::list<Unit*>& targets)
{
    Trinity::AnyUnitInObjectRangeCheck u_check(bot, range);
    Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(bot, targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool NearestNpcsValue::AcceptUnit(Unit* unit) { return unit->GetTypeId() != TYPEID_PLAYER; }

namespace
{
    constexpr uint32 RpgNpcFlags = UNIT_NPC_FLAG_INNKEEPER | UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER |
        UNIT_NPC_FLAG_FLIGHTMASTER | UNIT_NPC_FLAG_BANKER | UNIT_NPC_FLAG_GUILD_BANKER | UNIT_NPC_FLAG_TRAINER |
        UNIT_NPC_FLAG_TRAINER_CLASS | UNIT_NPC_FLAG_TRAINER_PROFESSION | UNIT_NPC_FLAG_VENDOR |
        UNIT_NPC_FLAG_VENDOR_AMMO | UNIT_NPC_FLAG_VENDOR_FOOD | UNIT_NPC_FLAG_VENDOR_POISON |
        UNIT_NPC_FLAG_VENDOR_REAGENT | UNIT_NPC_FLAG_REPAIR | UNIT_NPC_FLAG_AUCTIONEER | UNIT_NPC_FLAG_STABLEMASTER |
        UNIT_NPC_FLAG_PETITIONER | UNIT_NPC_FLAG_TABARDDESIGNER | UNIT_NPC_FLAG_BATTLEMASTER | UNIT_NPC_FLAG_MAILBOX |
        UNIT_NPC_FLAG_TRANSMOGRIFIER | UNIT_NPC_FLAG_REFORGER;
}

void PossibleRpgTargetsValue::FindUnits(std::list<Unit*>& targets)
{
    Trinity::AnyUnitInObjectRangeCheck u_check(bot, range);
    Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(bot, targets, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
}

bool PossibleRpgTargetsValue::AcceptUnit(Unit* unit)
{
    if (!unit || !unit->IsInWorld() || unit->IsDuringRemoveFromWorld())
        return false;

    if (unit->GetTypeId() == TYPEID_PLAYER || unit->IsHostileTo(bot))
        return false;

    if (bot->GetDistance2d(unit) <= sPlayerbotAIConfig->tooCloseDistance)
        return false;

    if (unit->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER | UNIT_NPC_FLAG_SPIRITGUIDE))
        return false;

    return unit->HasFlag(UNIT_FIELD_NPC_FLAGS, RpgNpcFlags);
}
