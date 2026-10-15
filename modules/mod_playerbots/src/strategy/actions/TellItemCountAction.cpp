/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "TellItemCountAction.h"

#include "Event.h"
#include "Playerbots.h"

bool TellItemCountAction::Execute(Event event)
{
    std::string const text = event.getParam();

    std::map<uint32, uint32> items;
    std::map<uint32, bool> soulbound;
    if (text.empty())
    {
        ListItemsVisitor visitor;
        IterateItems(&visitor);
        items = visitor.items;
        soulbound = visitor.soulbound;
    }
    else
    {
        for (Item* item : parseItems(text, IterateItemsMask(ITERATE_ITEMS_IN_BAGS | ITERATE_ITEMS_IN_EQUIP)))
        {
            items[item->GetEntry()] += item->GetCount();
            soulbound[item->GetEntry()] = item->IsSoulBound();
        }
    }

    if (items.empty())
    {
        // Shift-clicked items reach every bot in the group: only those that have it answer
        if (text.find("|Hitem:") == std::string::npos)
            botAI->TellMaster(text.empty() ? "My bags are empty" : "I don't have " + text);

        return false;
    }

    botAI->TellMaster("=== Inventory ===");
    TellItems(items, soulbound);
    return true;
}
