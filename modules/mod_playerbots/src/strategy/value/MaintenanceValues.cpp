/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MaintenanceValues.h"

#include "Bag.h"
#include "ItemUsageValue.h"
#include "Playerbots.h"
#include "StatsAction.h"
#include "VendorActions.h"

bool CanMoveAroundValue::Calculate()
{
    if (bot->IsInCombat())
        return false;

    if (bot->GetTradeData())
        return false;

    if (botAI->HasStrategy("stay", BOT_STATE_NON_COMBAT))
        return false;

    if (!AI_VALUE(bool, "group ready"))
        return false;

    return true;
}

bool ShouldHomeBindValue::Calculate()
{
    return false;
    //return AI_VALUE2(float, "distance", "home bind") > 1000.0f;
}

bool ShouldRepairValue::Calculate() { return AI_VALUE(uint8, "durability") < 80; }

bool CanRepairValue::Calculate()
{
    if (AI_VALUE(uint8, "durability") >= 100 || !RepairAllAction::FindRepairNpc(botAI))
        return false;

    uint32 const cost = StatsAction::RepairCost(bot);
    return cost && cost <= bot->GetMoney();
}

bool ShouldSellValue::Calculate() { return AI_VALUE(uint8, "bag space") > 80; }

bool CanSellValue::Calculate()
{
    bool hasVendor = false;
    for (ObjectGuid const& guid : AI_VALUE(GuidVector, "nearest npcs"))
        if (bot->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_VENDOR))
        {
            hasVendor = true;
            break;
        }

    return hasVendor && AI_VALUE(bool, "has junk");
}

bool HasJunkValue::Calculate()
{
    for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
        if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            if (IsJunk(item))
                return true;

    for (uint8 bagSlot = INVENTORY_SLOT_BAG_START; bagSlot < INVENTORY_SLOT_BAG_END; ++bagSlot)
        if (Bag* bag = bot->GetBagByPos(bagSlot))
            for (uint32 slot = 0; slot < bag->GetBagSize(); ++slot)
                if (Item* item = bag->GetItemByPos(slot))
                    if (IsJunk(item))
                        return true;

    return false;
}

bool HasJunkValue::IsJunk(Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    return proto->SellPrice &&
           AI_VALUE2(ItemUsage, "item usage", ItemUsageValue::Qualify(proto->ItemId)) == ITEM_USAGE_VENDOR;
}

bool CanFightEqualValue::Calculate()
{
    return AI_VALUE(uint8, "durability") > 20;
}

bool CanFightEliteValue::Calculate()
{
    return false;
    /*return bot->GetGroup() && AI_VALUE2(bool, "group and", "can fight equal") &&
        AI_VALUE2(bool, "group and", "following party") && !AI_VALUE2(bool, "group or", "should sell,can sell");*/
}

bool CanFightBossValue::Calculate()
{
    return false;
    //return bot->GetGroup() && bot->GetGroup()->GetMembersCount() > 3 &&
    //    AI_VALUE2(bool, "group and", "can fight equal") && AI_VALUE2(bool, "group and", "following party") &&
    //    !AI_VALUE2(bool, "group or", "should sell,can sell");
}
