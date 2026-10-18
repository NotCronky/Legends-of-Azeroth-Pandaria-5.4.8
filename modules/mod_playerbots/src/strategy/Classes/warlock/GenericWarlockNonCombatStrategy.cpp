/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericWarlockNonCombatStrategy.h"

#include "Playerbots.h"

void GenericWarlockNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("no pet", NextAction::array(0, new NextAction("summon demon", 22.0f), nullptr)));
    triggers.push_back(new TriggerNode("dark intent", NextAction::array(0, new NextAction("dark intent", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("no healthstone", NextAction::array(0, new NextAction("create healthstone", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("has pet", NextAction::array(0, new NextAction("toggle pet spell", 11.0f), nullptr)));
}
