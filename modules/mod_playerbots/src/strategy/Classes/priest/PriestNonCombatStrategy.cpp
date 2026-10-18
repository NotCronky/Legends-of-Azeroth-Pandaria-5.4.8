/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PriestNonCombatStrategy.h"

#include "Playerbots.h"

void PriestNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("party member dead", NextAction::array(0, new NextAction("resurrection", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("power word: fortitude", NextAction::array(0, new NextAction("power word: fortitude", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("inner fire", NextAction::array(0, new NextAction("inner fire", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("shadowform", NextAction::array(0, new NextAction("shadowform", 20.0f), nullptr)));
}
