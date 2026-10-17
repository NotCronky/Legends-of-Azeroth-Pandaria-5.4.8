/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "SellAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "ItemUsageValue.h"
#include "Playerbots.h"

namespace
{
    // Bag items up to a quality; without allClasses only grey items and armor or weapons
    class SellableByQualityVisitor : public FindItemVisitor
    {
    public:
        SellableByQualityVisitor(uint32 maxQuality, bool allClasses) : maxQuality(maxQuality), allClasses(allClasses) {}

    protected:
        bool Accept(ItemTemplate const* proto) override
        {
            if (proto->Quality > maxQuality || !proto->SellPrice)
                return false;

            // Fishing poles, mining picks and the like
            if (proto->Class == ITEM_CLASS_WEAPON &&
                (proto->SubClass == ITEM_SUBCLASS_WEAPON_MISCELLANEOUS ||
                 proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE || proto->TotemCategory))
                return false;

            if (allClasses || proto->Quality == ITEM_QUALITY_POOR)
                return true;

            return proto->Class == ITEM_CLASS_ARMOR || proto->Class == ITEM_CLASS_WEAPON;
        }

    private:
        uint32 maxQuality;
        bool allClasses;
    };

    // Bag items whose only use is selling them, usable by the bot or not
    class VendorItemVisitor : public FindItemVisitor
    {
    public:
        VendorItemVisitor(AiObjectContext* context) : context(context) {}

    protected:
        bool Accept(ItemTemplate const* proto) override
        {
            return proto->SellPrice &&
                   AI_VALUE2(ItemUsage, "item usage", ItemUsageValue::Qualify(proto->ItemId)) == ITEM_USAGE_VENDOR;
        }

    private:
        AiObjectContext* context;
    };
}

bool SellAction::Execute(Event event)
{
    if (!CheckManager(event))
        return false;

    std::string const text = event.getParam();
    if (text.empty())
    {
        botAI->TellMaster("Usage: s gray/*/vendor/<quality> [all]/<item link or name>");
        return false;
    }

    Creature* vendor = FindVendor();
    if (!vendor)
    {
        botAI->TellMaster("There is no vendor nearby");
        return false;
    }

    std::vector<Item*> items;
    std::string quality = text == "*" ? "gray" : text;
    bool allClasses = false;
    if (quality.size() > 4 && quality.substr(quality.size() - 4) == " all")
    {
        quality.erase(quality.size() - 4);
        allClasses = true;
    }

    uint32 const maxQuality = ChatHelper::parseItemQuality(quality);
    if (text == "vendor")
    {
        // What the bot has no use for other than selling it
        VendorItemVisitor visitor(context);
        IterateItems(&visitor);
        items = visitor.GetResult();
    }
    else if (maxQuality != MAX_ITEM_QUALITY)
    {
        SellableByQualityVisitor visitor(maxQuality, allClasses);
        IterateItems(&visitor);
        items = visitor.GetResult();
    }
    else
        items = parseItems(text);

    if (items.empty())
    {
        botAI->TellMaster("I have nothing to sell");
        return false;
    }

    uint64 const money = bot->GetMoney();
    uint32 sold = 0;
    for (Item* item : items)
        if (Sell(vendor, item))
            ++sold;

    if (sold)
    {
        std::ostringstream out;
        out << "Sold " << sold << (sold == 1 ? " item" : " items") << " for "
            << ChatHelper::formatMoney(uint32(bot->GetMoney() - money));
        botAI->TellMaster(out);
    }

    return sold > 0;
}

Creature* SellAction::FindVendor()
{
    for (ObjectGuid const& guid : AI_VALUE(GuidVector, "nearest npcs"))
        if (Creature* creature = bot->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_VENDOR))
            return creature;

    return nullptr;
}

bool SellAction::Sell(Creature* vendor, Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    if (!proto->SellPrice)
    {
        botAI->TellMaster("I can't sell " + ChatHelper::FormatItem(proto));
        return false;
    }

    // CMSG_SELL_ITEM in the order WorldSession::HandleSellItemOpcode reads it; the item moves to buyback
    ObjectGuid const vendorGuid = vendor->GetGUID();
    ObjectGuid const itemGuid = item->GetGUID();
    WorldPacket packet(CMSG_SELL_ITEM, 4 + 2 + 16);
    packet << uint32(item->GetCount());
    packet.WriteBit(itemGuid[4]);
    packet.WriteBit(itemGuid[3]);
    packet.WriteBit(itemGuid[7]);
    packet.WriteBit(vendorGuid[6]);
    packet.WriteBit(vendorGuid[5]);
    packet.WriteBit(vendorGuid[1]);
    packet.WriteBit(itemGuid[5]);
    packet.WriteBit(itemGuid[2]);
    packet.WriteBit(itemGuid[1]);
    packet.WriteBit(vendorGuid[2]);
    packet.WriteBit(itemGuid[6]);
    packet.WriteBit(vendorGuid[4]);
    packet.WriteBit(vendorGuid[0]);
    packet.WriteBit(vendorGuid[7]);
    packet.WriteBit(vendorGuid[3]);
    packet.WriteBit(itemGuid[0]);
    packet.FlushBits();
    packet.WriteByteSeq(vendorGuid[6]);
    packet.WriteByteSeq(vendorGuid[3]);
    packet.WriteByteSeq(vendorGuid[1]);
    packet.WriteByteSeq(itemGuid[1]);
    packet.WriteByteSeq(vendorGuid[2]);
    packet.WriteByteSeq(itemGuid[7]);
    packet.WriteByteSeq(itemGuid[5]);
    packet.WriteByteSeq(vendorGuid[7]);
    packet.WriteByteSeq(itemGuid[2]);
    packet.WriteByteSeq(vendorGuid[0]);
    packet.WriteByteSeq(vendorGuid[5]);
    packet.WriteByteSeq(itemGuid[3]);
    packet.WriteByteSeq(itemGuid[6]);
    packet.WriteByteSeq(vendorGuid[4]);
    packet.WriteByteSeq(itemGuid[4]);
    packet.WriteByteSeq(itemGuid[0]);

    uint64 const money = bot->GetMoney();
    bot->GetSession()->HandleSellItemOpcode(packet);
    if (bot->GetMoney() == money)
    {
        botAI->TellMaster("I couldn't sell " + ChatHelper::FormatItem(proto));
        return false;
    }

    return true;
}
