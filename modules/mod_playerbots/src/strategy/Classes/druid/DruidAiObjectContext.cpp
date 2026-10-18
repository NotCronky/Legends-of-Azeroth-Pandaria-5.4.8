/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DruidAiObjectContext.h"

#include "DruidActions.h"
#include "DruidStrategies.h"
#include "DruidTriggers.h"
#include "GenericDruidNonCombatStrategy.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"

namespace
{
    class DruidStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        DruidStrategyFactoryInternal()
        {
            creators["nc"] = &DruidStrategyFactoryInternal::nc;
            creators["cure"] = &DruidStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericDruidNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new DruidCureStrategy(botAI); }
    };

    // One specialization at a time
    class DruidCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        DruidCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["caster"] = &DruidCombatStrategyFactoryInternal::balance;
            creators["balance"] = &DruidCombatStrategyFactoryInternal::balance;
            creators["cat"] = &DruidCombatStrategyFactoryInternal::feral;
            creators["feral"] = &DruidCombatStrategyFactoryInternal::feral;
            creators["bear"] = &DruidCombatStrategyFactoryInternal::guardian;
            creators["tank"] = &DruidCombatStrategyFactoryInternal::guardian;
            creators["heal"] = &DruidCombatStrategyFactoryInternal::restoration;
        }

    private:
        static Strategy* balance(PlayerbotAI* botAI) { return new BalanceDruidStrategy(botAI); }
        static Strategy* feral(PlayerbotAI* botAI) { return new FeralDruidStrategy(botAI); }
        static Strategy* guardian(PlayerbotAI* botAI) { return new GuardianDruidStrategy(botAI); }
        static Strategy* restoration(PlayerbotAI* botAI) { return new RestorationDruidStrategy(botAI); }
    };

    class DruidTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        DruidTriggerFactoryInternal()
        {
            creators["cat form"] = &DruidTriggerFactoryInternal::cat_form;
            creators["bear form"] = &DruidTriggerFactoryInternal::bear_form;
            creators["moonkin form"] = &DruidTriggerFactoryInternal::moonkin_form;
            creators["in animal form"] = &DruidTriggerFactoryInternal::in_animal_form;
            creators["mark of the wild"] = &DruidTriggerFactoryInternal::mark_of_the_wild;
            creators["moonfire"] = &DruidTriggerFactoryInternal::moonfire;
            creators["sunfire"] = &DruidTriggerFactoryInternal::sunfire;
            creators["shooting stars"] = &DruidTriggerFactoryInternal::shooting_stars;
            creators["starfall"] = &DruidTriggerFactoryInternal::starfall;
            creators["celestial alignment"] = &DruidTriggerFactoryInternal::celestial_alignment;
            creators["starfire phase"] = &DruidTriggerFactoryInternal::starfire_phase;
            creators["druid finisher"] = &DruidTriggerFactoryInternal::druid_finisher;
            creators["rip"] = &DruidTriggerFactoryInternal::rip;
            creators["savage roar"] = &DruidTriggerFactoryInternal::savage_roar;
            creators["tiger's fury"] = &DruidTriggerFactoryInternal::tigers_fury;
            creators["rake"] = &DruidTriggerFactoryInternal::rake;
            creators["faerie fire"] = &DruidTriggerFactoryInternal::faerie_fire;
            creators["berserk"] = &DruidTriggerFactoryInternal::berserk;
            creators["skull bash"] = &DruidTriggerFactoryInternal::skull_bash;
            creators["skull bash on enemy healer"] = &DruidTriggerFactoryInternal::skull_bash_on_enemy_healer;
            creators["lacerate"] = &DruidTriggerFactoryInternal::lacerate;
            creators["thrash"] = &DruidTriggerFactoryInternal::thrash;
            creators["savage defense"] = &DruidTriggerFactoryInternal::savage_defense;
            creators["high rage"] = &DruidTriggerFactoryInternal::high_rage;
            creators["lifebloom on main tank"] = &DruidTriggerFactoryInternal::lifebloom_on_main_tank;
            creators["nature's cure magic on party"] = &DruidTriggerFactoryInternal::natures_cure_magic_on_party;
            creators["nature's cure curse on party"] = &DruidTriggerFactoryInternal::natures_cure_curse_on_party;
            creators["nature's cure poison on party"] = &DruidTriggerFactoryInternal::natures_cure_poison_on_party;
            creators["remove corruption curse on party"] = &DruidTriggerFactoryInternal::remove_corruption_curse_on_party;
            creators["remove corruption poison on party"] = &DruidTriggerFactoryInternal::remove_corruption_poison_on_party;
        }

    private:
        static Trigger* cat_form(PlayerbotAI* botAI) { return new CatFormTrigger(botAI); }
        static Trigger* bear_form(PlayerbotAI* botAI) { return new BearFormTrigger(botAI); }
        static Trigger* moonkin_form(PlayerbotAI* botAI) { return new MoonkinFormTrigger(botAI); }
        static Trigger* in_animal_form(PlayerbotAI* botAI) { return new InAnimalFormTrigger(botAI); }
        static Trigger* mark_of_the_wild(PlayerbotAI* botAI) { return new MarkOfTheWildTrigger(botAI); }
        static Trigger* moonfire(PlayerbotAI* botAI) { return new MoonfireTrigger(botAI); }
        static Trigger* sunfire(PlayerbotAI* botAI) { return new SunfireTrigger(botAI); }
        static Trigger* shooting_stars(PlayerbotAI* botAI) { return new ShootingStarsTrigger(botAI); }
        static Trigger* starfall(PlayerbotAI* botAI) { return new StarfallTrigger(botAI); }
        static Trigger* celestial_alignment(PlayerbotAI* botAI) { return new CelestialAlignmentTrigger(botAI); }
        static Trigger* starfire_phase(PlayerbotAI* botAI) { return new StarfirePhaseTrigger(botAI); }
        static Trigger* druid_finisher(PlayerbotAI* botAI) { return new DruidFinisherTrigger(botAI); }
        static Trigger* rip(PlayerbotAI* botAI) { return new RipTrigger(botAI); }
        static Trigger* savage_roar(PlayerbotAI* botAI) { return new SavageRoarTrigger(botAI); }
        static Trigger* tigers_fury(PlayerbotAI* botAI) { return new TigersFuryTrigger(botAI); }
        static Trigger* rake(PlayerbotAI* botAI) { return new RakeTrigger(botAI); }
        static Trigger* faerie_fire(PlayerbotAI* botAI) { return new FaerieFireTrigger(botAI); }
        static Trigger* berserk(PlayerbotAI* botAI) { return new BerserkTrigger(botAI); }
        static Trigger* skull_bash(PlayerbotAI* botAI) { return new SkullBashInterruptSpellTrigger(botAI); }
        static Trigger* skull_bash_on_enemy_healer(PlayerbotAI* botAI) { return new SkullBashInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* lacerate(PlayerbotAI* botAI) { return new LacerateTrigger(botAI); }
        static Trigger* thrash(PlayerbotAI* botAI) { return new ThrashTrigger(botAI); }
        static Trigger* savage_defense(PlayerbotAI* botAI) { return new DruidRageTrigger(botAI, "savage defense", 60); }
        static Trigger* high_rage(PlayerbotAI* botAI) { return new DruidRageTrigger(botAI, "high rage", 80); }
        static Trigger* lifebloom_on_main_tank(PlayerbotAI* botAI) { return new LifebloomOnMainTankTrigger(botAI); }
        static Trigger* natures_cure_magic_on_party(PlayerbotAI* botAI) { return new NaturesCureMagicOnPartyTrigger(botAI); }
        static Trigger* natures_cure_curse_on_party(PlayerbotAI* botAI) { return new NaturesCureCurseOnPartyTrigger(botAI); }
        static Trigger* natures_cure_poison_on_party(PlayerbotAI* botAI) { return new NaturesCurePoisonOnPartyTrigger(botAI); }
        static Trigger* remove_corruption_curse_on_party(PlayerbotAI* botAI) { return new RemoveCorruptionCurseOnPartyTrigger(botAI); }
        static Trigger* remove_corruption_poison_on_party(PlayerbotAI* botAI) { return new RemoveCorruptionPoisonOnPartyTrigger(botAI); }
    };

    class DruidActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        DruidActionFactoryInternal()
        {
            creators["cat form"] = &DruidActionFactoryInternal::cat_form;
            creators["bear form"] = &DruidActionFactoryInternal::bear_form;
            creators["moonkin form"] = &DruidActionFactoryInternal::moonkin_form;
            creators["caster form"] = &DruidActionFactoryInternal::caster_form;
            creators["mark of the wild"] = &DruidActionFactoryInternal::mark_of_the_wild;
            creators["barkskin"] = &DruidActionFactoryInternal::barkskin;
            creators["innervate"] = &DruidActionFactoryInternal::innervate;

            creators["moonfire"] = &DruidActionFactoryInternal::moonfire;
            creators["sunfire"] = &DruidActionFactoryInternal::sunfire;
            creators["wrath"] = &DruidActionFactoryInternal::wrath;
            creators["starfire"] = &DruidActionFactoryInternal::starfire;
            creators["starsurge"] = &DruidActionFactoryInternal::starsurge;
            creators["starfall"] = &DruidActionFactoryInternal::starfall;
            creators["celestial alignment"] = &DruidActionFactoryInternal::celestial_alignment;
            creators["hurricane"] = &DruidActionFactoryInternal::hurricane;

            creators["shred"] = &DruidActionFactoryInternal::shred;
            creators["mangle"] = &DruidActionFactoryInternal::mangle;
            creators["rake"] = &DruidActionFactoryInternal::rake;
            creators["rip"] = &DruidActionFactoryInternal::rip;
            creators["ferocious bite"] = &DruidActionFactoryInternal::ferocious_bite;
            creators["savage roar"] = &DruidActionFactoryInternal::savage_roar;
            creators["tiger's fury"] = &DruidActionFactoryInternal::tigers_fury;
            creators["berserk"] = &DruidActionFactoryInternal::berserk;
            creators["thrash"] = &DruidActionFactoryInternal::thrash;
            creators["swipe"] = &DruidActionFactoryInternal::swipe;
            creators["faerie fire"] = &DruidActionFactoryInternal::faerie_fire;
            creators["skull bash"] = &DruidActionFactoryInternal::skull_bash;
            creators["skull bash on enemy healer"] = &DruidActionFactoryInternal::skull_bash_on_enemy_healer;
            creators["survival instincts"] = &DruidActionFactoryInternal::survival_instincts;

            creators["maul"] = &DruidActionFactoryInternal::maul;
            creators["lacerate"] = &DruidActionFactoryInternal::lacerate;
            creators["savage defense"] = &DruidActionFactoryInternal::savage_defense;
            creators["frenzied regeneration"] = &DruidActionFactoryInternal::frenzied_regeneration;
            creators["might of ursoc"] = &DruidActionFactoryInternal::might_of_ursoc;
            creators["growl"] = &DruidActionFactoryInternal::growl;

            creators["rejuvenation on party"] = &DruidActionFactoryInternal::rejuvenation_on_party;
            creators["regrowth on party"] = &DruidActionFactoryInternal::regrowth_on_party;
            creators["healing touch on party"] = &DruidActionFactoryInternal::healing_touch_on_party;
            creators["nourish on party"] = &DruidActionFactoryInternal::nourish_on_party;
            creators["swiftmend on party"] = &DruidActionFactoryInternal::swiftmend_on_party;
            creators["wild growth on party"] = &DruidActionFactoryInternal::wild_growth_on_party;
            creators["ironbark on party"] = &DruidActionFactoryInternal::ironbark_on_party;
            creators["tranquility"] = &DruidActionFactoryInternal::tranquility;
            creators["nature's swiftness"] = &DruidActionFactoryInternal::natures_swiftness;
            creators["rejuvenation"] = &DruidActionFactoryInternal::rejuvenation;
            creators["healing touch"] = &DruidActionFactoryInternal::healing_touch;
            creators["lifebloom on main tank"] = &DruidActionFactoryInternal::lifebloom_on_main_tank;

            creators["nature's cure magic on party"] = &DruidActionFactoryInternal::natures_cure_magic_on_party;
            creators["nature's cure curse on party"] = &DruidActionFactoryInternal::natures_cure_curse_on_party;
            creators["nature's cure poison on party"] = &DruidActionFactoryInternal::natures_cure_poison_on_party;
            creators["remove corruption curse on party"] = &DruidActionFactoryInternal::remove_corruption_curse_on_party;
            creators["remove corruption poison on party"] = &DruidActionFactoryInternal::remove_corruption_poison_on_party;
            creators["revive"] = &DruidActionFactoryInternal::revive;
        }

    private:
        static Action* cat_form(PlayerbotAI* botAI) { return new CastCatFormAction(botAI); }
        static Action* bear_form(PlayerbotAI* botAI) { return new CastBearFormAction(botAI); }
        static Action* moonkin_form(PlayerbotAI* botAI) { return new CastMoonkinFormAction(botAI); }
        static Action* caster_form(PlayerbotAI* botAI) { return new CastCasterFormAction(botAI); }
        static Action* mark_of_the_wild(PlayerbotAI* botAI) { return new CastMarkOfTheWildAction(botAI); }
        static Action* barkskin(PlayerbotAI* botAI) { return new CastBarkskinAction(botAI); }
        static Action* innervate(PlayerbotAI* botAI) { return new CastInnervateAction(botAI); }

        static Action* moonfire(PlayerbotAI* botAI) { return new CastMoonfireAction(botAI); }
        static Action* sunfire(PlayerbotAI* botAI) { return new CastSunfireAction(botAI); }
        static Action* wrath(PlayerbotAI* botAI) { return new CastWrathAction(botAI); }
        static Action* starfire(PlayerbotAI* botAI) { return new CastStarfireAction(botAI); }
        static Action* starsurge(PlayerbotAI* botAI) { return new CastStarsurgeAction(botAI); }
        static Action* starfall(PlayerbotAI* botAI) { return new CastStarfallAction(botAI); }
        static Action* celestial_alignment(PlayerbotAI* botAI) { return new CastCelestialAlignmentAction(botAI); }
        static Action* hurricane(PlayerbotAI* botAI) { return new CastHurricaneAction(botAI); }

        static Action* shred(PlayerbotAI* botAI) { return new CastShredAction(botAI); }
        static Action* mangle(PlayerbotAI* botAI) { return new CastMangleAction(botAI); }
        static Action* rake(PlayerbotAI* botAI) { return new CastRakeAction(botAI); }
        static Action* rip(PlayerbotAI* botAI) { return new CastRipAction(botAI); }
        static Action* ferocious_bite(PlayerbotAI* botAI) { return new CastFerociousBiteAction(botAI); }
        static Action* savage_roar(PlayerbotAI* botAI) { return new CastSavageRoarAction(botAI); }
        static Action* tigers_fury(PlayerbotAI* botAI) { return new CastTigersFuryAction(botAI); }
        static Action* berserk(PlayerbotAI* botAI) { return new CastBerserkAction(botAI); }
        static Action* thrash(PlayerbotAI* botAI) { return new CastThrashAction(botAI); }
        static Action* swipe(PlayerbotAI* botAI) { return new CastSwipeAction(botAI); }
        static Action* faerie_fire(PlayerbotAI* botAI) { return new CastFaerieFireAction(botAI); }
        static Action* skull_bash(PlayerbotAI* botAI) { return new CastSkullBashAction(botAI); }
        static Action* skull_bash_on_enemy_healer(PlayerbotAI* botAI) { return new CastSkullBashOnEnemyHealerAction(botAI); }
        static Action* survival_instincts(PlayerbotAI* botAI) { return new CastSurvivalInstinctsAction(botAI); }

        static Action* maul(PlayerbotAI* botAI) { return new CastMaulAction(botAI); }
        static Action* lacerate(PlayerbotAI* botAI) { return new CastLacerateAction(botAI); }
        static Action* savage_defense(PlayerbotAI* botAI) { return new CastSavageDefenseAction(botAI); }
        static Action* frenzied_regeneration(PlayerbotAI* botAI) { return new CastFrenziedRegenerationAction(botAI); }
        static Action* might_of_ursoc(PlayerbotAI* botAI) { return new CastMightOfUrsocAction(botAI); }
        static Action* growl(PlayerbotAI* botAI) { return new CastGrowlAction(botAI); }

        static Action* rejuvenation_on_party(PlayerbotAI* botAI) { return new CastRejuvenationOnPartyAction(botAI); }
        static Action* regrowth_on_party(PlayerbotAI* botAI) { return new CastRegrowthOnPartyAction(botAI); }
        static Action* healing_touch_on_party(PlayerbotAI* botAI) { return new CastHealingTouchOnPartyAction(botAI); }
        static Action* nourish_on_party(PlayerbotAI* botAI) { return new CastNourishOnPartyAction(botAI); }
        static Action* swiftmend_on_party(PlayerbotAI* botAI) { return new CastSwiftmendOnPartyAction(botAI); }
        static Action* wild_growth_on_party(PlayerbotAI* botAI) { return new CastWildGrowthOnPartyAction(botAI); }
        static Action* ironbark_on_party(PlayerbotAI* botAI) { return new CastIronbarkOnPartyAction(botAI); }
        static Action* tranquility(PlayerbotAI* botAI) { return new CastTranquilityAction(botAI); }
        static Action* natures_swiftness(PlayerbotAI* botAI) { return new CastNaturesSwiftnessAction(botAI); }
        static Action* rejuvenation(PlayerbotAI* botAI) { return new CastRejuvenationAction(botAI); }
        static Action* healing_touch(PlayerbotAI* botAI) { return new CastHealingTouchAction(botAI); }
        static Action* lifebloom_on_main_tank(PlayerbotAI* botAI) { return new CastLifebloomOnMainTankAction(botAI); }

        static Action* natures_cure_magic_on_party(PlayerbotAI* botAI) { return new CastNaturesCureMagicOnPartyAction(botAI); }
        static Action* natures_cure_curse_on_party(PlayerbotAI* botAI) { return new CastNaturesCureCurseOnPartyAction(botAI); }
        static Action* natures_cure_poison_on_party(PlayerbotAI* botAI) { return new CastNaturesCurePoisonOnPartyAction(botAI); }
        static Action* remove_corruption_curse_on_party(PlayerbotAI* botAI) { return new CastRemoveCorruptionCurseOnPartyAction(botAI); }
        static Action* remove_corruption_poison_on_party(PlayerbotAI* botAI) { return new CastRemoveCorruptionPoisonOnPartyAction(botAI); }
        static Action* revive(PlayerbotAI* botAI) { return new CastReviveAction(botAI); }
    };
}

DruidAiObjectContext::DruidAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new DruidStrategyFactoryInternal());
    strategyContexts.Add(new DruidCombatStrategyFactoryInternal());
    actionContexts.Add(new DruidActionFactoryInternal());
    triggerContexts.Add(new DruidTriggerFactoryInternal());
}
