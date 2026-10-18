/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PRIESTNONCOMBATSTRATEGY_H
#define _PLAYERBOT_PRIESTNONCOMBATSTRATEGY_H

#include "NonCombatStrategy.h"

class PlayerbotAI;

// Power Word: Fortitude, Inner Fire, Shadowform and resurrecting the group out of combat
class PriestNonCombatStrategy : public NonCombatStrategy
{
public:
    PriestNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI) {}

    std::string const getName() override { return "nc"; }
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
};

#endif
