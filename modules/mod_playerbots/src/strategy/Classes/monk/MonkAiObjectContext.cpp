/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MonkAiObjectContext.h"

#include "MonkActions.h"
#include "MonkNonCombatStrategy.h"
#include "MonkStrategies.h"
#include "MonkTriggers.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"

namespace
{
    class MonkStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        MonkStrategyFactoryInternal()
        {
            creators["nc"] = &MonkStrategyFactoryInternal::nc;
            creators["cure"] = &MonkStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new MonkNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new MonkCureStrategy(botAI); }
    };

    // One specialization at a time
    class MonkCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        MonkCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["tank"] = &MonkCombatStrategyFactoryInternal::brewmaster;
            creators["brewmaster"] = &MonkCombatStrategyFactoryInternal::brewmaster;
            creators["melee"] = &MonkCombatStrategyFactoryInternal::windwalker;
            creators["dps"] = &MonkCombatStrategyFactoryInternal::windwalker;
            creators["windwalker"] = &MonkCombatStrategyFactoryInternal::windwalker;
            creators["heal"] = &MonkCombatStrategyFactoryInternal::mistweaver;
            creators["mistweaver"] = &MonkCombatStrategyFactoryInternal::mistweaver;
        }

    private:
        static Strategy* brewmaster(PlayerbotAI* botAI) { return new BrewmasterMonkStrategy(botAI); }
        static Strategy* windwalker(PlayerbotAI* botAI) { return new WindwalkerMonkStrategy(botAI); }
        static Strategy* mistweaver(PlayerbotAI* botAI) { return new MistweaverMonkStrategy(botAI); }
    };

    class MonkTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        MonkTriggerFactoryInternal()
        {
            creators["stance of the sturdy ox"] = &MonkTriggerFactoryInternal::stance_of_the_sturdy_ox;
            creators["stance of the fierce tiger"] = &MonkTriggerFactoryInternal::stance_of_the_fierce_tiger;
            creators["stance of the wise serpent"] = &MonkTriggerFactoryInternal::stance_of_the_wise_serpent;
            creators["tiger power"] = &MonkTriggerFactoryInternal::tiger_power;
            creators["shuffle"] = &MonkTriggerFactoryInternal::shuffle;
            creators["guard"] = &MonkTriggerFactoryInternal::guard;
            creators["legacy of the emperor"] = &MonkTriggerFactoryInternal::legacy_of_the_emperor;
            creators["combo breaker: tiger palm"] = &MonkTriggerFactoryInternal::combo_breaker_tiger_palm;
            creators["combo breaker: blackout kick"] = &MonkTriggerFactoryInternal::combo_breaker_blackout_kick;
            creators["rising sun kick"] = &MonkTriggerFactoryInternal::rising_sun_kick;
            creators["heavy stagger"] = &MonkTriggerFactoryInternal::heavy_stagger;
            creators["energizing brew"] = &MonkTriggerFactoryInternal::energizing_brew;
            creators["touch of death"] = &MonkTriggerFactoryInternal::touch_of_death;
            creators["keg smash"] = &MonkTriggerFactoryInternal::keg_smash;
            creators["chi wave"] = &MonkTriggerFactoryInternal::chi_wave;
            creators["invoke xuen"] = &MonkTriggerFactoryInternal::invoke_xuen;
            creators["spear hand strike"] = &MonkTriggerFactoryInternal::spear_hand_strike;
            creators["spear hand strike on enemy healer"] = &MonkTriggerFactoryInternal::spear_hand_strike_on_enemy_healer;
            creators["detox magic on party"] = &MonkTriggerFactoryInternal::detox_magic_on_party;
            creators["detox poison on party"] = &MonkTriggerFactoryInternal::detox_poison_on_party;
            creators["detox disease on party"] = &MonkTriggerFactoryInternal::detox_disease_on_party;
        }

    private:
        static Trigger* stance_of_the_sturdy_ox(PlayerbotAI* botAI) { return new StanceOfTheSturdyOxTrigger(botAI); }
        static Trigger* stance_of_the_fierce_tiger(PlayerbotAI* botAI) { return new StanceOfTheFierceTigerTrigger(botAI); }
        static Trigger* stance_of_the_wise_serpent(PlayerbotAI* botAI) { return new StanceOfTheWiseSerpentTrigger(botAI); }
        static Trigger* tiger_power(PlayerbotAI* botAI) { return new TigerPowerTrigger(botAI); }
        static Trigger* shuffle(PlayerbotAI* botAI) { return new ShuffleTrigger(botAI); }
        static Trigger* guard(PlayerbotAI* botAI) { return new GuardTrigger(botAI); }
        static Trigger* legacy_of_the_emperor(PlayerbotAI* botAI) { return new LegacyOfTheEmperorTrigger(botAI); }
        static Trigger* combo_breaker_tiger_palm(PlayerbotAI* botAI) { return new ComboBreakerTigerPalmTrigger(botAI); }
        static Trigger* combo_breaker_blackout_kick(PlayerbotAI* botAI) { return new ComboBreakerBlackoutKickTrigger(botAI); }
        static Trigger* rising_sun_kick(PlayerbotAI* botAI) { return new RisingSunKickTrigger(botAI); }
        static Trigger* heavy_stagger(PlayerbotAI* botAI) { return new HeavyStaggerTrigger(botAI); }
        static Trigger* energizing_brew(PlayerbotAI* botAI) { return new MonkLowEnergyTrigger(botAI); }
        static Trigger* touch_of_death(PlayerbotAI* botAI) { return new TouchOfDeathTrigger(botAI); }
        static Trigger* keg_smash(PlayerbotAI* botAI) { return new KegSmashTrigger(botAI); }
        static Trigger* chi_wave(PlayerbotAI* botAI) { return new ChiWaveTrigger(botAI); }
        static Trigger* invoke_xuen(PlayerbotAI* botAI) { return new InvokeXuenTrigger(botAI); }
        static Trigger* spear_hand_strike(PlayerbotAI* botAI) { return new SpearHandStrikeInterruptSpellTrigger(botAI); }
        static Trigger* spear_hand_strike_on_enemy_healer(PlayerbotAI* botAI) { return new SpearHandStrikeInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* detox_magic_on_party(PlayerbotAI* botAI) { return new DetoxMagicOnPartyTrigger(botAI); }
        static Trigger* detox_poison_on_party(PlayerbotAI* botAI) { return new DetoxPoisonOnPartyTrigger(botAI); }
        static Trigger* detox_disease_on_party(PlayerbotAI* botAI) { return new DetoxDiseaseOnPartyTrigger(botAI); }
    };

    class MonkActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        MonkActionFactoryInternal()
        {
            creators["stance of the sturdy ox"] = &MonkActionFactoryInternal::stance_of_the_sturdy_ox;
            creators["stance of the fierce tiger"] = &MonkActionFactoryInternal::stance_of_the_fierce_tiger;
            creators["stance of the wise serpent"] = &MonkActionFactoryInternal::stance_of_the_wise_serpent;
            creators["legacy of the emperor"] = &MonkActionFactoryInternal::legacy_of_the_emperor;

            creators["jab"] = &MonkActionFactoryInternal::jab;
            creators["expel harm"] = &MonkActionFactoryInternal::expel_harm;
            creators["tiger palm"] = &MonkActionFactoryInternal::tiger_palm;
            creators["blackout kick"] = &MonkActionFactoryInternal::blackout_kick;
            creators["rising sun kick"] = &MonkActionFactoryInternal::rising_sun_kick;
            creators["fists of fury"] = &MonkActionFactoryInternal::fists_of_fury;
            creators["touch of death"] = &MonkActionFactoryInternal::touch_of_death;
            creators["spinning crane kick"] = &MonkActionFactoryInternal::spinning_crane_kick;
            creators["chi wave"] = &MonkActionFactoryInternal::chi_wave;
            creators["spear hand strike"] = &MonkActionFactoryInternal::spear_hand_strike;
            creators["spear hand strike on enemy healer"] = &MonkActionFactoryInternal::spear_hand_strike_on_enemy_healer;

            creators["keg smash"] = &MonkActionFactoryInternal::keg_smash;
            creators["breath of fire"] = &MonkActionFactoryInternal::breath_of_fire;
            creators["purifying brew"] = &MonkActionFactoryInternal::purifying_brew;
            creators["guard"] = &MonkActionFactoryInternal::guard;
            creators["provoke"] = &MonkActionFactoryInternal::provoke;
            creators["energizing brew"] = &MonkActionFactoryInternal::energizing_brew;
            creators["invoke xuen, the white tiger"] = &MonkActionFactoryInternal::invoke_xuen;
            creators["fortifying brew"] = &MonkActionFactoryInternal::fortifying_brew;

            creators["renewing mist on party"] = &MonkActionFactoryInternal::renewing_mist_on_party;
            creators["enveloping mist on party"] = &MonkActionFactoryInternal::enveloping_mist_on_party;
            creators["surging mist on party"] = &MonkActionFactoryInternal::surging_mist_on_party;
            creators["life cocoon on party"] = &MonkActionFactoryInternal::life_cocoon_on_party;
            creators["uplift"] = &MonkActionFactoryInternal::uplift;
            creators["revival"] = &MonkActionFactoryInternal::revival;
            creators["surging mist"] = &MonkActionFactoryInternal::surging_mist;

            creators["detox magic on party"] = &MonkActionFactoryInternal::detox_magic_on_party;
            creators["detox poison on party"] = &MonkActionFactoryInternal::detox_poison_on_party;
            creators["detox disease on party"] = &MonkActionFactoryInternal::detox_disease_on_party;
            creators["resuscitate"] = &MonkActionFactoryInternal::resuscitate;
        }

    private:
        static Action* stance_of_the_sturdy_ox(PlayerbotAI* botAI) { return new CastStanceOfTheSturdyOxAction(botAI); }
        static Action* stance_of_the_fierce_tiger(PlayerbotAI* botAI) { return new CastStanceOfTheFierceTigerAction(botAI); }
        static Action* stance_of_the_wise_serpent(PlayerbotAI* botAI) { return new CastStanceOfTheWiseSerpentAction(botAI); }
        static Action* legacy_of_the_emperor(PlayerbotAI* botAI) { return new CastLegacyOfTheEmperorAction(botAI); }

        static Action* jab(PlayerbotAI* botAI) { return new CastJabAction(botAI); }
        static Action* expel_harm(PlayerbotAI* botAI) { return new CastExpelHarmAction(botAI); }
        static Action* tiger_palm(PlayerbotAI* botAI) { return new CastTigerPalmAction(botAI); }
        static Action* blackout_kick(PlayerbotAI* botAI) { return new CastBlackoutKickAction(botAI); }
        static Action* rising_sun_kick(PlayerbotAI* botAI) { return new CastRisingSunKickAction(botAI); }
        static Action* fists_of_fury(PlayerbotAI* botAI) { return new CastFistsOfFuryAction(botAI); }
        static Action* touch_of_death(PlayerbotAI* botAI) { return new CastTouchOfDeathAction(botAI); }
        static Action* spinning_crane_kick(PlayerbotAI* botAI) { return new CastSpinningCraneKickAction(botAI); }
        static Action* chi_wave(PlayerbotAI* botAI) { return new CastChiWaveAction(botAI); }
        static Action* spear_hand_strike(PlayerbotAI* botAI) { return new CastSpearHandStrikeAction(botAI); }
        static Action* spear_hand_strike_on_enemy_healer(PlayerbotAI* botAI) { return new CastSpearHandStrikeOnEnemyHealerAction(botAI); }

        static Action* keg_smash(PlayerbotAI* botAI) { return new CastKegSmashAction(botAI); }
        static Action* breath_of_fire(PlayerbotAI* botAI) { return new CastBreathOfFireAction(botAI); }
        static Action* purifying_brew(PlayerbotAI* botAI) { return new CastPurifyingBrewAction(botAI); }
        static Action* guard(PlayerbotAI* botAI) { return new CastGuardAction(botAI); }
        static Action* provoke(PlayerbotAI* botAI) { return new CastProvokeAction(botAI); }
        static Action* energizing_brew(PlayerbotAI* botAI) { return new CastEnergizingBrewAction(botAI); }
        static Action* invoke_xuen(PlayerbotAI* botAI) { return new CastInvokeXuenAction(botAI); }
        static Action* fortifying_brew(PlayerbotAI* botAI) { return new CastFortifyingBrewAction(botAI); }

        static Action* renewing_mist_on_party(PlayerbotAI* botAI) { return new CastRenewingMistOnPartyAction(botAI); }
        static Action* enveloping_mist_on_party(PlayerbotAI* botAI) { return new CastEnvelopingMistOnPartyAction(botAI); }
        static Action* surging_mist_on_party(PlayerbotAI* botAI) { return new CastSurgingMistOnPartyAction(botAI); }
        static Action* life_cocoon_on_party(PlayerbotAI* botAI) { return new CastLifeCocoonOnPartyAction(botAI); }
        static Action* uplift(PlayerbotAI* botAI) { return new CastUpliftAction(botAI); }
        static Action* revival(PlayerbotAI* botAI) { return new CastRevivalAction(botAI); }
        static Action* surging_mist(PlayerbotAI* botAI) { return new CastSurgingMistAction(botAI); }

        static Action* detox_magic_on_party(PlayerbotAI* botAI) { return new CastDetoxMagicOnPartyAction(botAI); }
        static Action* detox_poison_on_party(PlayerbotAI* botAI) { return new CastDetoxPoisonOnPartyAction(botAI); }
        static Action* detox_disease_on_party(PlayerbotAI* botAI) { return new CastDetoxDiseaseOnPartyAction(botAI); }
        static Action* resuscitate(PlayerbotAI* botAI) { return new CastResuscitateAction(botAI); }
    };
}

MonkAiObjectContext::MonkAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new MonkStrategyFactoryInternal());
    strategyContexts.Add(new MonkCombatStrategyFactoryInternal());
    actionContexts.Add(new MonkActionFactoryInternal());
    triggerContexts.Add(new MonkTriggerFactoryInternal());
}
