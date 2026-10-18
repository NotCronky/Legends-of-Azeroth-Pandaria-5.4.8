/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericDKNonCombatStrategy.h"

#include "Playerbots.h"

void GenericDKNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("dk no ghoul", NextAction::array(0, new NextAction("raise dead", 22.0f), nullptr)));
    triggers.push_back(new TriggerNode("horn of winter", NextAction::array(0, new NextAction("horn of winter", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("bone shield", NextAction::array(0, new NextAction("bone shield", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("has pet", NextAction::array(0, new NextAction("toggle pet spell", 11.0f), nullptr)));
}
