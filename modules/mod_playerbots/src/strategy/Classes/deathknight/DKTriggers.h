/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DKTRIGGERS_H
#define _PLAYERBOT_DKTRIGGERS_H

#include "GenericTriggers.h"

class PlayerbotAI;

// -- presences and buffs
BUFF_TRIGGER(BloodPresenceTrigger, "blood presence");
BUFF_TRIGGER(FrostPresenceTrigger, "frost presence");
BUFF_TRIGGER(UnholyPresenceTrigger, "unholy presence");
BUFF_TRIGGER(HornOfWinterTrigger, "horn of winter");
BUFF_TRIGGER(BoneShieldTrigger, "bone shield");

// -- our diseases on the target
DEBUFF_CHECKISOWNER_TRIGGER(FrostFeverTrigger, "frost fever");
DEBUFF_CHECKISOWNER_TRIGGER(BloodPlagueTrigger, "blood plague");

// -- procs
HAS_AURA_TRIGGER(KillingMachineTrigger, "killing machine");
HAS_AURA_TRIGGER(FreezingFogTrigger, "freezing fog");
HAS_AURA_TRIGGER(SuddenDoomTrigger, "sudden doom");
HAS_AURA_TRIGGER(CrimsonScourgeTrigger, "crimson scourge");

// Runic power near the cap: spend it before it is wasted
class HighRunicPowerTrigger : public Trigger
{
public:
    HighRunicPowerTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high runic power") {}

    bool IsActive() override;
};

// Every rune on cooldown: Empower Rune Weapon refreshes them
class NoRunesTrigger : public Trigger
{
public:
    NoRunesTrigger(PlayerbotAI* botAI) : Trigger(botAI, "no runes") {}

    bool IsActive() override;
};

// Five Shadow Infusion stacks on the ghoul: Dark Transformation
class ShadowInfusionTrigger : public HasAuraStackTrigger
{
public:
    ShadowInfusionTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "shadow infusion", 5) {}

    Value<Unit*>* GetTargetValue() override;
};

// Enough Blood Charges for Blood Tap to bring back a rune
class BloodChargeTrigger : public HasAuraStackTrigger
{
public:
    BloodChargeTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "blood charge", 10) {}
};

// The target is below 35% health and Soul Reaper is ready
class SoulReaperTrigger : public SpellCanBeCastTrigger
{
public:
    SoulReaperTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "soul reaper") {}

    bool IsActive() override;
};

// Unholy's ghoul is missing (the others raise theirs as a cooldown)
class DKNoGhoulTrigger : public Trigger
{
public:
    DKNoGhoulTrigger(PlayerbotAI* botAI) : Trigger(botAI, "dk no ghoul", 2) {}

    bool IsActive() override;
};

CAN_CAST_TRIGGER(PillarOfFrostTrigger, "pillar of frost");
CAN_CAST_TRIGGER(SummonGargoyleTrigger, "summon gargoyle");
CAN_CAST_TRIGGER(UnholyFrenzyTrigger, "unholy frenzy");
CAN_CAST_TRIGGER(DancingRuneWeaponTrigger, "dancing rune weapon");
CAN_CAST_TRIGGER(RaiseDeadTrigger, "raise dead");
INTERRUPT_TRIGGER(MindFreezeInterruptSpellTrigger, "mind freeze");
INTERRUPT_HEALER_TRIGGER(MindFreezeInterruptEnemyHealerSpellTrigger, "mind freeze");

#endif
