/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ShamanAiObjectContext.h"

#include "NamedObjectContext.h"
#include "Playerbots.h"
#include "ShamanActions.h"
#include "ShamanNonCombatStrategy.h"
#include "ShamanStrategies.h"
#include "ShamanTriggers.h"

namespace
{
    class ShamanStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        ShamanStrategyFactoryInternal()
        {
            creators["nc"] = &ShamanStrategyFactoryInternal::nc;
            creators["cure"] = &ShamanStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new ShamanNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new ShamanCureStrategy(botAI); }
    };

    // One specialization at a time
    class ShamanCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        ShamanCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["caster"] = &ShamanCombatStrategyFactoryInternal::elemental;
            creators["elemental"] = &ShamanCombatStrategyFactoryInternal::elemental;
            creators["melee"] = &ShamanCombatStrategyFactoryInternal::enhancement;
            creators["enhancement"] = &ShamanCombatStrategyFactoryInternal::enhancement;
            creators["heal"] = &ShamanCombatStrategyFactoryInternal::restoration;
            creators["restoration"] = &ShamanCombatStrategyFactoryInternal::restoration;
        }

    private:
        static Strategy* elemental(PlayerbotAI* botAI) { return new ElementalShamanStrategy(botAI); }
        static Strategy* enhancement(PlayerbotAI* botAI) { return new EnhancementShamanStrategy(botAI); }
        static Strategy* restoration(PlayerbotAI* botAI) { return new RestorationShamanStrategy(botAI); }
    };

    class ShamanTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        ShamanTriggerFactoryInternal()
        {
            creators["lightning shield"] = &ShamanTriggerFactoryInternal::lightning_shield;
            creators["water shield"] = &ShamanTriggerFactoryInternal::water_shield;
            creators["flametongue weapon"] = &ShamanTriggerFactoryInternal::flametongue_weapon;
            creators["windfury weapon"] = &ShamanTriggerFactoryInternal::windfury_weapon;
            creators["earthliving weapon"] = &ShamanTriggerFactoryInternal::earthliving_weapon;
            creators["flame shock"] = &ShamanTriggerFactoryInternal::flame_shock;
            creators["lava surge"] = &ShamanTriggerFactoryInternal::lava_surge;
            creators["fulmination"] = &ShamanTriggerFactoryInternal::fulmination;
            creators["maelstrom weapon"] = &ShamanTriggerFactoryInternal::maelstrom_weapon;
            creators["no fire totem"] = &ShamanTriggerFactoryInternal::no_fire_totem;
            creators["ascendance"] = &ShamanTriggerFactoryInternal::ascendance;
            creators["elemental mastery"] = &ShamanTriggerFactoryInternal::elemental_mastery;
            creators["feral spirit"] = &ShamanTriggerFactoryInternal::feral_spirit;
            creators["fire elemental totem"] = &ShamanTriggerFactoryInternal::fire_elemental_totem;
            creators["earth shield on main tank"] = &ShamanTriggerFactoryInternal::earth_shield_on_main_tank;
            creators["wind shear"] = &ShamanTriggerFactoryInternal::wind_shear;
            creators["wind shear on enemy healer"] = &ShamanTriggerFactoryInternal::wind_shear_on_enemy_healer;
            creators["cleanse spirit on party"] = &ShamanTriggerFactoryInternal::cleanse_spirit_on_party;
            creators["purify spirit curse on party"] = &ShamanTriggerFactoryInternal::purify_spirit_curse_on_party;
            creators["purify spirit magic on party"] = &ShamanTriggerFactoryInternal::purify_spirit_magic_on_party;
        }

    private:
        static Trigger* lightning_shield(PlayerbotAI* botAI) { return new ShamanShieldTrigger(botAI, "lightning shield", false); }
        static Trigger* water_shield(PlayerbotAI* botAI) { return new ShamanShieldTrigger(botAI, "water shield", true); }
        static Trigger* flametongue_weapon(PlayerbotAI* botAI) { return new ShamanWeaponImbueTrigger(botAI, "flametongue weapon", 8024, Specializations::SPEC_SHAMAN_ELEMENTAL); }
        static Trigger* windfury_weapon(PlayerbotAI* botAI) { return new ShamanWeaponImbueTrigger(botAI, "windfury weapon", 8232, Specializations::SPEC_SHAMAN_ENHANCEMENT); }
        static Trigger* earthliving_weapon(PlayerbotAI* botAI) { return new ShamanWeaponImbueTrigger(botAI, "earthliving weapon", 51730, Specializations::SPEC_SHAMAN_RESTORATION); }
        static Trigger* flame_shock(PlayerbotAI* botAI) { return new FlameShockTrigger(botAI); }
        static Trigger* lava_surge(PlayerbotAI* botAI) { return new LavaSurgeTrigger(botAI); }
        static Trigger* fulmination(PlayerbotAI* botAI) { return new FulminationTrigger(botAI); }
        static Trigger* maelstrom_weapon(PlayerbotAI* botAI) { return new MaelstromWeaponTrigger(botAI); }
        static Trigger* no_fire_totem(PlayerbotAI* botAI) { return new NoFireTotemTrigger(botAI); }
        static Trigger* ascendance(PlayerbotAI* botAI) { return new AscendanceTrigger(botAI); }
        static Trigger* elemental_mastery(PlayerbotAI* botAI) { return new ElementalMasteryTrigger(botAI); }
        static Trigger* feral_spirit(PlayerbotAI* botAI) { return new FeralSpiritTrigger(botAI); }
        static Trigger* fire_elemental_totem(PlayerbotAI* botAI) { return new FireElementalTotemTrigger(botAI); }
        static Trigger* earth_shield_on_main_tank(PlayerbotAI* botAI) { return new EarthShieldOnMainTankTrigger(botAI); }
        static Trigger* wind_shear(PlayerbotAI* botAI) { return new WindShearInterruptSpellTrigger(botAI); }
        static Trigger* wind_shear_on_enemy_healer(PlayerbotAI* botAI) { return new WindShearInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* cleanse_spirit_on_party(PlayerbotAI* botAI) { return new CleanseSpiritOnPartyTrigger(botAI); }
        static Trigger* purify_spirit_curse_on_party(PlayerbotAI* botAI) { return new PurifySpiritCurseOnPartyTrigger(botAI); }
        static Trigger* purify_spirit_magic_on_party(PlayerbotAI* botAI) { return new PurifySpiritMagicOnPartyTrigger(botAI); }
    };

    class ShamanActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        ShamanActionFactoryInternal()
        {
            creators["lightning shield"] = &ShamanActionFactoryInternal::lightning_shield;
            creators["water shield"] = &ShamanActionFactoryInternal::water_shield;
            creators["flametongue weapon"] = &ShamanActionFactoryInternal::flametongue_weapon;
            creators["windfury weapon"] = &ShamanActionFactoryInternal::windfury_weapon;
            creators["earthliving weapon"] = &ShamanActionFactoryInternal::earthliving_weapon;

            creators["flame shock"] = &ShamanActionFactoryInternal::flame_shock;
            creators["earth shock"] = &ShamanActionFactoryInternal::earth_shock;
            creators["frost shock"] = &ShamanActionFactoryInternal::frost_shock;
            creators["lightning bolt"] = &ShamanActionFactoryInternal::lightning_bolt;
            creators["chain lightning"] = &ShamanActionFactoryInternal::chain_lightning;
            creators["lava burst"] = &ShamanActionFactoryInternal::lava_burst;
            creators["earthquake"] = &ShamanActionFactoryInternal::earthquake;
            creators["unleash elements"] = &ShamanActionFactoryInternal::unleash_elements;
            creators["wind shear"] = &ShamanActionFactoryInternal::wind_shear;
            creators["wind shear on enemy healer"] = &ShamanActionFactoryInternal::wind_shear_on_enemy_healer;

            creators["stormstrike"] = &ShamanActionFactoryInternal::stormstrike;
            creators["primal strike"] = &ShamanActionFactoryInternal::primal_strike;
            creators["lava lash"] = &ShamanActionFactoryInternal::lava_lash;
            creators["fire nova"] = &ShamanActionFactoryInternal::fire_nova;
            creators["feral spirit"] = &ShamanActionFactoryInternal::feral_spirit;

            creators["ascendance"] = &ShamanActionFactoryInternal::ascendance;
            creators["elemental mastery"] = &ShamanActionFactoryInternal::elemental_mastery;
            creators["astral shift"] = &ShamanActionFactoryInternal::astral_shift;
            creators["shamanistic rage"] = &ShamanActionFactoryInternal::shamanistic_rage;

            creators["searing totem"] = &ShamanActionFactoryInternal::searing_totem;
            creators["magma totem"] = &ShamanActionFactoryInternal::magma_totem;
            creators["fire elemental totem"] = &ShamanActionFactoryInternal::fire_elemental_totem;
            creators["healing stream totem"] = &ShamanActionFactoryInternal::healing_stream_totem;
            creators["healing tide totem"] = &ShamanActionFactoryInternal::healing_tide_totem;
            creators["mana tide totem"] = &ShamanActionFactoryInternal::mana_tide_totem;

            creators["riptide on party"] = &ShamanActionFactoryInternal::riptide_on_party;
            creators["healing wave on party"] = &ShamanActionFactoryInternal::healing_wave_on_party;
            creators["greater healing wave on party"] = &ShamanActionFactoryInternal::greater_healing_wave_on_party;
            creators["healing surge on party"] = &ShamanActionFactoryInternal::healing_surge_on_party;
            creators["chain heal on party"] = &ShamanActionFactoryInternal::chain_heal_on_party;
            creators["healing rain on party"] = &ShamanActionFactoryInternal::healing_rain_on_party;
            creators["healing surge"] = &ShamanActionFactoryInternal::healing_surge;
            creators["earth shield on main tank"] = &ShamanActionFactoryInternal::earth_shield_on_main_tank;

            creators["cleanse spirit on party"] = &ShamanActionFactoryInternal::cleanse_spirit_on_party;
            creators["purify spirit curse on party"] = &ShamanActionFactoryInternal::purify_spirit_curse_on_party;
            creators["purify spirit magic on party"] = &ShamanActionFactoryInternal::purify_spirit_magic_on_party;
            creators["ancestral spirit"] = &ShamanActionFactoryInternal::ancestral_spirit;
        }

    private:
        static Action* lightning_shield(PlayerbotAI* botAI) { return new CastLightningShieldAction(botAI); }
        static Action* water_shield(PlayerbotAI* botAI) { return new CastWaterShieldAction(botAI); }
        static Action* flametongue_weapon(PlayerbotAI* botAI) { return new CastFlametongueWeaponAction(botAI); }
        static Action* windfury_weapon(PlayerbotAI* botAI) { return new CastWindfuryWeaponAction(botAI); }
        static Action* earthliving_weapon(PlayerbotAI* botAI) { return new CastEarthlivingWeaponAction(botAI); }

        static Action* flame_shock(PlayerbotAI* botAI) { return new CastFlameShockAction(botAI); }
        static Action* earth_shock(PlayerbotAI* botAI) { return new CastEarthShockAction(botAI); }
        static Action* frost_shock(PlayerbotAI* botAI) { return new CastFrostShockAction(botAI); }
        static Action* lightning_bolt(PlayerbotAI* botAI) { return new CastLightningBoltAction(botAI); }
        static Action* chain_lightning(PlayerbotAI* botAI) { return new CastChainLightningAction(botAI); }
        static Action* lava_burst(PlayerbotAI* botAI) { return new CastLavaBurstAction(botAI); }
        static Action* earthquake(PlayerbotAI* botAI) { return new CastEarthquakeAction(botAI); }
        static Action* unleash_elements(PlayerbotAI* botAI) { return new CastUnleashElementsAction(botAI); }
        static Action* wind_shear(PlayerbotAI* botAI) { return new CastWindShearAction(botAI); }
        static Action* wind_shear_on_enemy_healer(PlayerbotAI* botAI) { return new CastWindShearOnEnemyHealerAction(botAI); }

        static Action* stormstrike(PlayerbotAI* botAI) { return new CastStormstrikeAction(botAI); }
        static Action* primal_strike(PlayerbotAI* botAI) { return new CastPrimalStrikeAction(botAI); }
        static Action* lava_lash(PlayerbotAI* botAI) { return new CastLavaLashAction(botAI); }
        static Action* fire_nova(PlayerbotAI* botAI) { return new CastFireNovaAction(botAI); }
        static Action* feral_spirit(PlayerbotAI* botAI) { return new CastFeralSpiritAction(botAI); }

        static Action* ascendance(PlayerbotAI* botAI) { return new CastAscendanceAction(botAI); }
        static Action* elemental_mastery(PlayerbotAI* botAI) { return new CastElementalMasteryAction(botAI); }
        static Action* astral_shift(PlayerbotAI* botAI) { return new CastAstralShiftAction(botAI); }
        static Action* shamanistic_rage(PlayerbotAI* botAI) { return new CastShamanisticRageAction(botAI); }

        static Action* searing_totem(PlayerbotAI* botAI) { return new CastSearingTotemAction(botAI); }
        static Action* magma_totem(PlayerbotAI* botAI) { return new CastMagmaTotemAction(botAI); }
        static Action* fire_elemental_totem(PlayerbotAI* botAI) { return new CastFireElementalTotemAction(botAI); }
        static Action* healing_stream_totem(PlayerbotAI* botAI) { return new CastHealingStreamTotemAction(botAI); }
        static Action* healing_tide_totem(PlayerbotAI* botAI) { return new CastHealingTideTotemAction(botAI); }
        static Action* mana_tide_totem(PlayerbotAI* botAI) { return new CastManaTideTotemAction(botAI); }

        static Action* riptide_on_party(PlayerbotAI* botAI) { return new CastRiptideOnPartyAction(botAI); }
        static Action* healing_wave_on_party(PlayerbotAI* botAI) { return new CastHealingWaveOnPartyAction(botAI); }
        static Action* greater_healing_wave_on_party(PlayerbotAI* botAI) { return new CastGreaterHealingWaveOnPartyAction(botAI); }
        static Action* healing_surge_on_party(PlayerbotAI* botAI) { return new CastHealingSurgeOnPartyAction(botAI); }
        static Action* chain_heal_on_party(PlayerbotAI* botAI) { return new CastChainHealOnPartyAction(botAI); }
        static Action* healing_rain_on_party(PlayerbotAI* botAI) { return new CastHealingRainOnPartyAction(botAI); }
        static Action* healing_surge(PlayerbotAI* botAI) { return new CastHealingSurgeAction(botAI); }
        static Action* earth_shield_on_main_tank(PlayerbotAI* botAI) { return new CastEarthShieldOnMainTankAction(botAI); }

        static Action* cleanse_spirit_on_party(PlayerbotAI* botAI) { return new CastCleanseSpiritOnPartyAction(botAI); }
        static Action* purify_spirit_curse_on_party(PlayerbotAI* botAI) { return new CastPurifySpiritCurseOnPartyAction(botAI); }
        static Action* purify_spirit_magic_on_party(PlayerbotAI* botAI) { return new CastPurifySpiritMagicOnPartyAction(botAI); }
        static Action* ancestral_spirit(PlayerbotAI* botAI) { return new CastAncestralSpiritAction(botAI); }
    };
}

ShamanAiObjectContext::ShamanAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new ShamanStrategyFactoryInternal());
    strategyContexts.Add(new ShamanCombatStrategyFactoryInternal());
    actionContexts.Add(new ShamanActionFactoryInternal());
    triggerContexts.Add(new ShamanTriggerFactoryInternal());
}
