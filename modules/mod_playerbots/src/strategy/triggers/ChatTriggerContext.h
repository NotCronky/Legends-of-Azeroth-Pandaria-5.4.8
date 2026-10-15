/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CHATTRIGGERCONTEXT_H
#define _PLAYERBOT_CHATTRIGGERCONTEXT_H

#include <algorithm>
#include <cstddef>

#include "ChatCommandTrigger.h"
#include "NamedObjectContext.h"

namespace ChatTriggers
{
    // A string literal usable as a template argument, so each command gets its own creator function
    template <size_t N>
    struct CommandName
    {
        constexpr CommandName(char const (&text)[N]) { std::copy_n(text, N, value); }
        char value[N];
    };

    template <CommandName Name>
    Trigger* command(PlayerbotAI* botAI) { return new ChatCommandTrigger(botAI, Name.value); }
}

// One trigger per chat command a player can send; ChatCommandHandlerStrategy maps them to actions
class ChatTriggerContext : public NamedObjectContext<Trigger>
{
public:
    ChatTriggerContext()
    {
        creators["co"] = &ChatTriggers::command<"co">;
        creators["nc"] = &ChatTriggers::command<"nc">;
        creators["de"] = &ChatTriggers::command<"de">;
        creators["follow"] = &ChatTriggers::command<"follow">;
        creators["stay"] = &ChatTriggers::command<"stay">;
        creators["flee"] = &ChatTriggers::command<"flee">;
        creators["runaway"] = &ChatTriggers::command<"runaway">;
        creators["grind"] = &ChatTriggers::command<"grind">;
        creators["move from group"] = &ChatTriggers::command<"move from group">;
        creators["attack"] = &ChatTriggers::command<"attack">;
        creators["tank attack"] = &ChatTriggers::command<"tank attack">;
        creators["max dps"] = &ChatTriggers::command<"max dps">;
        creators["summon"] = &ChatTriggers::command<"summon">;
        creators["leave"] = &ChatTriggers::command<"leave">;
        creators["rti"] = &ChatTriggers::command<"rti">;
        creators["stats"] = &ChatTriggers::command<"stats">;
        creators["rpg status"] = &ChatTriggers::command<"rpg status">;
        creators["help"] = &ChatTriggers::command<"help">;
        creators["c"] = &ChatTriggers::command<"c">;
        creators["e"] = &ChatTriggers::command<"e">;
        creators["equip"] = &ChatTriggers::command<"equip">;
        creators["ue"] = &ChatTriggers::command<"ue">;
        creators["unequip"] = &ChatTriggers::command<"unequip">;
        creators["s"] = &ChatTriggers::command<"s">;
        creators["sell"] = &ChatTriggers::command<"sell">;
        creators["u"] = &ChatTriggers::command<"u">;
        creators["use"] = &ChatTriggers::command<"use">;
        creators["t"] = &ChatTriggers::command<"t">;
        creators["trade"] = &ChatTriggers::command<"trade">;
        creators["quests"] = &ChatTriggers::command<"quests">;
        creators["spells"] = &ChatTriggers::command<"spells">;
        creators["talents"] = &ChatTriggers::command<"talents">;
    }
};

#endif
