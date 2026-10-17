#ifndef _PLAYERBOT_UPKEEPTRIGGERS_H
#define _PLAYERBOT_UPKEEPTRIGGERS_H

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

// Mounted while fighting
class MountedTrigger : public Trigger
{
public:
    MountedTrigger(PlayerbotAI* botAI) : Trigger(botAI, "mounted") {}

    bool IsActive() override;
};

#endif
