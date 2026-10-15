/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_INVENTORYACTION_H
#define _PLAYERBOT_INVENTORYACTION_H

#include "Action.h"
#include "ItemVisitors.h"

class PlayerbotAI;

struct ItemTemplate;

// Base for chat commands that act on the bot's items
class InventoryAction : public Action
{
public:
    InventoryAction(PlayerbotAI* botAI, std::string const name) : Action(botAI, name) {}

protected:
    void IterateItems(IterateItemsVisitor* visitor, IterateItemsMask mask = ITERATE_ITEMS_IN_BAGS);
    void TellItems(std::map<uint32, uint32> const& items, std::map<uint32, bool> const& soulbound);
    void TellItem(ItemTemplate const* proto, uint32 count, bool soulbound);
    // Items described by chat text: links, a name, a quality, a class, a slot or a keyword such as "food".
    // A trailing number limits how many items a quality or class matches (default: a trade window's worth).
    std::vector<Item*> parseItems(std::string const& text, IterateItemsMask mask = ITERATE_ITEMS_IN_BAGS);
    // Tells the commander off and returns false if they may not manage this bot's items
    bool CheckManager(Event& event);

private:
    void IterateItemsInBags(IterateItemsVisitor* visitor);
    void IterateItemsInEquip(IterateItemsVisitor* visitor);
};

#endif
