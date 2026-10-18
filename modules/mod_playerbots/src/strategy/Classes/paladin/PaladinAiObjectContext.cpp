/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PaladinAiObjectContext.h"

#include "GenericPaladinNonCombatStrategy.h"
#include "NamedObjectContext.h"
#include "PaladinActions.h"
#include "PaladinStrategies.h"
#include "PaladinTriggers.h"
#include "Playerbots.h"

namespace
{
    class PaladinStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        PaladinStrategyFactoryInternal()
        {
            creators["nc"] = &PaladinStrategyFactoryInternal::nc;
            creators["cure"] = &PaladinStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericPaladinNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new PaladinCureStrategy(botAI); }
    };

    // One specialization at a time
    class PaladinCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        PaladinCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["dps"] = &PaladinCombatStrategyFactoryInternal::retribution;
            creators["retribution"] = &PaladinCombatStrategyFactoryInternal::retribution;
            creators["tank"] = &PaladinCombatStrategyFactoryInternal::protection;
            creators["protection"] = &PaladinCombatStrategyFactoryInternal::protection;
            creators["heal"] = &PaladinCombatStrategyFactoryInternal::holy;
            creators["holy"] = &PaladinCombatStrategyFactoryInternal::holy;
        }

    private:
        static Strategy* retribution(PlayerbotAI* botAI) { return new RetributionPaladinStrategy(botAI); }
        static Strategy* protection(PlayerbotAI* botAI) { return new ProtectionPaladinStrategy(botAI); }
        static Strategy* holy(PlayerbotAI* botAI) { return new HolyPaladinStrategy(botAI); }
    };

    class PaladinTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        PaladinTriggerFactoryInternal()
        {
            creators["seal of truth"] = &PaladinTriggerFactoryInternal::seal_of_truth;
            creators["seal of insight"] = &PaladinTriggerFactoryInternal::seal_of_insight;
            creators["righteous fury"] = &PaladinTriggerFactoryInternal::righteous_fury;
            creators["inquisition"] = &PaladinTriggerFactoryInternal::inquisition;
            creators["sacred shield"] = &PaladinTriggerFactoryInternal::sacred_shield;
            creators["blessing of kings"] = &PaladinTriggerFactoryInternal::blessing_of_kings;
            creators["blessing of might"] = &PaladinTriggerFactoryInternal::blessing_of_might;
            creators["grand crusader"] = &PaladinTriggerFactoryInternal::grand_crusader;
            creators["divine purpose"] = &PaladinTriggerFactoryInternal::divine_purpose;
            creators["avenging wrath"] = &PaladinTriggerFactoryInternal::avenging_wrath;
            creators["guardian of ancient kings"] = &PaladinTriggerFactoryInternal::guardian_of_ancient_kings;
            creators["execution sentence"] = &PaladinTriggerFactoryInternal::execution_sentence;
            creators["holy prism"] = &PaladinTriggerFactoryInternal::holy_prism;
            creators["light's hammer"] = &PaladinTriggerFactoryInternal::lights_hammer;
            creators["beacon of light on main tank"] = &PaladinTriggerFactoryInternal::beacon_of_light_on_main_tank;
            creators["rebuke"] = &PaladinTriggerFactoryInternal::rebuke;
            creators["rebuke on enemy healer"] = &PaladinTriggerFactoryInternal::rebuke_on_enemy_healer;
            creators["hammer of justice on enemy healer"] = &PaladinTriggerFactoryInternal::hammer_of_justice_on_enemy_healer;
            creators["cleanse poison on party"] = &PaladinTriggerFactoryInternal::cleanse_poison_on_party;
            creators["cleanse disease on party"] = &PaladinTriggerFactoryInternal::cleanse_disease_on_party;
            creators["cleanse magic on party"] = &PaladinTriggerFactoryInternal::cleanse_magic_on_party;
        }

    private:
        static Trigger* seal_of_truth(PlayerbotAI* botAI) { return new SealOfTruthTrigger(botAI); }
        static Trigger* seal_of_insight(PlayerbotAI* botAI) { return new SealOfInsightTrigger(botAI); }
        static Trigger* righteous_fury(PlayerbotAI* botAI) { return new RighteousFuryTrigger(botAI); }
        static Trigger* inquisition(PlayerbotAI* botAI) { return new InquisitionTrigger(botAI); }
        static Trigger* sacred_shield(PlayerbotAI* botAI) { return new SacredShieldTrigger(botAI); }
        static Trigger* blessing_of_kings(PlayerbotAI* botAI) { return new BlessingOfKingsTrigger(botAI); }
        static Trigger* blessing_of_might(PlayerbotAI* botAI) { return new BlessingOfMightTrigger(botAI); }
        static Trigger* grand_crusader(PlayerbotAI* botAI) { return new GrandCrusaderTrigger(botAI); }
        static Trigger* divine_purpose(PlayerbotAI* botAI) { return new DivinePurposeTrigger(botAI); }
        static Trigger* avenging_wrath(PlayerbotAI* botAI) { return new AvengingWrathTrigger(botAI); }
        static Trigger* guardian_of_ancient_kings(PlayerbotAI* botAI) { return new GuardianOfAncientKingsTrigger(botAI); }
        static Trigger* execution_sentence(PlayerbotAI* botAI) { return new ExecutionSentenceTrigger(botAI); }
        static Trigger* holy_prism(PlayerbotAI* botAI) { return new HolyPrismTrigger(botAI); }
        static Trigger* lights_hammer(PlayerbotAI* botAI) { return new LightsHammerTrigger(botAI); }
        static Trigger* beacon_of_light_on_main_tank(PlayerbotAI* botAI) { return new BeaconOfLightOnMainTankTrigger(botAI); }
        static Trigger* rebuke(PlayerbotAI* botAI) { return new RebukeInterruptSpellTrigger(botAI); }
        static Trigger* rebuke_on_enemy_healer(PlayerbotAI* botAI) { return new RebukeInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* hammer_of_justice_on_enemy_healer(PlayerbotAI* botAI) { return new HammerOfJusticeEnemyHealerTrigger(botAI); }
        static Trigger* cleanse_poison_on_party(PlayerbotAI* botAI) { return new CleansePoisonOnPartyTrigger(botAI); }
        static Trigger* cleanse_disease_on_party(PlayerbotAI* botAI) { return new CleanseDiseaseOnPartyTrigger(botAI); }
        static Trigger* cleanse_magic_on_party(PlayerbotAI* botAI) { return new CleanseMagicOnPartyTrigger(botAI); }
    };

    class PaladinActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        PaladinActionFactoryInternal()
        {
            creators["seal of truth"] = &PaladinActionFactoryInternal::seal_of_truth;
            creators["seal of insight"] = &PaladinActionFactoryInternal::seal_of_insight;
            creators["seal of righteousness"] = &PaladinActionFactoryInternal::seal_of_righteousness;
            creators["blessing of kings"] = &PaladinActionFactoryInternal::blessing_of_kings;
            creators["blessing of might"] = &PaladinActionFactoryInternal::blessing_of_might;
            creators["righteous fury"] = &PaladinActionFactoryInternal::righteous_fury;
            creators["inquisition"] = &PaladinActionFactoryInternal::inquisition;
            creators["sacred shield"] = &PaladinActionFactoryInternal::sacred_shield;

            creators["crusader strike"] = &PaladinActionFactoryInternal::crusader_strike;
            creators["hammer of the righteous"] = &PaladinActionFactoryInternal::hammer_of_the_righteous;
            creators["judgment"] = &PaladinActionFactoryInternal::judgment;
            creators["exorcism"] = &PaladinActionFactoryInternal::exorcism;
            creators["hammer of wrath"] = &PaladinActionFactoryInternal::hammer_of_wrath;
            creators["templar's verdict"] = &PaladinActionFactoryInternal::templars_verdict;
            creators["divine storm"] = &PaladinActionFactoryInternal::divine_storm;
            creators["shield of the righteous"] = &PaladinActionFactoryInternal::shield_of_the_righteous;
            creators["avenger's shield"] = &PaladinActionFactoryInternal::avengers_shield;
            creators["consecration"] = &PaladinActionFactoryInternal::consecration;
            creators["holy wrath"] = &PaladinActionFactoryInternal::holy_wrath;

            creators["execution sentence"] = &PaladinActionFactoryInternal::execution_sentence;
            creators["holy prism"] = &PaladinActionFactoryInternal::holy_prism;
            creators["light's hammer"] = &PaladinActionFactoryInternal::lights_hammer;
            creators["avenging wrath"] = &PaladinActionFactoryInternal::avenging_wrath;
            creators["guardian of ancient kings"] = &PaladinActionFactoryInternal::guardian_of_ancient_kings;
            creators["divine plea"] = &PaladinActionFactoryInternal::divine_plea;
            creators["divine favor"] = &PaladinActionFactoryInternal::divine_favor;

            creators["ardent defender"] = &PaladinActionFactoryInternal::ardent_defender;
            creators["divine protection"] = &PaladinActionFactoryInternal::divine_protection;
            creators["divine shield"] = &PaladinActionFactoryInternal::divine_shield;
            creators["reckoning"] = &PaladinActionFactoryInternal::reckoning;
            creators["rebuke"] = &PaladinActionFactoryInternal::rebuke;
            creators["rebuke on enemy healer"] = &PaladinActionFactoryInternal::rebuke_on_enemy_healer;
            creators["hammer of justice on enemy healer"] = &PaladinActionFactoryInternal::hammer_of_justice_on_enemy_healer;

            creators["word of glory"] = &PaladinActionFactoryInternal::word_of_glory;
            creators["eternal flame"] = &PaladinActionFactoryInternal::eternal_flame;
            creators["lay on hands"] = &PaladinActionFactoryInternal::lay_on_hands;
            creators["flash of light"] = &PaladinActionFactoryInternal::flash_of_light;
            creators["holy shock on party"] = &PaladinActionFactoryInternal::holy_shock_on_party;
            creators["flash of light on party"] = &PaladinActionFactoryInternal::flash_of_light_on_party;
            creators["holy light on party"] = &PaladinActionFactoryInternal::holy_light_on_party;
            creators["divine light on party"] = &PaladinActionFactoryInternal::divine_light_on_party;
            creators["word of glory on party"] = &PaladinActionFactoryInternal::word_of_glory_on_party;
            creators["eternal flame on party"] = &PaladinActionFactoryInternal::eternal_flame_on_party;
            creators["holy radiance on party"] = &PaladinActionFactoryInternal::holy_radiance_on_party;
            creators["lay on hands on party"] = &PaladinActionFactoryInternal::lay_on_hands_on_party;
            creators["light of dawn"] = &PaladinActionFactoryInternal::light_of_dawn;
            creators["beacon of light on main tank"] = &PaladinActionFactoryInternal::beacon_of_light_on_main_tank;

            creators["cleanse poison on party"] = &PaladinActionFactoryInternal::cleanse_poison_on_party;
            creators["cleanse disease on party"] = &PaladinActionFactoryInternal::cleanse_disease_on_party;
            creators["cleanse magic on party"] = &PaladinActionFactoryInternal::cleanse_magic_on_party;
            creators["redemption"] = &PaladinActionFactoryInternal::redemption;
        }

    private:
        static Action* seal_of_truth(PlayerbotAI* botAI) { return new CastSealOfTruthAction(botAI); }
        static Action* seal_of_insight(PlayerbotAI* botAI) { return new CastSealOfInsightAction(botAI); }
        static Action* seal_of_righteousness(PlayerbotAI* botAI) { return new CastSealOfRighteousnessAction(botAI); }
        static Action* blessing_of_kings(PlayerbotAI* botAI) { return new CastBlessingOfKingsAction(botAI); }
        static Action* blessing_of_might(PlayerbotAI* botAI) { return new CastBlessingOfMightAction(botAI); }
        static Action* righteous_fury(PlayerbotAI* botAI) { return new CastRighteousFuryAction(botAI); }
        static Action* inquisition(PlayerbotAI* botAI) { return new CastInquisitionAction(botAI); }
        static Action* sacred_shield(PlayerbotAI* botAI) { return new CastSacredShieldAction(botAI); }

        static Action* crusader_strike(PlayerbotAI* botAI) { return new CastCrusaderStrikeAction(botAI); }
        static Action* hammer_of_the_righteous(PlayerbotAI* botAI) { return new CastHammerOfTheRighteousAction(botAI); }
        static Action* judgment(PlayerbotAI* botAI) { return new CastJudgmentAction(botAI); }
        static Action* exorcism(PlayerbotAI* botAI) { return new CastExorcismAction(botAI); }
        static Action* hammer_of_wrath(PlayerbotAI* botAI) { return new CastHammerOfWrathAction(botAI); }
        static Action* templars_verdict(PlayerbotAI* botAI) { return new CastTemplarsVerdictAction(botAI); }
        static Action* divine_storm(PlayerbotAI* botAI) { return new CastDivineStormAction(botAI); }
        static Action* shield_of_the_righteous(PlayerbotAI* botAI) { return new CastShieldOfTheRighteousAction(botAI); }
        static Action* avengers_shield(PlayerbotAI* botAI) { return new CastAvengersShieldAction(botAI); }
        static Action* consecration(PlayerbotAI* botAI) { return new CastConsecrationAction(botAI); }
        static Action* holy_wrath(PlayerbotAI* botAI) { return new CastHolyWrathAction(botAI); }

        static Action* execution_sentence(PlayerbotAI* botAI) { return new CastExecutionSentenceAction(botAI); }
        static Action* holy_prism(PlayerbotAI* botAI) { return new CastHolyPrismAction(botAI); }
        static Action* lights_hammer(PlayerbotAI* botAI) { return new CastLightsHammerAction(botAI); }
        static Action* avenging_wrath(PlayerbotAI* botAI) { return new CastAvengingWrathAction(botAI); }
        static Action* guardian_of_ancient_kings(PlayerbotAI* botAI) { return new CastGuardianOfAncientKingsAction(botAI); }
        static Action* divine_plea(PlayerbotAI* botAI) { return new CastDivinePleaAction(botAI); }
        static Action* divine_favor(PlayerbotAI* botAI) { return new CastDivineFavorAction(botAI); }

        static Action* ardent_defender(PlayerbotAI* botAI) { return new CastArdentDefenderAction(botAI); }
        static Action* divine_protection(PlayerbotAI* botAI) { return new CastDivineProtectionAction(botAI); }
        static Action* divine_shield(PlayerbotAI* botAI) { return new CastDivineShieldAction(botAI); }
        static Action* reckoning(PlayerbotAI* botAI) { return new CastReckoningAction(botAI); }
        static Action* rebuke(PlayerbotAI* botAI) { return new CastRebukeAction(botAI); }
        static Action* rebuke_on_enemy_healer(PlayerbotAI* botAI) { return new CastRebukeOnEnemyHealerAction(botAI); }
        static Action* hammer_of_justice_on_enemy_healer(PlayerbotAI* botAI) { return new CastHammerOfJusticeOnEnemyHealerAction(botAI); }

        static Action* word_of_glory(PlayerbotAI* botAI) { return new CastWordOfGloryAction(botAI); }
        static Action* eternal_flame(PlayerbotAI* botAI) { return new CastEternalFlameAction(botAI); }
        static Action* lay_on_hands(PlayerbotAI* botAI) { return new CastLayOnHandsAction(botAI); }
        static Action* flash_of_light(PlayerbotAI* botAI) { return new CastFlashOfLightAction(botAI); }
        static Action* holy_shock_on_party(PlayerbotAI* botAI) { return new CastHolyShockOnPartyAction(botAI); }
        static Action* flash_of_light_on_party(PlayerbotAI* botAI) { return new CastFlashOfLightOnPartyAction(botAI); }
        static Action* holy_light_on_party(PlayerbotAI* botAI) { return new CastHolyLightOnPartyAction(botAI); }
        static Action* divine_light_on_party(PlayerbotAI* botAI) { return new CastDivineLightOnPartyAction(botAI); }
        static Action* word_of_glory_on_party(PlayerbotAI* botAI) { return new CastWordOfGloryOnPartyAction(botAI); }
        static Action* eternal_flame_on_party(PlayerbotAI* botAI) { return new CastEternalFlameOnPartyAction(botAI); }
        static Action* holy_radiance_on_party(PlayerbotAI* botAI) { return new CastHolyRadianceOnPartyAction(botAI); }
        static Action* lay_on_hands_on_party(PlayerbotAI* botAI) { return new CastLayOnHandsOnPartyAction(botAI); }
        static Action* light_of_dawn(PlayerbotAI* botAI) { return new CastLightOfDawnAction(botAI); }
        static Action* beacon_of_light_on_main_tank(PlayerbotAI* botAI) { return new CastBeaconOfLightOnMainTankAction(botAI); }

        static Action* cleanse_poison_on_party(PlayerbotAI* botAI) { return new CastCleansePoisonOnPartyAction(botAI); }
        static Action* cleanse_disease_on_party(PlayerbotAI* botAI) { return new CastCleanseDiseaseOnPartyAction(botAI); }
        static Action* cleanse_magic_on_party(PlayerbotAI* botAI) { return new CastCleanseMagicOnPartyAction(botAI); }
        static Action* redemption(PlayerbotAI* botAI) { return new CastRedemptionAction(botAI); }
    };
}

PaladinAiObjectContext::PaladinAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new PaladinStrategyFactoryInternal());
    strategyContexts.Add(new PaladinCombatStrategyFactoryInternal());
    actionContexts.Add(new PaladinActionFactoryInternal());
    triggerContexts.Add(new PaladinTriggerFactoryInternal());
}
