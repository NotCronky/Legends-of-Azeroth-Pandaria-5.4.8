/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_ROGUETRIGGERS_H
#define _PLAYERBOT_ROGUETRIGGERS_H

#include "GenericTriggers.h"

class Player;
class PlayerbotAI;

// Combo points on the current target
uint8 RogueComboPoints(PlayerbotAI* botAI, Player* bot);

// At least an amount of combo points on the current target
class RogueComboPointsTrigger : public Trigger
{
public:
    RogueComboPointsTrigger(PlayerbotAI* botAI, std::string const name, uint8 amount)
        : Trigger(botAI, name), amount(amount) {}

    bool IsActive() override;

private:
    uint8 amount;
};

// Five combo points: spend them
class RogueFinisherTrigger : public RogueComboPointsTrigger
{
public:
    RogueFinisherTrigger(PlayerbotAI* botAI) : RogueComboPointsTrigger(botAI, "finisher", 5) {}
};

// Some combo points on a target about to die: spend them before they are lost
class RogueTargetDyingTrigger : public RogueComboPointsTrigger
{
public:
    RogueTargetDyingTrigger(PlayerbotAI* botAI) : RogueComboPointsTrigger(botAI, "combo points target dying", 2) {}

    bool IsActive() override;
};

// Slice and Dice is missing or about to end, and there is a combo point to renew it with
class SliceAndDiceTrigger : public Trigger
{
public:
    SliceAndDiceTrigger(PlayerbotAI* botAI) : Trigger(botAI, "slice and dice") {}

    bool IsActive() override;
};

// Our Rupture is missing on a target that will live long enough, and five combo points to cast it with
class RuptureTrigger : public DebuffTrigger
{
public:
    RuptureTrigger(PlayerbotAI* botAI) : DebuffTrigger(botAI, "rupture", 1, true, 12.0f) {}

    bool IsActive() override;
};

DEBUFF_CHECKISOWNER_TRIGGER(RevealingStrikeTrigger, "revealing strike");
DEBUFF_CHECKISOWNER_TRIGGER(HemorrhageTrigger, "hemorrhage");
HAS_AURA_TRIGGER(BlindsideTrigger, "blindside");
INTERRUPT_TRIGGER(KickInterruptSpellTrigger, "kick");
INTERRUPT_HEALER_TRIGGER(KickInterruptEnemyHealerSpellTrigger, "kick");

// In stealth, Vanish, Shadow Dance or Subterfuge: the stealth attacks work
class RogueStealthedTrigger : public Trigger
{
public:
    RogueStealthedTrigger(PlayerbotAI* botAI) : Trigger(botAI, "stealthed") {}

    bool IsActive() override;
};

// Shadow Dance is ready and there is the energy to fill it with Ambushes
class ShadowDanceTrigger : public SpellCanBeCastTrigger
{
public:
    ShadowDanceTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "shadow dance") {}

    bool IsActive() override;
};

// Killing Spree is ready, low on energy and not in Adrenaline Rush
class KillingSpreeTrigger : public SpellCanBeCastTrigger
{
public:
    KillingSpreeTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "killing spree") {}

    bool IsActive() override;
};

CAN_CAST_TRIGGER(VendettaTrigger, "vendetta");
CAN_CAST_TRIGGER(AdrenalineRushTrigger, "adrenaline rush");
CAN_CAST_TRIGGER(ShadowBladesTrigger, "shadow blades");

// No lethal (Deadly, Wound) or non-lethal (Crippling, Mind-numbing and their talents) poison on
class RoguePoisonTrigger : public Trigger
{
public:
    RoguePoisonTrigger(PlayerbotAI* botAI, std::string const name, bool lethal)
        : Trigger(botAI, name, 2), lethal(lethal) {}

    bool IsActive() override;

private:
    bool lethal;
};

class TricksOfTheTradeOnMainTankTrigger : public BuffOnMainTankTrigger
{
public:
    TricksOfTheTradeOnMainTankTrigger(PlayerbotAI* botAI) : BuffOnMainTankTrigger(botAI, "tricks of the trade", true) {}
};

#endif
