/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericPaladinNonCombatStrategy.h"

#include "Playerbots.h"

void GenericPaladinNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("party member dead", NextAction::array(0, new NextAction("redemption", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("blessing of kings", NextAction::array(0, new NextAction("blessing of kings", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("blessing of might", NextAction::array(0, new NextAction("blessing of might", 21.0f), nullptr)));
    triggers.push_back(new TriggerNode("righteous fury", NextAction::array(0, new NextAction("righteous fury", 20.0f), nullptr)));
}
