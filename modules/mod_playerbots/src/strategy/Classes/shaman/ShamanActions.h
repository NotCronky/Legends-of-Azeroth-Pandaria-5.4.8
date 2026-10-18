/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SHAMANACTIONS_H
#define _PLAYERBOT_SHAMANACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// A totem: not when one of the same name is already down, nor for a fight too short to use it
class CastTotemAction : public CastBuffSpellAction
{
public:
    CastTotemAction(PlayerbotAI* botAI, std::string const spell, std::string const buffName = "",
                    float needLifeTime = 8.0f)
        : CastBuffSpellAction(botAI, spell), needLifeTime(needLifeTime), buff(buffName.empty() ? spell : buffName)
    {
    }

    bool isUseful() override;

protected:
    float needLifeTime;
    std::string buff;
};

#define TOTEM_ACTION(clazz, spell, needLifeTime)                                                \
    class clazz : public CastTotemAction                                                        \
    {                                                                                           \
    public:                                                                                     \
        clazz(PlayerbotAI* botAI) : CastTotemAction(botAI, spell, "", needLifeTime) {}         \
    }

// -- shields and weapon imbues (an imbue is a weapon enchant, not an aura)
BUFF_ACTION(CastLightningShieldAction, "lightning shield");
BUFF_ACTION(CastWaterShieldAction, "water shield");
BUFF_ACTION(CastFlametongueWeaponAction, "flametongue weapon");
BUFF_ACTION(CastWindfuryWeaponAction, "windfury weapon");
BUFF_ACTION(CastEarthlivingWeaponAction, "earthliving weapon");

// -- shocks and spells
DEBUFF_CHECKISOWNER_ACTION(CastFlameShockAction, "flame shock");
SPELL_ACTION(CastEarthShockAction, "earth shock");
SPELL_ACTION(CastFrostShockAction, "frost shock");
SPELL_ACTION(CastLightningBoltAction, "lightning bolt");
SPELL_ACTION(CastChainLightningAction, "chain lightning");
SPELL_ACTION(CastLavaBurstAction, "lava burst");
SPELL_ACTION(CastEarthquakeAction, "earthquake");
SPELL_ACTION(CastUnleashElementsAction, "unleash elements");
SPELL_ACTION(CastWindShearAction, "wind shear");
ENEMY_HEALER_ACTION(CastWindShearOnEnemyHealerAction, "wind shear");

// -- enhancement
MELEE_ACTION(CastStormstrikeAction, "stormstrike");
MELEE_ACTION(CastPrimalStrikeAction, "primal strike");
MELEE_ACTION(CastLavaLashAction, "lava lash");
SPELL_ACTION(CastFireNovaAction, "fire nova");
BUFF_ACTION(CastFeralSpiritAction, "feral spirit");

// -- cooldowns
BUFF_ACTION(CastAscendanceAction, "ascendance");
BUFF_ACTION(CastElementalMasteryAction, "elemental mastery");
BUFF_ACTION(CastAstralShiftAction, "astral shift");
BUFF_ACTION(CastShamanisticRageAction, "shamanistic rage");

// -- totems
TOTEM_ACTION(CastSearingTotemAction, "searing totem", 8.0f);
TOTEM_ACTION(CastMagmaTotemAction, "magma totem", 4.0f);
TOTEM_ACTION(CastFireElementalTotemAction, "fire elemental totem", 15.0f);
TOTEM_ACTION(CastHealingStreamTotemAction, "healing stream totem", 0.0f);
TOTEM_ACTION(CastHealingTideTotemAction, "healing tide totem", 0.0f);
TOTEM_ACTION(CastManaTideTotemAction, "mana tide totem", 0.0f);

// -- heals
HEAL_PARTY_ACTION(CastRiptideOnPartyAction, "riptide", 15.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastHealingWaveOnPartyAction, "healing wave", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastGreaterHealingWaveOnPartyAction, "greater healing wave", 30.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastHealingSurgeOnPartyAction, "healing surge", 25.0f, HealingManaEfficiency::LOW);
HEAL_PARTY_ACTION(CastChainHealOnPartyAction, "chain heal", 20.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastHealingRainOnPartyAction, "healing rain", 20.0f, HealingManaEfficiency::MEDIUM);
HEAL_ACTION(CastHealingSurgeAction, "healing surge");

class CastEarthShieldOnMainTankAction : public BuffOnMainTankAction
{
public:
    CastEarthShieldOnMainTankAction(PlayerbotAI* botAI) : BuffOnMainTankAction(botAI, "earth shield", true) {}
};

// -- cleanse and resurrect
CURE_PARTY_ACTION(CastCleanseSpiritOnPartyAction, "cleanse spirit", DISPEL_CURSE);
CURE_PARTY_ACTION(CastPurifySpiritCurseOnPartyAction, "purify spirit", DISPEL_CURSE);
CURE_PARTY_ACTION(CastPurifySpiritMagicOnPartyAction, "purify spirit", DISPEL_MAGIC);
RESS_ACTION(CastAncestralSpiritAction, "ancestral spirit");

#endif
