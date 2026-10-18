/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "WarlockTriggers.h"

#include "Playerbots.h"

bool NoHealthstoneTrigger::IsActive()
{
    if (!bot->HasSpell(6201) /*Create Healthstone*/ || bot->HasItemCount(5512 /*Healthstone*/, 1))
        return false;

    // With full bags the spell makes nothing and the trigger would fire again at once
    ItemPosCountVec dest;
    return bot->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 5512, 1) == EQUIP_ERR_OK;
}

bool DrainSoulTrigger::IsActive()
{
    Unit* target = AI_VALUE(Unit*, "current target");
    return target && target->GetHealthPct() < 20.0f;
}

bool BurningEmbersTrigger::IsActive()
{
    // Embers are kept in tenths
    return bot->GetPower(POWER_BURNING_EMBERS) >= 30;
}
