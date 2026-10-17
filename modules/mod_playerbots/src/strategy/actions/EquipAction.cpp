/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "EquipAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Helper.h"
#include "ItemUsageValue.h"
#include "Playerbots.h"
#include "StatsWeightCalculator.h"

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

bool EquipAction::EquipItem(Item* item, bool tellErrors)
{
    ItemTemplate const* proto = item->GetTemplate();

    uint16 dest;
    InventoryResult const result = bot->CanEquipItem(NULL_SLOT, dest, item, !item->IsBag());
    if (result != EQUIP_ERR_OK)
    {
        if (!tellErrors)
            return false;

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

bool EquipUpgradesAction::Execute(Event /*event*/)
{
    if (!sPlayerbotAIConfig->autoEquipUpgradeLoot && !sRandomPlayerbotMgr->IsRandomBot(bot))
        return false;

    // Armor can't be changed in combat
    if (bot->IsInCombat())
        return false;

    class EquippableVisitor : public FindItemVisitor
    {
    protected:
        bool Accept(ItemTemplate const* proto) override { return proto->InventoryType != INVTYPE_NON_EQUIP; }
    };

    // Each upgrade changes what the next one is compared with, so look again after every swap
    bool equipped = false;
    std::set<ObjectGuid> tried;
    for (uint32 pass = 0; pass < MAX_EQUIPMENT_ITEMS; ++pass)
    {
        EquippableVisitor visitor;
        IterateItems(&visitor);

        Item* upgrade = nullptr;
        for (Item* item : visitor.GetResult())
        {
            if (tried.count(item->GetGUID()))
                continue;

            ItemTemplate const* proto = item->GetTemplate();
            ItemUsage const usage =
                AI_VALUE2(ItemUsage, "item usage", ItemUsageValue::Qualify(proto->ItemId, item->GetItemRandomPropertyId()));
            if (usage == ITEM_USAGE_EQUIP || usage == ITEM_USAGE_REPLACE || usage == ITEM_USAGE_BROKEN_EQUIP)
            {
                upgrade = item;
                break;
            }
        }

        if (!upgrade)
            break;

        tried.insert(upgrade->GetGUID());

        // Bags go in a free bag slot
        if (upgrade->GetTemplate()->Class == ITEM_CLASS_CONTAINER)
        {
            equipped |= EquipItem(upgrade, false);
            continue;
        }

        // Into the slot the upgrade was measured against: the auto-equip handler would always pick the first ring,
        // trinket or hand, and swap the replaced item back in next time
        StatsWeightCalculator calculator(bot);
        float oldScore = 0.0f;
        uint8 const slot = ItemUsageValue::FindUpgradeSlot(bot, upgrade->GetTemplate(), calculator, oldScore);
        if (slot == NULL_SLOT)
            continue;

        uint16 dest;
        if (bot->CanEquipItem(slot, dest, upgrade, true) != EQUIP_ERR_OK)
            continue;

        std::string const link = ChatHelper::FormatItem(upgrade->GetTemplate());
        bot->SwapItem(upgrade->GetPos(), dest);
        if (bot->GetItemByPos(dest) == upgrade)
        {
            botAI->TellMaster("Equipping " + link);
            equipped = true;
        }
    }

    return equipped;
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
