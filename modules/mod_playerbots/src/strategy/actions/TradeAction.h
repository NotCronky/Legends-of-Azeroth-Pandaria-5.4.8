/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_TRADEACTION_H
#define _PLAYERBOT_TRADEACTION_H

#include "InventoryAction.h"

class PlayerbotAI;

// "t <items or money>": open a trade with the commander if needed and put items or gold in it
class TradeAction : public InventoryAction
{
public:
    TradeAction(PlayerbotAI* botAI, std::string const name = "trade") : InventoryAction(botAI, name) {}

    bool Execute(Event event) override;

protected:
    // Puts what the text describes in the open trade window
    bool AddToTrade(std::string const& text);

private:
    bool TradeItem(Item* item);
};

// Answers SMSG_TRADE_STATUS: opens trades from the bot's master and group, adds pending items once the
// window is up and accepts when the other side accepts
class TradeStatusAction : public TradeAction
{
public:
    TradeStatusAction(PlayerbotAI* botAI) : TradeAction(botAI, "trade status") {}

    bool Execute(Event event) override;

private:
    void CancelTrade();
};

#endif
