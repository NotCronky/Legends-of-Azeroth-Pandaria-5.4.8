/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ExternalEventHelper.h"

//#include "ChatHelper.h"
#include "Playerbots.h"
#include "Trigger.h"

bool ExternalEventHelper::ParseChatCommand(std::string const command, Player* owner)
{
    // Chat text is user input: only look up names that are registered so ordinary chat isn't reported as missing
    std::set<std::string> const supported = aiObjectContext->GetSupportedTriggers();
    auto handle = [&](std::string const& name, std::string const& param)
    {
        return supported.count(name) && HandleCommand(name, param, owner);
    };

    if (handle(command, ""))
        return true;

    size_t i = std::string::npos;
    while (true)
    {
        size_t found = command.rfind(" ", i);
        if (found == std::string::npos || !found)
            break;

        std::string const name = command.substr(0, found);
        std::string const param = command.substr(found + 1);

        i = found - 1;

        if (handle(name, param))
            return true;
    }

    //if (!ChatHelper::parseable(command))
        //return false;

    // Item links: item count and trade, once those commands are ported
    bool const counted = handle("c", command);
    bool const traded = handle("t", command);
    return counted || traded;
}

void ExternalEventHelper::HandlePacket(std::map<uint16, std::string>& handlers, WorldPacket const& packet,
                                       Player* owner)
{
    uint16 opcode = packet.GetOpcode();
    std::string const name = handlers[opcode];
    if (name.empty())
        return;

    // AiObjectContext::GetTrigger reports unregistered packet triggers once
    Trigger* trigger = aiObjectContext->GetTrigger(name);
    if (!trigger)
        return;

    WorldPacket p(packet);
    trigger->ExternalEvent(p, owner);
}

bool ExternalEventHelper::HandleCommand(std::string const name, std::string const param, Player* owner)
{
    Trigger* trigger = aiObjectContext->GetTrigger(name);
    if (!trigger)
        return false;

    trigger->ExternalEvent(param, owner);

    return true;
}
