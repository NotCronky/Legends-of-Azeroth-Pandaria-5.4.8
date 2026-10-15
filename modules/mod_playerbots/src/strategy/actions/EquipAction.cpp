/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "EquipAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Helper.h"
#include "Playerbots.h"

bool EquipAction::Execute(Event event)
{
    if (!CheckManager(event))
        return false;

    std::string const text = event.getParam();
    if (text.empty())
    {
        botAI->TellMaster("Usage: e <item link or name>");
        return false;
    }

    std::vector<Item*> const items = parseItems(text);
    if (items.empty())
    {
        botAI->TellMaster("I don't have " + text + " in my bags");
        return false;
    }

    // One of each item: several copies of a ring would otherwise replace each other
    std::set<uint32> tried;
    bool equipped = false;
    for (Item* item : items)
        if (!item->IsEquipped() && item->GetTemplate()->InventoryType != INVTYPE_NON_EQUIP &&
            tried.insert(item->GetEntry()).second)
            equipped |= EquipItem(item);

    return equipped;
}

bool EquipAction::EquipItem(Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();

    uint16 dest;
    InventoryResult const result = bot->CanEquipItem(NULL_SLOT, dest, item, !item->IsBag());
    if (result != EQUIP_ERR_OK)
    {
        std::ostringstream out;
        out << "I can't equip " << ChatHelper::FormatItem(proto) << " (error " << uint32(result) << ")";
        botAI->TellMaster(out);
        return false;
    }

    // The client's auto-equip: swaps out what's in the slot, and handles bags and two-handers
    WorldPacket packet(CMSG_AUTOEQUIP_ITEM, 2);
    packet << uint8(item->GetSlot()) << uint8(item->GetBagSlot());
    bot->GetSession()->HandleAutoEquipItemOpcode(packet);

    if (!item->IsEquipped() && !bot->IsBagPos(item->GetPos()))
    {
        botAI->TellMaster("I couldn't equip " + ChatHelper::FormatItem(proto));
        return false;
    }

    botAI->TellMaster("Equipping " + ChatHelper::FormatItem(proto));
    return true;
}

bool UnequipAction::Execute(Event event)
{
    if (!CheckManager(event))
        return false;

    std::string const text = event.getParam();
    if (text.empty())
    {
        botAI->TellMaster("Usage: ue <item link, name or slot>[, <slot>...]");
        return false;
    }

    // Slots can be listed: "ue head, hands"
    std::vector<Item*> items;
    for (std::string name : split(text, ','))
    {
        TrimString(name);
        for (Item* item : parseItems(name, ITERATE_ITEMS_IN_EQUIP))
            if (std::find(items.begin(), items.end(), item) == items.end())
                items.push_back(item);
    }

    if (items.empty())
    {
        botAI->TellMaster("I'm not wearing " + text);
        return false;
    }

    bool unequipped = false;
    for (Item* item : items)
        unequipped |= UnequipItem(item);

    return unequipped;
}

bool UnequipAction::UnequipItem(Item* item)
{
    std::string const link = ChatHelper::FormatItem(item->GetTemplate());

    WorldPacket packet(CMSG_AUTOSTORE_BAG_ITEM, 3);
    packet << uint8(item->GetSlot()) << uint8(item->GetBagSlot()) << uint8(NULL_BAG);
    bot->GetSession()->HandleAutoStoreBagItemOpcode(packet);

    if (item->IsEquipped())
    {
        botAI->TellMaster("I have no room in my bags for " + link);
        return false;
    }

    botAI->TellMaster(link + " unequipped");
    return true;
}
