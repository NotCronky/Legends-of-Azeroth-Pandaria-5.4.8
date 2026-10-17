/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SELLACTION_H
#define _PLAYERBOT_SELLACTION_H

#include "InventoryAction.h"

class Creature;
class PlayerbotAI;

// "s <items>": sell to a nearby vendor. "s gray" or "s *" sells grey items; "s vendor" what the bot has no
// other use for; "s <quality>" sells armor and weapons of that quality or lower, "s <quality> all" every kind of item
class SellAction : public InventoryAction
{
public:
    SellAction(PlayerbotAI* botAI, std::string const name = "sell") : InventoryAction(botAI, name) {}

    bool Execute(Event event) override;

protected:
    Creature* FindVendor();

private:
    bool Sell(Creature* vendor, Item* item);
};

#endif
