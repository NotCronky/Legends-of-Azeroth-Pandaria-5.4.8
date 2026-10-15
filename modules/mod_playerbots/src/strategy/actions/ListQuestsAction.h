/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_LISTQUESTSACTION_H
#define _PLAYERBOT_LISTQUESTSACTION_H

#include "Action.h"

class PlayerbotAI;

// "quests [co|in|all]": the bot's quest log. No filter gives the counts only; co lists completed quests,
// in incomplete ones
class ListQuestsAction : public Action
{
public:
    ListQuestsAction(PlayerbotAI* botAI) : Action(botAI, "quests") {}

    bool Execute(Event event) override;

private:
    uint32 ListQuests(bool completed, bool silent);
};

#endif
