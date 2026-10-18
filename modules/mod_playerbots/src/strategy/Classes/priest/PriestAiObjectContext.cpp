/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PriestAiObjectContext.h"

#include "NamedObjectContext.h"
#include "Playerbots.h"
#include "PriestActions.h"
#include "PriestNonCombatStrategy.h"
#include "PriestStrategies.h"
#include "PriestTriggers.h"

namespace
{
    class PriestStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        PriestStrategyFactoryInternal()
        {
            creators["nc"] = &PriestStrategyFactoryInternal::nc;
            creators["cure"] = &PriestStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new PriestNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new PriestCureStrategy(botAI); }
    };

    // One specialization at a time
    class PriestCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        PriestCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["dps"] = &PriestCombatStrategyFactoryInternal::shadow;
            creators["shadow"] = &PriestCombatStrategyFactoryInternal::shadow;
            creators["heal"] = &PriestCombatStrategyFactoryInternal::discipline;
            creators["discipline"] = &PriestCombatStrategyFactoryInternal::discipline;
            creators["holy"] = &PriestCombatStrategyFactoryInternal::holy;
            creators["holy heal"] = &PriestCombatStrategyFactoryInternal::holy;
            creators["holy dps"] = &PriestCombatStrategyFactoryInternal::holy_dps;
        }

    private:
        static Strategy* shadow(PlayerbotAI* botAI) { return new ShadowPriestStrategy(botAI); }
        static Strategy* discipline(PlayerbotAI* botAI) { return new DisciplinePriestStrategy(botAI); }
        static Strategy* holy(PlayerbotAI* botAI) { return new HolyPriestStrategy(botAI); }
        static Strategy* holy_dps(PlayerbotAI* botAI) { return new HolyDpsPriestStrategy(botAI); }
    };

    class PriestTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        PriestTriggerFactoryInternal()
        {
            creators["power word: fortitude"] = &PriestTriggerFactoryInternal::power_word_fortitude;
            creators["inner fire"] = &PriestTriggerFactoryInternal::inner_fire;
            creators["shadowform"] = &PriestTriggerFactoryInternal::shadowform;
            creators["chakra: serenity"] = &PriestTriggerFactoryInternal::chakra_serenity;
            creators["shadow word: pain"] = &PriestTriggerFactoryInternal::shadow_word_pain;
            creators["vampiric touch"] = &PriestTriggerFactoryInternal::vampiric_touch;
            creators["surge of darkness"] = &PriestTriggerFactoryInternal::surge_of_darkness;
            creators["halo"] = &PriestTriggerFactoryInternal::halo;
            creators["shadowfiend"] = &PriestTriggerFactoryInternal::shadowfiend;
            creators["silence"] = &PriestTriggerFactoryInternal::silence;
            creators["silence on enemy healer"] = &PriestTriggerFactoryInternal::silence_on_enemy_healer;
            creators["purify magic on party"] = &PriestTriggerFactoryInternal::purify_magic_on_party;
            creators["purify disease on party"] = &PriestTriggerFactoryInternal::purify_disease_on_party;
        }

    private:
        static Trigger* power_word_fortitude(PlayerbotAI* botAI) { return new PowerWordFortitudeTrigger(botAI); }
        static Trigger* inner_fire(PlayerbotAI* botAI) { return new InnerFireTrigger(botAI); }
        static Trigger* shadowform(PlayerbotAI* botAI) { return new ShadowformTrigger(botAI); }
        static Trigger* chakra_serenity(PlayerbotAI* botAI) { return new ChakraSerenityTrigger(botAI); }
        static Trigger* shadow_word_pain(PlayerbotAI* botAI) { return new ShadowWordPainTrigger(botAI); }
        static Trigger* vampiric_touch(PlayerbotAI* botAI) { return new VampiricTouchTrigger(botAI); }
        static Trigger* surge_of_darkness(PlayerbotAI* botAI) { return new SurgeOfDarknessTrigger(botAI); }
        static Trigger* halo(PlayerbotAI* botAI) { return new HaloTrigger(botAI); }
        static Trigger* shadowfiend(PlayerbotAI* botAI) { return new ShadowfiendTrigger(botAI); }
        static Trigger* silence(PlayerbotAI* botAI) { return new SilenceInterruptSpellTrigger(botAI); }
        static Trigger* silence_on_enemy_healer(PlayerbotAI* botAI) { return new SilenceInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* purify_magic_on_party(PlayerbotAI* botAI) { return new PurifyMagicOnPartyTrigger(botAI); }
        static Trigger* purify_disease_on_party(PlayerbotAI* botAI) { return new PurifyDiseaseOnPartyTrigger(botAI); }
    };

    class PriestActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        PriestActionFactoryInternal()
        {
            creators["power word: fortitude"] = &PriestActionFactoryInternal::power_word_fortitude;
            creators["inner fire"] = &PriestActionFactoryInternal::inner_fire;
            creators["shadowform"] = &PriestActionFactoryInternal::shadowform;
            creators["chakra: serenity"] = &PriestActionFactoryInternal::chakra_serenity;
            creators["fade"] = &PriestActionFactoryInternal::fade;

            creators["shadow word: pain"] = &PriestActionFactoryInternal::shadow_word_pain;
            creators["vampiric touch"] = &PriestActionFactoryInternal::vampiric_touch;
            creators["devouring plague"] = &PriestActionFactoryInternal::devouring_plague;
            creators["mind blast"] = &PriestActionFactoryInternal::mind_blast;
            creators["shadow word: death"] = &PriestActionFactoryInternal::shadow_word_death;
            creators["mind flay"] = &PriestActionFactoryInternal::mind_flay;
            creators["mind spike"] = &PriestActionFactoryInternal::mind_spike;
            creators["mind sear"] = &PriestActionFactoryInternal::mind_sear;
            creators["shadowfiend"] = &PriestActionFactoryInternal::shadowfiend;
            creators["mindbender"] = &PriestActionFactoryInternal::mindbender;
            creators["dispersion"] = &PriestActionFactoryInternal::dispersion;
            creators["silence"] = &PriestActionFactoryInternal::silence;
            creators["silence on enemy healer"] = &PriestActionFactoryInternal::silence_on_enemy_healer;

            creators["smite"] = &PriestActionFactoryInternal::smite;
            creators["holy fire"] = &PriestActionFactoryInternal::holy_fire;
            creators["penance"] = &PriestActionFactoryInternal::penance;
            creators["halo"] = &PriestActionFactoryInternal::halo;
            creators["power word: shield"] = &PriestActionFactoryInternal::power_word_shield;
            creators["power word: shield on party"] = &PriestActionFactoryInternal::power_word_shield_on_party;

            creators["penance on party"] = &PriestActionFactoryInternal::penance_on_party;
            creators["flash heal on party"] = &PriestActionFactoryInternal::flash_heal_on_party;
            creators["greater heal on party"] = &PriestActionFactoryInternal::greater_heal_on_party;
            creators["heal on party"] = &PriestActionFactoryInternal::heal_on_party;
            creators["renew on party"] = &PriestActionFactoryInternal::renew_on_party;
            creators["prayer of mending on party"] = &PriestActionFactoryInternal::prayer_of_mending_on_party;
            creators["circle of healing on party"] = &PriestActionFactoryInternal::circle_of_healing_on_party;
            creators["prayer of healing on party"] = &PriestActionFactoryInternal::prayer_of_healing_on_party;
            creators["pain suppression on party"] = &PriestActionFactoryInternal::pain_suppression_on_party;
            creators["guardian spirit on party"] = &PriestActionFactoryInternal::guardian_spirit_on_party;
            creators["divine hymn"] = &PriestActionFactoryInternal::divine_hymn;
            creators["hymn of hope"] = &PriestActionFactoryInternal::hymn_of_hope;
            creators["flash heal"] = &PriestActionFactoryInternal::flash_heal;

            creators["purify magic on party"] = &PriestActionFactoryInternal::purify_magic_on_party;
            creators["purify disease on party"] = &PriestActionFactoryInternal::purify_disease_on_party;
            creators["resurrection"] = &PriestActionFactoryInternal::resurrection;
        }

    private:
        static Action* power_word_fortitude(PlayerbotAI* botAI) { return new CastPowerWordFortitudeAction(botAI); }
        static Action* inner_fire(PlayerbotAI* botAI) { return new CastInnerFireAction(botAI); }
        static Action* shadowform(PlayerbotAI* botAI) { return new CastShadowformAction(botAI); }
        static Action* chakra_serenity(PlayerbotAI* botAI) { return new CastChakraSerenityAction(botAI); }
        static Action* fade(PlayerbotAI* botAI) { return new CastFadeAction(botAI); }

        static Action* shadow_word_pain(PlayerbotAI* botAI) { return new CastShadowWordPainAction(botAI); }
        static Action* vampiric_touch(PlayerbotAI* botAI) { return new CastVampiricTouchAction(botAI); }
        static Action* devouring_plague(PlayerbotAI* botAI) { return new CastDevouringPlagueAction(botAI); }
        static Action* mind_blast(PlayerbotAI* botAI) { return new CastMindBlastAction(botAI); }
        static Action* shadow_word_death(PlayerbotAI* botAI) { return new CastShadowWordDeathAction(botAI); }
        static Action* mind_flay(PlayerbotAI* botAI) { return new CastMindFlayAction(botAI); }
        static Action* mind_spike(PlayerbotAI* botAI) { return new CastMindSpikeAction(botAI); }
        static Action* mind_sear(PlayerbotAI* botAI) { return new CastMindSearAction(botAI); }
        static Action* shadowfiend(PlayerbotAI* botAI) { return new CastShadowfiendAction(botAI); }
        static Action* mindbender(PlayerbotAI* botAI) { return new CastMindbenderAction(botAI); }
        static Action* dispersion(PlayerbotAI* botAI) { return new CastDispersionAction(botAI); }
        static Action* silence(PlayerbotAI* botAI) { return new CastSilenceAction(botAI); }
        static Action* silence_on_enemy_healer(PlayerbotAI* botAI) { return new CastSilenceOnEnemyHealerAction(botAI); }

        static Action* smite(PlayerbotAI* botAI) { return new CastSmiteAction(botAI); }
        static Action* holy_fire(PlayerbotAI* botAI) { return new CastHolyFireAction(botAI); }
        static Action* penance(PlayerbotAI* botAI) { return new CastPenanceAction(botAI); }
        static Action* halo(PlayerbotAI* botAI) { return new CastHaloAction(botAI); }
        static Action* power_word_shield(PlayerbotAI* botAI) { return new CastPowerWordShieldAction(botAI); }
        static Action* power_word_shield_on_party(PlayerbotAI* botAI) { return new CastPowerWordShieldOnPartyAction(botAI); }

        static Action* penance_on_party(PlayerbotAI* botAI) { return new CastPenanceOnPartyAction(botAI); }
        static Action* flash_heal_on_party(PlayerbotAI* botAI) { return new CastFlashHealOnPartyAction(botAI); }
        static Action* greater_heal_on_party(PlayerbotAI* botAI) { return new CastGreaterHealOnPartyAction(botAI); }
        static Action* heal_on_party(PlayerbotAI* botAI) { return new CastHealOnPartyAction(botAI); }
        static Action* renew_on_party(PlayerbotAI* botAI) { return new CastRenewOnPartyAction(botAI); }
        static Action* prayer_of_mending_on_party(PlayerbotAI* botAI) { return new CastPrayerOfMendingOnPartyAction(botAI); }
        static Action* circle_of_healing_on_party(PlayerbotAI* botAI) { return new CastCircleOfHealingOnPartyAction(botAI); }
        static Action* prayer_of_healing_on_party(PlayerbotAI* botAI) { return new CastPrayerOfHealingOnPartyAction(botAI); }
        static Action* pain_suppression_on_party(PlayerbotAI* botAI) { return new CastPainSuppressionOnPartyAction(botAI); }
        static Action* guardian_spirit_on_party(PlayerbotAI* botAI) { return new CastGuardianSpiritOnPartyAction(botAI); }
        static Action* divine_hymn(PlayerbotAI* botAI) { return new CastDivineHymnAction(botAI); }
        static Action* hymn_of_hope(PlayerbotAI* botAI) { return new CastHymnOfHopeAction(botAI); }
        static Action* flash_heal(PlayerbotAI* botAI) { return new CastFlashHealAction(botAI); }

        static Action* purify_magic_on_party(PlayerbotAI* botAI) { return new CastPurifyMagicOnPartyAction(botAI); }
        static Action* purify_disease_on_party(PlayerbotAI* botAI) { return new CastPurifyDiseaseOnPartyAction(botAI); }
        static Action* resurrection(PlayerbotAI* botAI) { return new CastResurrectionAction(botAI); }
    };
}

PriestAiObjectContext::PriestAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new PriestStrategyFactoryInternal());
    strategyContexts.Add(new PriestCombatStrategyFactoryInternal());
    actionContexts.Add(new PriestActionFactoryInternal());
    triggerContexts.Add(new PriestTriggerFactoryInternal());
}
