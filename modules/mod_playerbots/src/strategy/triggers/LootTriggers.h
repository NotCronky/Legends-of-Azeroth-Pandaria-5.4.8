/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_LOOTTRIGGERS_H
#define _PLAYERBOT_LOOTTRIGGERS_H

#include "Trigger.h"

class PlayerbotAI;

// Something to loot, and either it is in reach or nothing hostile is around
class LootAvailableTrigger : public Trigger
{
public:
    LootAvailableTrigger(PlayerbotAI* botAI) : Trigger(botAI, "loot available") {}

    bool IsActive() override;
};

class FarFromCurrentLootTrigger : public Trigger
{
public:
    FarFromCurrentLootTrigger(PlayerbotAI* botAI) : Trigger(botAI, "far from loot target") {}

    bool IsActive() override;
};

class CanLootTrigger : public Trigger
{
public:
    CanLootTrigger(PlayerbotAI* botAI) : Trigger(botAI, "can loot") {}

    bool IsActive() override;
};

// Group rolls the bot hasn't voted on
class LootRollTrigger : public Trigger
{
public:
    LootRollTrigger(PlayerbotAI* botAI) : Trigger(botAI, "loot roll") {}

    bool IsActive() override;
};

#endif
