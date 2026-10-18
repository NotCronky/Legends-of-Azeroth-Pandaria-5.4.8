/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MonkNonCombatStrategy.h"

#include "Playerbots.h"

void MonkNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("party member dead", NextAction::array(0, new NextAction("resuscitate", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("legacy of the emperor", NextAction::array(0, new NextAction("legacy of the emperor", 21.0f), nullptr)));
}
