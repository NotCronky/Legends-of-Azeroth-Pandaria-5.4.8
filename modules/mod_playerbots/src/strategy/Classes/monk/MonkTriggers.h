/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MONKTRIGGERS_H
#define _PLAYERBOT_MONKTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

// -- stances and buffs
BUFF_TRIGGER(StanceOfTheSturdyOxTrigger, "stance of the sturdy ox");
BUFF_TRIGGER(StanceOfTheFierceTigerTrigger, "stance of the fierce tiger");
BUFF_TRIGGER(StanceOfTheWiseSerpentTrigger, "stance of the wise serpent");
BUFF_TRIGGER(TigerPowerTrigger, "tiger power");
BUFF_TRIGGER(ShuffleTrigger, "shuffle");
BUFF_TRIGGER(GuardTrigger, "guard");

// No stats buff of any kind (Legacy of the Emperor, Mark of the Wild, Kings, Embrace of the Shale Spider)
class LegacyOfTheEmperorTrigger : public Trigger
{
public:
    LegacyOfTheEmperorTrigger(PlayerbotAI* botAI) : Trigger(botAI, "legacy of the emperor", 2) {}

    bool IsActive() override;
};

// -- procs and debuffs
HAS_AURA_TRIGGER(ComboBreakerTigerPalmTrigger, "combo breaker: tiger palm");
HAS_AURA_TRIGGER(ComboBreakerBlackoutKickTrigger, "combo breaker: blackout kick");
DEBUFF_CHECKISOWNER_TRIGGER(RisingSunKickTrigger, "rising sun kick");

// Moderate or heavy Stagger: Purifying Brew clears it
class HeavyStaggerTrigger : public Trigger
{
public:
    HeavyStaggerTrigger(PlayerbotAI* botAI) : Trigger(botAI, "heavy stagger") {}

    bool IsActive() override;
};

// Energy is low: Energizing Brew
class MonkLowEnergyTrigger : public SpellCanBeCastTrigger
{
public:
    MonkLowEnergyTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "energizing brew") {}

    bool IsActive() override;
};

// The target has less health than the monk, or is below 10%: Touch of Death
class TouchOfDeathTrigger : public SpellCanBeCastTrigger
{
public:
    TouchOfDeathTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "touch of death") {}

    bool IsActive() override;
};

CAN_CAST_TRIGGER(KegSmashTrigger, "keg smash");
CAN_CAST_TRIGGER(ChiWaveTrigger, "chi wave");
CAN_CAST_TRIGGER(InvokeXuenTrigger, "invoke xuen, the white tiger");
INTERRUPT_TRIGGER(SpearHandStrikeInterruptSpellTrigger, "spear hand strike");
INTERRUPT_HEALER_TRIGGER(SpearHandStrikeInterruptEnemyHealerSpellTrigger, "spear hand strike");

CURE_PARTY_TRIGGER(DetoxMagicOnPartyTrigger, "detox", DISPEL_MAGIC);
CURE_PARTY_TRIGGER(DetoxPoisonOnPartyTrigger, "detox", DISPEL_POISON);
CURE_PARTY_TRIGGER(DetoxDiseaseOnPartyTrigger, "detox", DISPEL_DISEASE);

#endif
