/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MAGETRIGGERS_H
#define _PLAYERBOT_MAGETRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

BUFF_TRIGGER(ArcaneBrillianceTrigger, "arcane brilliance");

// The specialization's armor is missing (Mage Armor for Arcane, Molten for Fire, Frost Armor for Frost)
class MageArmorTrigger : public BuffTrigger
{
public:
    MageArmorTrigger(PlayerbotAI* botAI, std::string const spell, Specializations spec)
        : BuffTrigger(botAI, spell, 2), spec(spec) {}

    bool IsActive() override;

private:
    Specializations spec;
};

// -- arcane
HAS_AURA_TRIGGER(ArcaneMissilesProcTrigger, "arcane missiles!");
DEBUFF_CHECKISOWNER_TRIGGER(NetherTempestTrigger, "nether tempest");
CAN_CAST_TRIGGER(ArcanePowerTrigger, "arcane power");

// Four Arcane Charges and mana running low: Arcane Barrage spends them
class ArcaneChargesTrigger : public HasAuraStackTrigger
{
public:
    ArcaneChargesTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "arcane charge", 4) {}

    bool IsActive() override;
};

// -- fire
HAS_AURA_TRIGGER(PyroblastProcTrigger, "pyroblast!");
HAS_AURA_TRIGGER(HeatingUpTrigger, "heating up");
DEBUFF_CHECKISOWNER_TRIGGER(LivingBombTrigger, "living bomb");
CAN_CAST_TRIGGER(CombustionTrigger, "combustion");

// -- frost
HAS_AURA_TRIGGER(FingersOfFrostTrigger, "fingers of frost");
HAS_AURA_TRIGGER(BrainFreezeTrigger, "brain freeze");
DEBUFF_CHECKISOWNER_TRIGGER(FrostBombTrigger, "frost bomb");
CAN_CAST_TRIGGER(FrozenOrbTrigger, "frozen orb");
CAN_CAST_TRIGGER(IcyVeinsTrigger, "icy veins");

// Frost's Water Elemental is missing
class WaterElementalTrigger : public Trigger
{
public:
    WaterElementalTrigger(PlayerbotAI* botAI) : Trigger(botAI, "water elemental", 2) {}

    bool IsActive() override;
};

// -- every specialization
CAN_CAST_TRIGGER(MirrorImageTrigger, "mirror image");
INTERRUPT_TRIGGER(CounterspellInterruptSpellTrigger, "counterspell");
INTERRUPT_HEALER_TRIGGER(CounterspellInterruptEnemyHealerSpellTrigger, "counterspell");
CURE_PARTY_TRIGGER(RemoveCurseOnPartyTrigger, "remove curse", DISPEL_CURSE);

#endif
