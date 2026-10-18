/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MONKNONCOMBATSTRATEGY_H
#define _PLAYERBOT_MONKNONCOMBATSTRATEGY_H

#include "NonCombatStrategy.h"

class PlayerbotAI;

// Legacy of the Emperor and resurrecting the group out of combat
class MonkNonCombatStrategy : public NonCombatStrategy
{
public:
    MonkNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI) {}

    std::string const getName() override { return "nc"; }
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
};

#endif
