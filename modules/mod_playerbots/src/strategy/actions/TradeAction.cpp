/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "TradeAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"
#include "TradeData.h"

bool TradeAction::Execute(Event event)
{
    if (!CheckManager(event))
        return false;

    std::string const text = event.getParam();
    Player* owner = event.getOwner() ? event.getOwner() : GetMaster();
    if (!owner)
        return false;

    if (Player* trader = bot->GetTrader())
    {
        if (trader != owner)
        {
            botAI->TellMaster("I'm trading with someone else");
            return false;
        }

        // The window is open but not set up yet: TradeStatusAction adds the items when it is
        if (!AI_VALUE(bool, "trade initiated"))
        {
            context->GetValue<std::string>("pending trade")->Set(text);
            return true;
        }

        return text.empty() || AddToTrade(text);
    }

    if (!bot->IsWithinDistInMap(owner, 10.0f, false))
    {
        botAI->TellMaster("You are too far away to trade");
        return false;
    }

    // CMSG_INITIATE_TRADE in the order WorldSession::HandleInitiateTradeOpcode reads it
    ObjectGuid const guid = owner->GetGUID();
    WorldPacket packet(CMSG_INITIATE_TRADE, 1 + 8);
    packet.WriteBit(guid[5]);
    packet.WriteBit(guid[1]);
    packet.WriteBit(guid[4]);
    packet.WriteBit(guid[2]);
    packet.WriteBit(guid[3]);
    packet.WriteBit(guid[7]);
    packet.WriteBit(guid[0]);
    packet.WriteBit(guid[6]);
    packet.FlushBits();
    packet.WriteByteSeq(guid[4]);
    packet.WriteByteSeq(guid[6]);
    packet.WriteByteSeq(guid[2]);
    packet.WriteByteSeq(guid[0]);
    packet.WriteByteSeq(guid[3]);
    packet.WriteByteSeq(guid[7]);
    packet.WriteByteSeq(guid[5]);
    packet.WriteByteSeq(guid[1]);
    context->GetValue<bool>("trade initiated")->Set(false);
    bot->GetSession()->HandleInitiateTradeOpcode(packet);

    if (bot->GetTrader() != owner)
    {
        botAI->TellMaster("I can't trade with you right now");
        return false;
    }

    context->GetValue<std::string>("pending trade")->Set(text);
    return true;
}

bool TradeAction::AddToTrade(std::string const& text)
{
    TradeData* trade = bot->GetTradeData();
    if (!trade)
        return false;

    if (uint32 copper = ChatHelper::parseMoney(text))
    {
        if (copper > bot->GetMoney())
        {
            botAI->TellMaster("I only have " + ChatHelper::formatMoney(uint32(bot->GetMoney())));
            return false;
        }

        WorldPacket packet(CMSG_SET_TRADE_GOLD, 8);
        packet << uint64(copper);
        bot->GetSession()->HandleSetTradeGoldOpcode(packet);
        return true;
    }

    std::vector<Item*> const items = parseItems(text);
    if (items.empty())
    {
        botAI->TellMaster("I don't have " + text);
        return false;
    }

    bool traded = false;
    for (Item* item : items)
    {
        if (trade->HasItem(item->GetGUID()))
            continue;

        if (!TradeItem(item))
            break;

        traded = true;
    }

    return traded;
}

bool TradeAction::TradeItem(Item* item)
{
    TradeData* trade = bot->GetTradeData();
    std::string const link = ChatHelper::FormatItem(item->GetTemplate());
    if (!item->CanBeTraded(false, true))
    {
        botAI->TellMaster("I can't trade " + link);
        return true;
    }

    for (uint8 slot = 0; slot < TRADE_SLOT_TRADED_COUNT; ++slot)
    {
        if (trade->GetItem(TradeSlots(slot)))
            continue;

        // CMSG_SET_TRADE_ITEM as WorldSession::HandleSetTradeItemOpcode reads it
        WorldPacket packet(CMSG_SET_TRADE_ITEM, 3);
        packet << uint8(slot) << uint8(item->GetSlot()) << uint8(item->GetBagSlot());
        bot->GetSession()->HandleSetTradeItemOpcode(packet);
        return true;
    }

    botAI->TellMaster("The trade window is full");
    return false;
}

bool TradeStatusAction::Execute(Event event)
{
    WorldPacket packet(event.getPacket());
    packet.rpos(0);
    packet.ResetBitPos();
    packet.ReadBit();
    uint32 const status = packet.ReadBits(5);

    Player* trader = bot->GetTrader();
    switch (status)
    {
        case TRADE_STATUS_PROPOSED:
        {
            // Someone opened a trade with the bot: only players the bot takes commands from
            context->GetValue<bool>("trade initiated")->Set(false);
            if (!trader)
                return false;

            if (GET_PLAYERBOT_AI(trader) || !botAI->CanTakeCommandFrom(trader))
            {
                bot->Whisper("I'm kind of busy now", LANG_UNIVERSAL, trader);
                CancelTrade();
                return false;
            }

            WorldPacket begin(CMSG_BEGIN_TRADE);
            bot->GetSession()->HandleBeginTradeOpcode(begin);
            return true;
        }
        case TRADE_STATUS_INITIATED:
        {
            context->GetValue<bool>("trade initiated")->Set(true);
            std::string const pending = AI_VALUE(std::string, "pending trade");
            context->GetValue<std::string>("pending trade")->Set("");
            return pending.empty() || AddToTrade(pending);
        }
        case TRADE_STATUS_ACCEPTED:
        {
            // The other side accepted: accept too. Items only go in on the master's command (TradeAction)
            if (!trader || !bot->GetTradeData() || bot->GetTradeData()->IsAccepted() ||
                !botAI->CanTakeCommandFrom(trader))
                return false;

            WorldPacket accept(CMSG_ACCEPT_TRADE);
            bot->GetSession()->HandleAcceptTradeOpcode(accept);
            return true;
        }
        case TRADE_STATUS_COMPLETE:
            context->GetValue<bool>("trade initiated")->Set(false);
            botAI->TellMaster("Thank you");
            return true;
        case TRADE_STATUS_CANCELLED:
            context->GetValue<bool>("trade initiated")->Set(false);
            context->GetValue<std::string>("pending trade")->Set("");
            return false;
        default:
            return false;
    }
}

void TradeStatusAction::CancelTrade()
{
    context->GetValue<std::string>("pending trade")->Set("");
    WorldPacket packet(CMSG_CANCEL_TRADE);
    bot->GetSession()->HandleCancelTradeOpcode(packet);
}
