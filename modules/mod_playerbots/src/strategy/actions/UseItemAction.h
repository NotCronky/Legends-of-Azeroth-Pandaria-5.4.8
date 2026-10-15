/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_USEITEMACTION_H
#define _PLAYERBOT_USEITEMACTION_H

#include "InventoryAction.h"

class PlayerbotAI;
class SpellInfo;
class Unit;

// "u <item>": use an item. A second item link is the target (an enchant on a weapon, for example); harmful
// items are used on the bot's or the master's target, others on the bot itself
class UseItemAction : public InventoryAction
{
public:
    UseItemAction(PlayerbotAI* botAI) : InventoryAction(botAI, "use") {}

    bool Execute(Event event) override;

private:
    bool UseItem(Item* item, Item* itemTarget);
    Unit* FindHarmfulTarget();
};

#endif
