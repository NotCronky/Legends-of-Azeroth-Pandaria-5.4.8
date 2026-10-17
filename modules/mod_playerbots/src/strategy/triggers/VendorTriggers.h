#ifndef _PLAYERBOT_VENDORTRIGGERS_H
#define _PLAYERBOT_VENDORTRIGGERS_H

#include "Trigger.h"

class PlayerbotAI;

// Damaged gear, a repair NPC in reach and the money to pay it
class CanRepairTrigger : public Trigger
{
public:
    CanRepairTrigger(PlayerbotAI* botAI) : Trigger(botAI, "can repair", 5) {}

    bool IsActive() override;
};

// Items only worth selling, and a vendor in reach
class CanSellTrigger : public Trigger
{
public:
    CanSellTrigger(PlayerbotAI* botAI) : Trigger(botAI, "can sell", 5) {}

    bool IsActive() override;
};

#endif
