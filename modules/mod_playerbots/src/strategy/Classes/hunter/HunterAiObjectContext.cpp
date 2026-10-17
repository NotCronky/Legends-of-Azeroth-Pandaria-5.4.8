/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "HunterAiObjectContext.h"

#include "GenericHunterNonCombatStrategy.h"
#include "HunterActions.h"
#include "HunterStrategies.h"
#include "HunterTriggers.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"

namespace
{
    class HunterStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        HunterStrategyFactoryInternal()
        {
            creators["nc"] = &HunterStrategyFactoryInternal::nc;
            creators["pet"] = &HunterStrategyFactoryInternal::pet;
            creators["aoe"] = &HunterStrategyFactoryInternal::aoe;
            creators["boost"] = &HunterStrategyFactoryInternal::boost;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericHunterNonCombatStrategy(botAI); }
        static Strategy* pet(PlayerbotAI* botAI) { return new HunterPetStrategy(botAI); }
        static Strategy* aoe(PlayerbotAI* botAI) { return new HunterAoeStrategy(botAI); }
        static Strategy* boost(PlayerbotAI* botAI) { return new HunterBoostStrategy(botAI); }
    };

    // One specialization at a time
    class HunterCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        HunterCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["bm"] = &HunterCombatStrategyFactoryInternal::bm;
            creators["mm"] = &HunterCombatStrategyFactoryInternal::mm;
            creators["sv"] = &HunterCombatStrategyFactoryInternal::sv;
        }

    private:
        static Strategy* bm(PlayerbotAI* botAI) { return new BeastMasteryHunterStrategy(botAI); }
        static Strategy* mm(PlayerbotAI* botAI) { return new MarksmanshipHunterStrategy(botAI); }
        static Strategy* sv(PlayerbotAI* botAI) { return new SurvivalHunterStrategy(botAI); }
    };

    class HunterTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        HunterTriggerFactoryInternal()
        {
            creators["high focus"] = &HunterTriggerFactoryInternal::high_focus;
            creators["auto shot dead target"] = &HunterTriggerFactoryInternal::auto_shot_dead_target;
            creators["no aspect"] = &HunterTriggerFactoryInternal::no_aspect;
            creators["hunter no pet"] = &HunterTriggerFactoryInternal::no_pet;
            creators["hunter pet dead"] = &HunterTriggerFactoryInternal::pet_dead;
            creators["hunter pet low health"] = &HunterTriggerFactoryInternal::pet_low_health;
            creators["misdirection on main tank"] = &HunterTriggerFactoryInternal::misdirection_on_main_tank;

            creators["serpent sting"] = &HunterTriggerFactoryInternal::serpent_sting;
            creators["black arrow"] = &HunterTriggerFactoryInternal::black_arrow;
            creators["hunter's mark"] = &HunterTriggerFactoryInternal::hunters_mark;
            creators["counter shot"] = &HunterTriggerFactoryInternal::counter_shot;

            creators["lock and load"] = &HunterTriggerFactoryInternal::lock_and_load;
            creators["fire!"] = &HunterTriggerFactoryInternal::fire;
            creators["thrill of the hunt"] = &HunterTriggerFactoryInternal::thrill_of_the_hunt;
            creators["frenzy five stacks"] = &HunterTriggerFactoryInternal::frenzy;

            // Off cooldown, in range and affordable
            creators["kill command"] = &HunterTriggerFactoryInternal::kill_command;
            creators["dire beast"] = &HunterTriggerFactoryInternal::dire_beast;
            creators["chimera shot"] = &HunterTriggerFactoryInternal::chimera_shot;
            creators["explosive shot"] = &HunterTriggerFactoryInternal::explosive_shot;
            creators["rapid fire"] = &HunterTriggerFactoryInternal::rapid_fire;
            creators["a murder of crows"] = &HunterTriggerFactoryInternal::murder_of_crows;
            creators["lynx rush"] = &HunterTriggerFactoryInternal::lynx_rush;
            creators["glaive toss"] = &HunterTriggerFactoryInternal::glaive_toss;
            creators["powershot"] = &HunterTriggerFactoryInternal::powershot;
            creators["stampede"] = &HunterTriggerFactoryInternal::stampede;
        }

    private:
        static Trigger* high_focus(PlayerbotAI* botAI) { return new HighFocusTrigger(botAI); }
        static Trigger* auto_shot_dead_target(PlayerbotAI* botAI) { return new AutoShotDeadTargetTrigger(botAI); }
        static Trigger* no_aspect(PlayerbotAI* botAI) { return new HunterNoAspectTrigger(botAI); }
        static Trigger* no_pet(PlayerbotAI* botAI) { return new HunterNoPetTrigger(botAI); }
        static Trigger* pet_dead(PlayerbotAI* botAI) { return new HuntersPetDeadTrigger(botAI); }
        static Trigger* pet_low_health(PlayerbotAI* botAI) { return new HuntersPetLowHealthTrigger(botAI); }
        static Trigger* misdirection_on_main_tank(PlayerbotAI* botAI) { return new MisdirectionOnMainTankTrigger(botAI); }

        static Trigger* serpent_sting(PlayerbotAI* botAI) { return new SerpentStingTrigger(botAI); }
        static Trigger* black_arrow(PlayerbotAI* botAI) { return new BlackArrowTrigger(botAI); }
        static Trigger* hunters_mark(PlayerbotAI* botAI) { return new HuntersMarkTrigger(botAI); }
        static Trigger* counter_shot(PlayerbotAI* botAI) { return new CounterShotTrigger(botAI); }

        static Trigger* lock_and_load(PlayerbotAI* botAI) { return new LockAndLoadTrigger(botAI); }
        static Trigger* fire(PlayerbotAI* botAI) { return new FireProcTrigger(botAI); }
        static Trigger* thrill_of_the_hunt(PlayerbotAI* botAI) { return new ThrillOfTheHuntTrigger(botAI); }
        static Trigger* frenzy(PlayerbotAI* botAI) { return new FrenzyFiveStacksTrigger(botAI); }

        static Trigger* kill_command(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "kill command"); }
        static Trigger* dire_beast(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "dire beast"); }
        static Trigger* chimera_shot(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "chimera shot"); }
        static Trigger* explosive_shot(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "explosive shot"); }
        static Trigger* rapid_fire(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "rapid fire"); }
        static Trigger* murder_of_crows(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "a murder of crows"); }
        static Trigger* lynx_rush(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "lynx rush"); }
        static Trigger* glaive_toss(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "glaive toss"); }
        static Trigger* powershot(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "powershot"); }
        static Trigger* stampede(PlayerbotAI* botAI) { return new SpellCanBeCastTrigger(botAI, "stampede"); }
    };

    class HunterActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        HunterActionFactoryInternal()
        {
            creators["auto shot"] = &HunterActionFactoryInternal::auto_shot;
            creators["stop auto shot"] = &HunterActionFactoryInternal::stop_auto_shot;
            creators["arcane shot"] = &HunterActionFactoryInternal::arcane_shot;
            creators["steady shot"] = &HunterActionFactoryInternal::steady_shot;
            creators["cobra shot"] = &HunterActionFactoryInternal::cobra_shot;
            creators["kill shot"] = &HunterActionFactoryInternal::kill_shot;
            creators["multi-shot"] = &HunterActionFactoryInternal::multi_shot;
            creators["chimera shot"] = &HunterActionFactoryInternal::chimera_shot;
            creators["aimed shot"] = &HunterActionFactoryInternal::aimed_shot;
            creators["explosive shot"] = &HunterActionFactoryInternal::explosive_shot;
            creators["black arrow"] = &HunterActionFactoryInternal::black_arrow;
            creators["serpent sting"] = &HunterActionFactoryInternal::serpent_sting;
            creators["hunter's mark"] = &HunterActionFactoryInternal::hunters_mark;
            creators["concussive shot"] = &HunterActionFactoryInternal::concussive_shot;
            creators["counter shot"] = &HunterActionFactoryInternal::counter_shot;
            creators["tranquilizing shot"] = &HunterActionFactoryInternal::tranquilizing_shot;

            creators["glaive toss"] = &HunterActionFactoryInternal::glaive_toss;
            creators["powershot"] = &HunterActionFactoryInternal::powershot;
            creators["barrage"] = &HunterActionFactoryInternal::barrage;
            creators["a murder of crows"] = &HunterActionFactoryInternal::murder_of_crows;
            creators["lynx rush"] = &HunterActionFactoryInternal::lynx_rush;
            creators["dire beast"] = &HunterActionFactoryInternal::dire_beast;
            creators["fervor"] = &HunterActionFactoryInternal::fervor;

            creators["rapid fire"] = &HunterActionFactoryInternal::rapid_fire;
            creators["focus fire"] = &HunterActionFactoryInternal::focus_fire;
            creators["bestial wrath"] = &HunterActionFactoryInternal::bestial_wrath;
            creators["kill command"] = &HunterActionFactoryInternal::kill_command;
            creators["stampede"] = &HunterActionFactoryInternal::stampede;
            creators["deterrence"] = &HunterActionFactoryInternal::deterrence;
            creators["feign death"] = &HunterActionFactoryInternal::feign_death;

            creators["aspect of the hawk"] = &HunterActionFactoryInternal::aspect_of_the_hawk;
            creators["aspect of the iron hawk"] = &HunterActionFactoryInternal::aspect_of_the_iron_hawk;
            creators["call pet 1"] = &HunterActionFactoryInternal::call_pet;
            creators["revive pet"] = &HunterActionFactoryInternal::revive_pet;
            creators["mend pet"] = &HunterActionFactoryInternal::mend_pet;
            creators["misdirection on main tank"] = &HunterActionFactoryInternal::misdirection_on_main_tank;
        }

    private:
        static Action* auto_shot(PlayerbotAI* botAI) { return new CastAutoShotAction(botAI); }
        static Action* stop_auto_shot(PlayerbotAI* botAI) { return new StopAutoShotAction(botAI); }
        static Action* arcane_shot(PlayerbotAI* botAI) { return new CastArcaneShotAction(botAI); }
        static Action* steady_shot(PlayerbotAI* botAI) { return new CastSteadyShotAction(botAI); }
        static Action* cobra_shot(PlayerbotAI* botAI) { return new CastCobraShotAction(botAI); }
        static Action* kill_shot(PlayerbotAI* botAI) { return new CastKillShotAction(botAI); }
        static Action* multi_shot(PlayerbotAI* botAI) { return new CastMultiShotAction(botAI); }
        static Action* chimera_shot(PlayerbotAI* botAI) { return new CastChimeraShotAction(botAI); }
        static Action* aimed_shot(PlayerbotAI* botAI) { return new CastAimedShotAction(botAI); }
        static Action* explosive_shot(PlayerbotAI* botAI) { return new CastExplosiveShotAction(botAI); }
        static Action* black_arrow(PlayerbotAI* botAI) { return new CastBlackArrowAction(botAI); }
        static Action* serpent_sting(PlayerbotAI* botAI) { return new CastSerpentStingAction(botAI); }
        static Action* hunters_mark(PlayerbotAI* botAI) { return new CastHuntersMarkAction(botAI); }
        static Action* concussive_shot(PlayerbotAI* botAI) { return new CastConcussiveShotAction(botAI); }
        static Action* counter_shot(PlayerbotAI* botAI) { return new CastCounterShotAction(botAI); }
        static Action* tranquilizing_shot(PlayerbotAI* botAI) { return new CastTranquilizingShotAction(botAI); }

        static Action* glaive_toss(PlayerbotAI* botAI) { return new CastGlaiveTossAction(botAI); }
        static Action* powershot(PlayerbotAI* botAI) { return new CastPowershotAction(botAI); }
        static Action* barrage(PlayerbotAI* botAI) { return new CastBarrageAction(botAI); }
        static Action* murder_of_crows(PlayerbotAI* botAI) { return new CastMurderOfCrowsAction(botAI); }
        static Action* lynx_rush(PlayerbotAI* botAI) { return new CastLynxRushAction(botAI); }
        static Action* dire_beast(PlayerbotAI* botAI) { return new CastDireBeastAction(botAI); }
        static Action* fervor(PlayerbotAI* botAI) { return new CastFervorAction(botAI); }

        static Action* rapid_fire(PlayerbotAI* botAI) { return new CastRapidFireAction(botAI); }
        static Action* focus_fire(PlayerbotAI* botAI) { return new CastFocusFireAction(botAI); }
        static Action* bestial_wrath(PlayerbotAI* botAI) { return new CastBestialWrathAction(botAI); }
        static Action* kill_command(PlayerbotAI* botAI) { return new CastKillCommandAction(botAI); }
        static Action* stampede(PlayerbotAI* botAI) { return new CastStampedeAction(botAI); }
        static Action* deterrence(PlayerbotAI* botAI) { return new CastDeterrenceAction(botAI); }
        static Action* feign_death(PlayerbotAI* botAI) { return new CastFeignDeathAction(botAI); }

        static Action* aspect_of_the_hawk(PlayerbotAI* botAI) { return new CastAspectOfTheHawkAction(botAI); }
        static Action* aspect_of_the_iron_hawk(PlayerbotAI* botAI) { return new CastAspectOfTheIronHawkAction(botAI); }
        static Action* call_pet(PlayerbotAI* botAI) { return new CastCallPetAction(botAI); }
        static Action* revive_pet(PlayerbotAI* botAI) { return new CastRevivePetAction(botAI); }
        static Action* mend_pet(PlayerbotAI* botAI) { return new CastMendPetAction(botAI); }
        static Action* misdirection_on_main_tank(PlayerbotAI* botAI) { return new CastMisdirectionOnMainTankAction(botAI); }
    };
}

HunterAiObjectContext::HunterAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new HunterStrategyFactoryInternal());
    strategyContexts.Add(new HunterCombatStrategyFactoryInternal());
    actionContexts.Add(new HunterActionFactoryInternal());
    triggerContexts.Add(new HunterTriggerFactoryInternal());
}
