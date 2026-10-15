/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "InventoryAction.h"

#include <algorithm>

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"
#include "TradeData.h"

namespace
{
    bool CompareItems(ItemTemplate const* proto1, ItemTemplate const* proto2)
    {
        if (proto1->Class != proto2->Class)
            return proto1->Class > proto2->Class;

        if (proto1->SubClass != proto2->SubClass)
            return proto1->SubClass < proto2->SubClass;

        if (proto1->Quality != proto2->Quality)
            return proto1->Quality < proto2->Quality;

        if (proto1->ItemLevel != proto2->ItemLevel)
            return proto1->ItemLevel > proto2->ItemLevel;

        return proto1->ItemId < proto2->ItemId;
    }

    char const* ItemClassHeader(uint32 itemClass)
    {
        switch (itemClass)
        {
            case ITEM_CLASS_CONSUMABLE: return "--- consumable ---";
            case ITEM_CLASS_CONTAINER: return "--- container ---";
            case ITEM_CLASS_WEAPON: return "--- weapon ---";
            case ITEM_CLASS_GEM: return "--- gem ---";
            case ITEM_CLASS_ARMOR: return "--- armor ---";
            case ITEM_CLASS_REAGENT: return "--- reagent ---";
            case ITEM_CLASS_TRADE_GOODS: return "--- trade goods ---";
            case ITEM_CLASS_RECIPE: return "--- recipe ---";
            case ITEM_CLASS_QUEST: return "--- quest items ---";
            case ITEM_CLASS_KEY: return "--- keys ---";
            case ITEM_CLASS_GLYPH: return "--- glyph ---";
            case ITEM_CLASS_BATTLE_PET: return "--- battle pet ---";
            default: return "--- other ---";
        }
    }

    // Keywords that select items by what they are rather than by name
    bool IsKeyword(std::string const& text)
    {
        static std::set<std::string> const keywords = { "food", "drink", "water", "conjured food", "conjured drink",
            "conjured water", "mana potion", "healing potion", "mount", "pet", "recipe", "quest" };
        return keywords.count(text) > 0;
    }
}

void InventoryAction::IterateItems(IterateItemsVisitor* visitor, IterateItemsMask mask)
{
    if (mask & ITERATE_ITEMS_IN_BAGS)
        IterateItemsInBags(visitor);

    if (mask & ITERATE_ITEMS_IN_EQUIP)
        IterateItemsInEquip(visitor);
}

void InventoryAction::IterateItemsInBags(IterateItemsVisitor* visitor)
{
    for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            if (!visitor->Visit(item))
                return;

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        if (Bag* bag = bot->GetBagByPos(i))
            for (uint32 j = 0; j < bag->GetBagSize(); ++j)
                if (Item* item = bag->GetItemByPos(j))
                    if (!visitor->Visit(item))
                        return;
}

void InventoryAction::IterateItemsInEquip(IterateItemsVisitor* visitor)
{
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            if (!visitor->Visit(item))
                return;
}

void InventoryAction::TellItems(std::map<uint32, uint32> const& items, std::map<uint32, bool> const& soulbound)
{
    std::vector<ItemTemplate const*> protos;
    for (auto const& [itemId, count] : items)
        if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId))
            protos.push_back(proto);

    std::sort(protos.begin(), protos.end(), CompareItems);

    uint32 oldClass = MAX_ITEM_CLASS;
    for (ItemTemplate const* proto : protos)
    {
        if (proto->Class != oldClass)
        {
            oldClass = proto->Class;
            botAI->TellMaster(ItemClassHeader(proto->Class));
        }

        auto const bound = soulbound.find(proto->ItemId);
        TellItem(proto, items.at(proto->ItemId), bound != soulbound.end() && bound->second);
    }
}

void InventoryAction::TellItem(ItemTemplate const* proto, uint32 count, bool soulbound)
{
    std::ostringstream out;
    out << ChatHelper::FormatItem(proto, count);
    if (soulbound)
        out << " (soulbound)";

    botAI->TellMaster(out.str());
}

std::vector<Item*> InventoryAction::parseItems(std::string const& param, IterateItemsMask mask)
{
    std::set<Item*> found;
    auto collect = [&](FindItemVisitor& visitor, IterateItemsMask visitMask)
    {
        IterateItems(&visitor, visitMask);
        found.insert(visitor.GetResult().begin(), visitor.GetResult().end());
    };

    // Links name exact items; everything else in the text is ignored
    ItemIds const ids = ChatHelper::parseItems(param);
    if (!ids.empty())
    {
        FindItemByIdsVisitor visitor(ids);
        collect(visitor, mask);
    }
    else
    {
        std::string text = param;
        uint32 count = TRADE_SLOT_TRADED_COUNT;
        size_t const space = text.rfind(' ');
        if (space != std::string::npos && text.find_first_not_of("0123456789", space + 1) == std::string::npos &&
            space + 1 < text.size() && text.size() - space <= 4)
        {
            count = std::max(1, std::stoi(text.substr(space + 1)));
            text = text.substr(0, space);
        }

        if (text == "food" || text == "conjured food")
        {
            FindFoodVisitor visitor(bot, SPELL_CATEGORY_FOOD, text == "conjured food");
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "drink" || text == "water" || text == "conjured drink" || text == "conjured water")
        {
            FindFoodVisitor visitor(bot, SPELL_CATEGORY_DRINK, text != "drink" && text != "water");
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "mana potion")
        {
            FindPotionVisitor visitor(bot, SPELL_EFFECT_ENERGIZE);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "healing potion")
        {
            FindPotionVisitor visitor(bot, SPELL_EFFECT_HEAL);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "mount")
        {
            FindMountVisitor visitor(bot);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "pet")
        {
            FindPetVisitor visitor(bot);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "recipe")
        {
            FindRecipeVisitor visitor(bot);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        if (text == "quest")
        {
            FindQuestItemVisitor visitor(bot);
            collect(visitor, ITERATE_ITEMS_IN_BAGS);
        }

        uint32 const quality = ChatHelper::parseItemQuality(text);
        if (quality != MAX_ITEM_QUALITY)
        {
            FindItemsToTradeByQualityVisitor visitor(quality, count);
            IterateItems(&visitor, mask);
            found.insert(visitor.GetResult().begin(), visitor.GetResult().end());
        }

        uint32 itemClass = MAX_ITEM_CLASS, itemSubClass = 0;
        if (ChatHelper::parseItemClass(text, &itemClass, &itemSubClass))
        {
            FindItemsToTradeByClassVisitor visitor(itemClass, itemSubClass, count);
            IterateItems(&visitor, mask);
            found.insert(visitor.GetResult().begin(), visitor.GetResult().end());
        }

        uint32 const slot = ChatHelper::parseSlot(text);
        if (slot != EQUIPMENT_SLOT_END)
        {
            if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                found.insert(item);
        }
        else if (!text.empty() && !IsKeyword(text) && quality == MAX_ITEM_QUALITY && itemClass == MAX_ITEM_CLASS)
        {
            FindNamedItemVisitor visitor(bot, text);
            collect(visitor, mask);
        }
    }

    std::vector<Item*> result(found.begin(), found.end());
    std::sort(result.begin(), result.end(),
        [](Item const* item1, Item const* item2) { return CompareItems(item1->GetTemplate(), item2->GetTemplate()); });
    return result;
}

bool InventoryAction::CheckManager(Event& event)
{
    Player* owner = event.getOwner();
    if (!owner || botAI->CanManageFor(owner))
        return true;

    bot->Whisper("Only my master can do that.", LANG_UNIVERSAL, owner);
    return false;
}
