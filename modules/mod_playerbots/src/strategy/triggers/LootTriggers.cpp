/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "LootTriggers.h"

#include "LootObjectStack.h"
#include "Playerbots.h"
#include "ServerFacade.h"

bool LootAvailableTrigger::IsActive()
{
    if (!AI_VALUE(bool, "has available loot"))
        return false;

    float const reach = botAI->HasStrategy("stay", BOT_STATE_NON_COMBAT) ? CONTACT_DISTANCE : INTERACTION_DISTANCE - 2.0f;
    if (sServerFacade->IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", "loot target"), reach) ||
        AI_VALUE(GuidVector, "all targets").empty())
        return true;

    // A target that can't be looted any more (despawned, taken by someone else) would stay selected forever;
    // report active so "loot" picks another
    LootObject lootTarget = AI_VALUE(LootObject, "loot target");
    return !lootTarget.IsEmpty() && !lootTarget.IsLootPossible(bot);
}

bool FarFromCurrentLootTrigger::IsActive()
{
    LootObject loot = AI_VALUE(LootObject, "loot target");
    if (!loot.IsLootPossible(bot))
        return false;

    return AI_VALUE2(float, "distance", "loot target") >= INTERACTION_DISTANCE - 2.0f;
}

bool CanLootTrigger::IsActive() { return AI_VALUE(bool, "can loot"); }

bool LootRollTrigger::IsActive() { return botAI->HasPendingLootRolls(); }
