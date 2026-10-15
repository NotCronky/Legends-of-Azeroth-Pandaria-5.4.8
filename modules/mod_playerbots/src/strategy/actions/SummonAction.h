/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SUMMONACTION_H
#define _PLAYERBOT_SUMMONACTION_H

#include "Action.h"

class PlayerbotAI;

// "summon": teleport next to the master, reviving first if dead
class SummonAction : public Action
{
public:
    SummonAction(PlayerbotAI* botAI) : Action(botAI, "summon") {}

    bool Execute(Event event) override;
};

#endif
