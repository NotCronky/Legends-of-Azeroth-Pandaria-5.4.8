/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_UPKEEPSTRATEGIES_H
#define _PLAYERBOT_UPKEEPSTRATEGIES_H

#include "Strategy.h"

class PlayerbotAI;

// Eat and drink out of combat
class UseFoodStrategy : public Strategy
{
public:
    UseFoodStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "food"; }
};

// Healthstones and potions in combat
class UsePotionsStrategy : public Strategy
{
public:
    UsePotionsStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "potions"; }
};

// Repair and sell junk when next to a repair NPC or vendor
class VendorStrategy : public Strategy
{
public:
    VendorStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "vendor"; }
};

// Ride with the master, or to far destinations
class MountStrategy : public Strategy
{
public:
    MountStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "mount"; }
};

#endif
