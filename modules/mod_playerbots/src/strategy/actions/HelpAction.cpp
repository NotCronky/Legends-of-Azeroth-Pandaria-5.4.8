/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "HelpAction.h"

#include "ChatTriggerContext.h"
#include "Event.h"
#include "Playerbots.h"

bool HelpAction::Execute(Event /*event*/)
{
    ChatTriggerContext commands;
    botAI->TellMaster("Commands: " + CombineSupported(commands.supports()));
    botAI->TellMaster("Strategies (co/nc/de +name, -name, ?): " +
        CombineSupported(botAI->GetAiObjectContext()->GetSupportedStrategies()));
    return true;
}

std::string const HelpAction::CombineSupported(std::set<std::string> const& names)
{
    std::ostringstream out;
    for (auto i = names.begin(); i != names.end();)
    {
        out << *i;
        if (++i != names.end())
            out << ", ";
    }

    return out.str();
}
