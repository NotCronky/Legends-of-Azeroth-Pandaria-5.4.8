/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_GENERICMAGENONCOMBATSTRATEGY_H
#define _PLAYERBOT_GENERICMAGENONCOMBATSTRATEGY_H

#include "NonCombatStrategy.h"

class PlayerbotAI;

// Arcane Brilliance, the specialization's armor and Frost's Water Elemental out of combat
class GenericMageNonCombatStrategy : public NonCombatStrategy
{
public:
    GenericMageNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI) {}

    std::string const getName() override { return "nc"; }
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
};

#endif
