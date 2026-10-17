/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "UpkeepStrategies.h"

#include "Playerbots.h"

void UseFoodStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Free food: top up early. Real food: only when low
    if (sPlayerbotAIConfig->HasCheat(BotCheatMask::food))
    {
        triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("food", 3.0f), nullptr)));
        triggers.push_back(new TriggerNode("high mana", NextAction::array(0, new NextAction("drink", 3.0f), nullptr)));
    }
    else
    {
        triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("food", 3.0f), nullptr)));
        triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("drink", 3.0f), nullptr)));
    }
}

void UsePotionsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("healthstone", ACTION_MEDIUM_HEAL + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium mana", NextAction::array(0, new NextAction("mana potion", ACTION_EMERGENCY), nullptr)));
}

void VendorStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("can repair", NextAction::array(0, new NextAction("repair", 2.5f), nullptr)));
    triggers.push_back(new TriggerNode("can sell", NextAction::array(0, new NextAction("sell junk", 2.5f), nullptr)));
}

void MountStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("timer", NextAction::array(0, new NextAction("check mount state", 4.0f), nullptr)));
}
