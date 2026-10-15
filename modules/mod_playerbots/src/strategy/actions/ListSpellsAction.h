/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_LISTSPELLSACTION_H
#define _PLAYERBOT_LISTSPELLSACTION_H

#include "Action.h"

class PlayerbotAI;

// "spells [profession] [name]": the bot's spells, filtered by profession and part of the name. Profession
// recipes show what they make, the reagents and how many the bot has
class ListSpellsAction : public Action
{
public:
    ListSpellsAction(PlayerbotAI* botAI) : Action(botAI, "spells") {}

    bool Execute(Event event) override;
};

#endif
