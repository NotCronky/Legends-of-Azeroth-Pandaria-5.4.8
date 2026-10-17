/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MOUNTACTIONS_H
#define _PLAYERBOT_MOUNTACTIONS_H

#include "Action.h"

class PlayerbotAI;

// Mount and dismount with the master; random bots mount to travel to a far destination
class CheckMountStateAction : public Action
{
public:
    CheckMountStateAction(PlayerbotAI* botAI) : Action(botAI, "check mount state") {}

    bool Execute(Event event) override;
    bool isPossible() override;

private:
    bool WantsMount(bool& flying);
    bool Mount(bool flying);
    uint32 FindMountSpell(bool flying);
};

#endif
