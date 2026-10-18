/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SHAMANTRIGGERS_H
#define _PLAYERBOT_SHAMANTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

// Restoration keeps Water Shield up, the others Lightning Shield (one replaces the other)
class ShamanShieldTrigger : public BuffTrigger
{
public:
    ShamanShieldTrigger(PlayerbotAI* botAI, std::string const spell, bool restoration)
        : BuffTrigger(botAI, spell), restoration(restoration) {}

    bool IsActive() override;

private:
    bool restoration;
};

// The main hand has no imbue (a temporary weapon enchant): this specialization's one, if it is known
class ShamanWeaponImbueTrigger : public Trigger
{
public:
    ShamanWeaponImbueTrigger(PlayerbotAI* botAI, std::string const name, uint32 spellId, Specializations spec)
        : Trigger(botAI, name, 5), spellId(spellId), spec(spec) {}

    bool IsActive() override;

private:
    uint32 spellId;
    Specializations spec;
};

DEBUFF_CHECKISOWNER_TRIGGER(FlameShockTrigger, "flame shock");
HAS_AURA_TRIGGER(LavaSurgeTrigger, "lava surge");

// Lightning Shield charges built by Fulmination: Earth Shock spends them
class FulminationTrigger : public HasAuraStackTrigger
{
public:
    FulminationTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "lightning shield", 7) {}
};

// Five Maelstrom Weapon stacks: an instant Lightning Bolt
class MaelstromWeaponTrigger : public HasAuraStackTrigger
{
public:
    MaelstromWeaponTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "maelstrom weapon", 5) {}
};

// No fire totem down in combat: Searing Totem
class NoFireTotemTrigger : public Trigger
{
public:
    NoFireTotemTrigger(PlayerbotAI* botAI) : Trigger(botAI, "no fire totem", 2) {}

    bool IsActive() override;
};

CAN_CAST_TRIGGER(AscendanceTrigger, "ascendance");
CAN_CAST_TRIGGER(ElementalMasteryTrigger, "elemental mastery");
CAN_CAST_TRIGGER(FeralSpiritTrigger, "feral spirit");
CAN_CAST_TRIGGER(FireElementalTotemTrigger, "fire elemental totem");

class EarthShieldOnMainTankTrigger : public BuffOnMainTankTrigger
{
public:
    EarthShieldOnMainTankTrigger(PlayerbotAI* botAI) : BuffOnMainTankTrigger(botAI, "earth shield", true) {}
};

INTERRUPT_TRIGGER(WindShearInterruptSpellTrigger, "wind shear");
INTERRUPT_HEALER_TRIGGER(WindShearInterruptEnemyHealerSpellTrigger, "wind shear");

CURE_PARTY_TRIGGER(CleanseSpiritOnPartyTrigger, "cleanse spirit", DISPEL_CURSE);
CURE_PARTY_TRIGGER(PurifySpiritCurseOnPartyTrigger, "purify spirit", DISPEL_CURSE);
CURE_PARTY_TRIGGER(PurifySpiritMagicOnPartyTrigger, "purify spirit", DISPEL_MAGIC);

#endif
