/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DKAiObjectContext.h"

#include "DKActions.h"
#include "DKStrategies.h"
#include "DKTriggers.h"
#include "GenericDKNonCombatStrategy.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"

namespace
{
    class DKStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        DKStrategyFactoryInternal()
        {
            creators["nc"] = &DKStrategyFactoryInternal::nc;
            creators["aoe"] = &DKStrategyFactoryInternal::aoe;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericDKNonCombatStrategy(botAI); }
        static Strategy* aoe(PlayerbotAI* botAI) { return new DKAoeStrategy(botAI); }
    };

    // One specialization at a time
    class DKCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        DKCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["blood"] = &DKCombatStrategyFactoryInternal::blood;
            creators["tank"] = &DKCombatStrategyFactoryInternal::blood;
            creators["frost"] = &DKCombatStrategyFactoryInternal::frost;
            creators["unholy"] = &DKCombatStrategyFactoryInternal::unholy;
        }

    private:
        static Strategy* blood(PlayerbotAI* botAI) { return new BloodDKStrategy(botAI); }
        static Strategy* frost(PlayerbotAI* botAI) { return new FrostDKStrategy(botAI); }
        static Strategy* unholy(PlayerbotAI* botAI) { return new UnholyDKStrategy(botAI); }
    };

    class DKTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        DKTriggerFactoryInternal()
        {
            creators["blood presence"] = &DKTriggerFactoryInternal::blood_presence;
            creators["frost presence"] = &DKTriggerFactoryInternal::frost_presence;
            creators["unholy presence"] = &DKTriggerFactoryInternal::unholy_presence;
            creators["horn of winter"] = &DKTriggerFactoryInternal::horn_of_winter;
            creators["bone shield"] = &DKTriggerFactoryInternal::bone_shield;
            creators["frost fever"] = &DKTriggerFactoryInternal::frost_fever;
            creators["blood plague"] = &DKTriggerFactoryInternal::blood_plague;
            creators["killing machine"] = &DKTriggerFactoryInternal::killing_machine;
            creators["freezing fog"] = &DKTriggerFactoryInternal::freezing_fog;
            creators["sudden doom"] = &DKTriggerFactoryInternal::sudden_doom;
            creators["crimson scourge"] = &DKTriggerFactoryInternal::crimson_scourge;
            creators["high runic power"] = &DKTriggerFactoryInternal::high_runic_power;
            creators["no runes"] = &DKTriggerFactoryInternal::no_runes;
            creators["shadow infusion"] = &DKTriggerFactoryInternal::shadow_infusion;
            creators["blood charge"] = &DKTriggerFactoryInternal::blood_charge;
            creators["soul reaper"] = &DKTriggerFactoryInternal::soul_reaper;
            creators["dk no ghoul"] = &DKTriggerFactoryInternal::no_ghoul;
            creators["pillar of frost"] = &DKTriggerFactoryInternal::pillar_of_frost;
            creators["summon gargoyle"] = &DKTriggerFactoryInternal::summon_gargoyle;
            creators["unholy frenzy"] = &DKTriggerFactoryInternal::unholy_frenzy;
            creators["dancing rune weapon"] = &DKTriggerFactoryInternal::dancing_rune_weapon;
            creators["raise dead"] = &DKTriggerFactoryInternal::raise_dead;
            creators["mind freeze"] = &DKTriggerFactoryInternal::mind_freeze;
            creators["mind freeze on enemy healer"] = &DKTriggerFactoryInternal::mind_freeze_on_enemy_healer;
        }

    private:
        static Trigger* blood_presence(PlayerbotAI* botAI) { return new BloodPresenceTrigger(botAI); }
        static Trigger* frost_presence(PlayerbotAI* botAI) { return new FrostPresenceTrigger(botAI); }
        static Trigger* unholy_presence(PlayerbotAI* botAI) { return new UnholyPresenceTrigger(botAI); }
        static Trigger* horn_of_winter(PlayerbotAI* botAI) { return new HornOfWinterTrigger(botAI); }
        static Trigger* bone_shield(PlayerbotAI* botAI) { return new BoneShieldTrigger(botAI); }
        static Trigger* frost_fever(PlayerbotAI* botAI) { return new FrostFeverTrigger(botAI); }
        static Trigger* blood_plague(PlayerbotAI* botAI) { return new BloodPlagueTrigger(botAI); }
        static Trigger* killing_machine(PlayerbotAI* botAI) { return new KillingMachineTrigger(botAI); }
        static Trigger* freezing_fog(PlayerbotAI* botAI) { return new FreezingFogTrigger(botAI); }
        static Trigger* sudden_doom(PlayerbotAI* botAI) { return new SuddenDoomTrigger(botAI); }
        static Trigger* crimson_scourge(PlayerbotAI* botAI) { return new CrimsonScourgeTrigger(botAI); }
        static Trigger* high_runic_power(PlayerbotAI* botAI) { return new HighRunicPowerTrigger(botAI); }
        static Trigger* no_runes(PlayerbotAI* botAI) { return new NoRunesTrigger(botAI); }
        static Trigger* shadow_infusion(PlayerbotAI* botAI) { return new ShadowInfusionTrigger(botAI); }
        static Trigger* blood_charge(PlayerbotAI* botAI) { return new BloodChargeTrigger(botAI); }
        static Trigger* soul_reaper(PlayerbotAI* botAI) { return new SoulReaperTrigger(botAI); }
        static Trigger* no_ghoul(PlayerbotAI* botAI) { return new DKNoGhoulTrigger(botAI); }
        static Trigger* pillar_of_frost(PlayerbotAI* botAI) { return new PillarOfFrostTrigger(botAI); }
        static Trigger* summon_gargoyle(PlayerbotAI* botAI) { return new SummonGargoyleTrigger(botAI); }
        static Trigger* unholy_frenzy(PlayerbotAI* botAI) { return new UnholyFrenzyTrigger(botAI); }
        static Trigger* dancing_rune_weapon(PlayerbotAI* botAI) { return new DancingRuneWeaponTrigger(botAI); }
        static Trigger* raise_dead(PlayerbotAI* botAI) { return new RaiseDeadTrigger(botAI); }
        static Trigger* mind_freeze(PlayerbotAI* botAI) { return new MindFreezeInterruptSpellTrigger(botAI); }
        static Trigger* mind_freeze_on_enemy_healer(PlayerbotAI* botAI) { return new MindFreezeInterruptEnemyHealerSpellTrigger(botAI); }
    };

    class DKActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        DKActionFactoryInternal()
        {
            creators["blood presence"] = &DKActionFactoryInternal::blood_presence;
            creators["frost presence"] = &DKActionFactoryInternal::frost_presence;
            creators["unholy presence"] = &DKActionFactoryInternal::unholy_presence;
            creators["horn of winter"] = &DKActionFactoryInternal::horn_of_winter;
            creators["bone shield"] = &DKActionFactoryInternal::bone_shield;

            creators["outbreak"] = &DKActionFactoryInternal::outbreak;
            creators["icy touch"] = &DKActionFactoryInternal::icy_touch;
            creators["plague strike"] = &DKActionFactoryInternal::plague_strike;
            creators["plague leech"] = &DKActionFactoryInternal::plague_leech;

            creators["obliterate"] = &DKActionFactoryInternal::obliterate;
            creators["frost strike"] = &DKActionFactoryInternal::frost_strike;
            creators["howling blast"] = &DKActionFactoryInternal::howling_blast;
            creators["pillar of frost"] = &DKActionFactoryInternal::pillar_of_frost;

            creators["scourge strike"] = &DKActionFactoryInternal::scourge_strike;
            creators["festering strike"] = &DKActionFactoryInternal::festering_strike;
            creators["death coil"] = &DKActionFactoryInternal::death_coil;
            creators["summon gargoyle"] = &DKActionFactoryInternal::summon_gargoyle;
            creators["unholy frenzy"] = &DKActionFactoryInternal::unholy_frenzy;
            creators["dark transformation"] = &DKActionFactoryInternal::dark_transformation;

            creators["heart strike"] = &DKActionFactoryInternal::heart_strike;
            creators["death strike"] = &DKActionFactoryInternal::death_strike;
            creators["rune strike"] = &DKActionFactoryInternal::rune_strike;
            creators["blood boil"] = &DKActionFactoryInternal::blood_boil;
            creators["vampiric blood"] = &DKActionFactoryInternal::vampiric_blood;
            creators["dancing rune weapon"] = &DKActionFactoryInternal::dancing_rune_weapon;
            creators["rune tap"] = &DKActionFactoryInternal::rune_tap;
            creators["dark command"] = &DKActionFactoryInternal::dark_command;

            creators["soul reaper"] = &DKActionFactoryInternal::soul_reaper;
            creators["death and decay"] = &DKActionFactoryInternal::death_and_decay;
            creators["empower rune weapon"] = &DKActionFactoryInternal::empower_rune_weapon;
            creators["blood tap"] = &DKActionFactoryInternal::blood_tap;
            creators["mind freeze"] = &DKActionFactoryInternal::mind_freeze;
            creators["mind freeze on enemy healer"] = &DKActionFactoryInternal::mind_freeze_on_enemy_healer;
            creators["icebound fortitude"] = &DKActionFactoryInternal::icebound_fortitude;
            creators["anti-magic shell"] = &DKActionFactoryInternal::anti_magic_shell;
            creators["remorseless winter"] = &DKActionFactoryInternal::remorseless_winter;
            creators["raise dead"] = &DKActionFactoryInternal::raise_dead;
        }

    private:
        static Action* blood_presence(PlayerbotAI* botAI) { return new CastBloodPresenceAction(botAI); }
        static Action* frost_presence(PlayerbotAI* botAI) { return new CastFrostPresenceAction(botAI); }
        static Action* unholy_presence(PlayerbotAI* botAI) { return new CastUnholyPresenceAction(botAI); }
        static Action* horn_of_winter(PlayerbotAI* botAI) { return new CastHornOfWinterAction(botAI); }
        static Action* bone_shield(PlayerbotAI* botAI) { return new CastBoneShieldAction(botAI); }

        static Action* outbreak(PlayerbotAI* botAI) { return new CastOutbreakAction(botAI); }
        static Action* icy_touch(PlayerbotAI* botAI) { return new CastIcyTouchAction(botAI); }
        static Action* plague_strike(PlayerbotAI* botAI) { return new CastPlagueStrikeAction(botAI); }
        static Action* plague_leech(PlayerbotAI* botAI) { return new CastPlagueLeechAction(botAI); }

        static Action* obliterate(PlayerbotAI* botAI) { return new CastObliterateAction(botAI); }
        static Action* frost_strike(PlayerbotAI* botAI) { return new CastFrostStrikeAction(botAI); }
        static Action* howling_blast(PlayerbotAI* botAI) { return new CastHowlingBlastAction(botAI); }
        static Action* pillar_of_frost(PlayerbotAI* botAI) { return new CastPillarOfFrostAction(botAI); }

        static Action* scourge_strike(PlayerbotAI* botAI) { return new CastScourgeStrikeAction(botAI); }
        static Action* festering_strike(PlayerbotAI* botAI) { return new CastFesteringStrikeAction(botAI); }
        static Action* death_coil(PlayerbotAI* botAI) { return new CastDeathCoilAction(botAI); }
        static Action* summon_gargoyle(PlayerbotAI* botAI) { return new CastSummonGargoyleAction(botAI); }
        static Action* unholy_frenzy(PlayerbotAI* botAI) { return new CastUnholyFrenzyAction(botAI); }
        static Action* dark_transformation(PlayerbotAI* botAI) { return new CastDarkTransformationAction(botAI); }

        static Action* heart_strike(PlayerbotAI* botAI) { return new CastHeartStrikeAction(botAI); }
        static Action* death_strike(PlayerbotAI* botAI) { return new CastDeathStrikeAction(botAI); }
        static Action* rune_strike(PlayerbotAI* botAI) { return new CastRuneStrikeAction(botAI); }
        static Action* blood_boil(PlayerbotAI* botAI) { return new CastBloodBoilAction(botAI); }
        static Action* vampiric_blood(PlayerbotAI* botAI) { return new CastVampiricBloodAction(botAI); }
        static Action* dancing_rune_weapon(PlayerbotAI* botAI) { return new CastDancingRuneWeaponAction(botAI); }
        static Action* rune_tap(PlayerbotAI* botAI) { return new CastRuneTapAction(botAI); }
        static Action* dark_command(PlayerbotAI* botAI) { return new CastDarkCommandAction(botAI); }

        static Action* soul_reaper(PlayerbotAI* botAI) { return new CastSoulReaperAction(botAI); }
        static Action* death_and_decay(PlayerbotAI* botAI) { return new CastDeathAndDecayAction(botAI); }
        static Action* empower_rune_weapon(PlayerbotAI* botAI) { return new CastEmpowerRuneWeaponAction(botAI); }
        static Action* blood_tap(PlayerbotAI* botAI) { return new CastBloodTapAction(botAI); }
        static Action* mind_freeze(PlayerbotAI* botAI) { return new CastMindFreezeAction(botAI); }
        static Action* mind_freeze_on_enemy_healer(PlayerbotAI* botAI) { return new CastMindFreezeOnEnemyHealerAction(botAI); }
        static Action* icebound_fortitude(PlayerbotAI* botAI) { return new CastIceboundFortitudeAction(botAI); }
        static Action* anti_magic_shell(PlayerbotAI* botAI) { return new CastAntiMagicShellAction(botAI); }
        static Action* remorseless_winter(PlayerbotAI* botAI) { return new CastRemorselessWinterAction(botAI); }
        static Action* raise_dead(PlayerbotAI* botAI) { return new CastRaiseDeadAction(botAI); }
    };
}

DKAiObjectContext::DKAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new DKStrategyFactoryInternal());
    strategyContexts.Add(new DKCombatStrategyFactoryInternal());
    actionContexts.Add(new DKActionFactoryInternal());
    triggerContexts.Add(new DKTriggerFactoryInternal());
}
