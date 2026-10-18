/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ShamanNonCombatStrategy.h"

#include "Playerbots.h"

void ShamanNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("party member dead", NextAction::array(0, new NextAction("ancestral spirit", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("flametongue weapon", NextAction::array(0, new NextAction("flametongue weapon", 22.0f), nullptr)));
    triggers.push_back(new TriggerNode("windfury weapon", NextAction::array(0, new NextAction("windfury weapon", 22.0f), nullptr)));
    triggers.push_back(new TriggerNode("earthliving weapon", NextAction::array(0, new NextAction("earthliving weapon", 22.0f), nullptr)));
    triggers.push_back(new TriggerNode("water shield", NextAction::array(0, new NextAction("water shield", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("lightning shield", NextAction::array(0, new NextAction("lightning shield", 21.0f), nullptr)));
}
