/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ChatCommandHandlerStrategy.h"

#include "Playerbots.h"

// Commands whose action has a different name
void ChatCommandHandlerStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    PassTroughStrategy::InitTriggers(triggers);

    auto add = [&](char const* command, char const* action)
    {
        triggers.push_back(new TriggerNode(command, NextAction::array(0, new NextAction(action, relevance), nullptr)));
    };

    add("follow", "follow chat shortcut");
    add("stay", "stay chat shortcut");
    add("flee", "flee chat shortcut");
    add("runaway", "runaway chat shortcut");
    add("grind", "grind chat shortcut");
    add("move from group", "move from group chat shortcut");
    add("attack", "attack my target");
    add("tank attack", "tank attack chat shortcut");
    add("max dps", "max dps chat shortcut");
    add("e", "equip");
    add("ue", "unequip");
    add("s", "sell");
    add("u", "use");
    add("t", "trade");
}

// Commands whose action has the same name
ChatCommandHandlerStrategy::ChatCommandHandlerStrategy(PlayerbotAI* botAI) : PassTroughStrategy(botAI)
{
    supported.push_back("co");
    supported.push_back("nc");
    supported.push_back("de");
    supported.push_back("summon");
    supported.push_back("leave");
    supported.push_back("rti");
    supported.push_back("stats");
    supported.push_back("rpg status");
    supported.push_back("help");
    supported.push_back("c");
    supported.push_back("equip");
    supported.push_back("unequip");
    supported.push_back("sell");
    supported.push_back("use");
    supported.push_back("trade");
    supported.push_back("quests");
    supported.push_back("spells");
    supported.push_back("talents");
    supported.push_back("ll");
}
