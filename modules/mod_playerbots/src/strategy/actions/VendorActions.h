/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_VENDORACTIONS_H
#define _PLAYERBOT_VENDORACTIONS_H

#include "SellAction.h"

class Creature;
class PlayerbotAI;

// "repair": repair everything at a repair NPC in reach
class RepairAllAction : public Action
{
public:
    RepairAllAction(PlayerbotAI* botAI) : Action(botAI, "repair") {}

    bool Execute(Event event) override;

    static Creature* FindRepairNpc(PlayerbotAI* botAI);
};

// Sell what the bot only has a use for selling ("s vendor") at a vendor in reach
class SellJunkAction : public SellAction
{
public:
    SellJunkAction(PlayerbotAI* botAI) : SellAction(botAI, "sell junk") {}

    bool Execute(Event event) override;
};

#endif
