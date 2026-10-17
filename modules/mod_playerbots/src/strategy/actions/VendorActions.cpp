/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "VendorActions.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"

Creature* RepairAllAction::FindRepairNpc(PlayerbotAI* botAI)
{
    Player* bot = botAI->GetBot();
    for (ObjectGuid const& guid : botAI->GetAiObjectContext()->GetValue<GuidVector>("nearest npcs")->Get())
        if (Creature* creature = bot->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_REPAIR))
            return creature;

    return nullptr;
}

bool RepairAllAction::Execute(Event /*event*/)
{
    Creature* npc = FindRepairNpc(botAI);
    if (!npc)
    {
        botAI->TellMaster("There is no one to repair at nearby");
        return false;
    }

    bot->SetFacingToObject(npc);

    // As WorldSession::HandleRepairItemOpcode does for "repair all"
    float const discount = bot->GetReputationPriceDiscount(npc);
    uint32 const cost = bot->DurabilityRepairAll(true, discount, false);
    if (!cost)
        return false;

    botAI->TellMasterNoFacing("Repaired for " + ChatHelper::formatMoney(cost));
    return true;
}

bool SellJunkAction::Execute(Event /*event*/)
{
    Event sell("sell junk", "vendor");
    return SellAction::Execute(sell);
}
