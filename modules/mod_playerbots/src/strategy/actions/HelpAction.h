/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_HELPACTION_H
#define _PLAYERBOT_HELPACTION_H

#include <set>

#include "Action.h"

class PlayerbotAI;

// "help": lists the chat commands and the strategies the co/nc/de commands accept
class HelpAction : public Action
{
public:
    HelpAction(PlayerbotAI* botAI) : Action(botAI, "help") {}

    bool Execute(Event event) override;

private:
    static std::string const CombineSupported(std::set<std::string> const& names);
};

#endif
