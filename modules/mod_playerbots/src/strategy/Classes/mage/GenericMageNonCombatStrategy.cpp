/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericMageNonCombatStrategy.h"

#include "Playerbots.h"

void GenericMageNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("arcane brilliance", NextAction::array(0, new NextAction("arcane brilliance", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("mage armor", NextAction::array(0, new NextAction("mage armor", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("molten armor", NextAction::array(0, new NextAction("molten armor", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("frost armor", NextAction::array(0, new NextAction("frost armor", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("water elemental", NextAction::array(0, new NextAction("summon water elemental", 22.0f), nullptr)));
}
