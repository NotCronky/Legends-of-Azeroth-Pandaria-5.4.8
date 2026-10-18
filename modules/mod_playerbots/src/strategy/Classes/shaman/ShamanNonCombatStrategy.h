/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SHAMANNONCOMBATSTRATEGY_H
#define _PLAYERBOT_SHAMANNONCOMBATSTRATEGY_H

#include "NonCombatStrategy.h"

class PlayerbotAI;

// A shield, the specialization's weapon imbue and resurrecting the group out of combat
class ShamanNonCombatStrategy : public NonCombatStrategy
{
public:
    ShamanNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "nc"; }
};

#endif
