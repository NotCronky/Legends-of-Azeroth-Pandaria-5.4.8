/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericDruidNonCombatStrategy.h"

#include "Playerbots.h"

void GenericDruidNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("party member dead", NextAction::array(0, new NextAction("revive", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("mark of the wild", NextAction::array(0, new NextAction("mark of the wild", 21.0f), nullptr)));
}
