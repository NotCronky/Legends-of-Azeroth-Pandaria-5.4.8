/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "WarlockAiObjectContext.h"

#include "GenericWarlockNonCombatStrategy.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"
#include "WarlockActions.h"
#include "WarlockStrategies.h"
#include "WarlockTriggers.h"

namespace
{
    class WarlockStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        WarlockStrategyFactoryInternal()
        {
            creators["nc"] = &WarlockStrategyFactoryInternal::nc;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericWarlockNonCombatStrategy(botAI); }
    };

    // One specialization at a time
    class WarlockCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        WarlockCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["affli"] = &WarlockCombatStrategyFactoryInternal::affliction;
            creators["affliction"] = &WarlockCombatStrategyFactoryInternal::affliction;
            creators["demono"] = &WarlockCombatStrategyFactoryInternal::demonology;
            creators["demonology"] = &WarlockCombatStrategyFactoryInternal::demonology;
            creators["destru"] = &WarlockCombatStrategyFactoryInternal::destruction;
            creators["destruction"] = &WarlockCombatStrategyFactoryInternal::destruction;
        }

    private:
        static Strategy* affliction(PlayerbotAI* botAI) { return new AfflictionWarlockStrategy(botAI); }
        static Strategy* demonology(PlayerbotAI* botAI) { return new DemonologyWarlockStrategy(botAI); }
        static Strategy* destruction(PlayerbotAI* botAI) { return new DestructionWarlockStrategy(botAI); }
    };

    class WarlockTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        WarlockTriggerFactoryInternal()
        {
            creators["dark intent"] = &WarlockTriggerFactoryInternal::dark_intent;
            creators["no healthstone"] = &WarlockTriggerFactoryInternal::no_healthstone;
            creators["agony"] = &WarlockTriggerFactoryInternal::agony;
            creators["corruption"] = &WarlockTriggerFactoryInternal::corruption;
            creators["unstable affliction"] = &WarlockTriggerFactoryInternal::unstable_affliction;
            creators["haunt"] = &WarlockTriggerFactoryInternal::haunt;
            creators["dark soul: misery"] = &WarlockTriggerFactoryInternal::dark_soul_misery;
            creators["drain soul"] = &WarlockTriggerFactoryInternal::drain_soul;
            creators["molten core"] = &WarlockTriggerFactoryInternal::molten_core;
            creators["shadowflame"] = &WarlockTriggerFactoryInternal::shadowflame;
            creators["dark soul: knowledge"] = &WarlockTriggerFactoryInternal::dark_soul_knowledge;
            creators["immolate"] = &WarlockTriggerFactoryInternal::immolate;
            creators["conflagrate"] = &WarlockTriggerFactoryInternal::conflagrate;
            creators["dark soul: instability"] = &WarlockTriggerFactoryInternal::dark_soul_instability;
            creators["burning embers"] = &WarlockTriggerFactoryInternal::burning_embers;
        }

    private:
        static Trigger* dark_intent(PlayerbotAI* botAI) { return new DarkIntentTrigger(botAI); }
        static Trigger* no_healthstone(PlayerbotAI* botAI) { return new NoHealthstoneTrigger(botAI); }
        static Trigger* agony(PlayerbotAI* botAI) { return new AgonyTrigger(botAI); }
        static Trigger* corruption(PlayerbotAI* botAI) { return new CorruptionTrigger(botAI); }
        static Trigger* unstable_affliction(PlayerbotAI* botAI) { return new UnstableAfflictionTrigger(botAI); }
        static Trigger* haunt(PlayerbotAI* botAI) { return new HauntTrigger(botAI); }
        static Trigger* dark_soul_misery(PlayerbotAI* botAI) { return new DarkSoulMiseryTrigger(botAI); }
        static Trigger* drain_soul(PlayerbotAI* botAI) { return new DrainSoulTrigger(botAI); }
        static Trigger* molten_core(PlayerbotAI* botAI) { return new MoltenCoreTrigger(botAI); }
        static Trigger* shadowflame(PlayerbotAI* botAI) { return new ShadowflameTrigger(botAI); }
        static Trigger* dark_soul_knowledge(PlayerbotAI* botAI) { return new DarkSoulKnowledgeTrigger(botAI); }
        static Trigger* immolate(PlayerbotAI* botAI) { return new ImmolateTrigger(botAI); }
        static Trigger* conflagrate(PlayerbotAI* botAI) { return new ConflagrateTrigger(botAI); }
        static Trigger* dark_soul_instability(PlayerbotAI* botAI) { return new DarkSoulInstabilityTrigger(botAI); }
        static Trigger* burning_embers(PlayerbotAI* botAI) { return new BurningEmbersTrigger(botAI); }
    };

    class WarlockActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        WarlockActionFactoryInternal()
        {
            creators["dark intent"] = &WarlockActionFactoryInternal::dark_intent;
            creators["summon imp"] = &WarlockActionFactoryInternal::summon_imp;
            creators["summon voidwalker"] = &WarlockActionFactoryInternal::summon_voidwalker;
            creators["summon felhunter"] = &WarlockActionFactoryInternal::summon_felhunter;
            creators["summon felguard"] = &WarlockActionFactoryInternal::summon_felguard;
            creators["summon demon"] = &WarlockActionFactoryInternal::summon_demon;
            creators["create healthstone"] = &WarlockActionFactoryInternal::create_healthstone;

            creators["agony"] = &WarlockActionFactoryInternal::agony;
            creators["corruption"] = &WarlockActionFactoryInternal::corruption;
            creators["unstable affliction"] = &WarlockActionFactoryInternal::unstable_affliction;
            creators["haunt"] = &WarlockActionFactoryInternal::haunt;
            creators["malefic grasp"] = &WarlockActionFactoryInternal::malefic_grasp;
            creators["drain soul"] = &WarlockActionFactoryInternal::drain_soul;
            creators["seed of corruption"] = &WarlockActionFactoryInternal::seed_of_corruption;
            creators["dark soul: misery"] = &WarlockActionFactoryInternal::dark_soul_misery;

            creators["shadow bolt"] = &WarlockActionFactoryInternal::shadow_bolt;
            creators["soul fire"] = &WarlockActionFactoryInternal::soul_fire;
            creators["hand of gul'dan"] = &WarlockActionFactoryInternal::hand_of_guldan;
            creators["dark soul: knowledge"] = &WarlockActionFactoryInternal::dark_soul_knowledge;

            creators["immolate"] = &WarlockActionFactoryInternal::immolate;
            creators["conflagrate"] = &WarlockActionFactoryInternal::conflagrate;
            creators["incinerate"] = &WarlockActionFactoryInternal::incinerate;
            creators["chaos bolt"] = &WarlockActionFactoryInternal::chaos_bolt;
            creators["shadowburn"] = &WarlockActionFactoryInternal::shadowburn;
            creators["rain of fire"] = &WarlockActionFactoryInternal::rain_of_fire;
            creators["dark soul: instability"] = &WarlockActionFactoryInternal::dark_soul_instability;

            creators["drain life"] = &WarlockActionFactoryInternal::drain_life;
            creators["unending resolve"] = &WarlockActionFactoryInternal::unending_resolve;
            creators["mortal coil"] = &WarlockActionFactoryInternal::mortal_coil;
            creators["life tap"] = &WarlockActionFactoryInternal::life_tap;
        }

    private:
        static Action* dark_intent(PlayerbotAI* botAI) { return new CastDarkIntentAction(botAI); }
        static Action* summon_imp(PlayerbotAI* botAI) { return new CastSummonImpAction(botAI); }
        static Action* summon_voidwalker(PlayerbotAI* botAI) { return new CastSummonVoidwalkerAction(botAI); }
        static Action* summon_felhunter(PlayerbotAI* botAI) { return new CastSummonFelhunterAction(botAI); }
        static Action* summon_felguard(PlayerbotAI* botAI) { return new CastSummonFelguardAction(botAI); }
        static Action* summon_demon(PlayerbotAI* botAI) { return new CastSummonDemonAction(botAI); }
        static Action* create_healthstone(PlayerbotAI* botAI) { return new CastCreateHealthstoneAction(botAI); }

        static Action* agony(PlayerbotAI* botAI) { return new CastAgonyAction(botAI); }
        static Action* corruption(PlayerbotAI* botAI) { return new CastCorruptionAction(botAI); }
        static Action* unstable_affliction(PlayerbotAI* botAI) { return new CastUnstableAfflictionAction(botAI); }
        static Action* haunt(PlayerbotAI* botAI) { return new CastHauntAction(botAI); }
        static Action* malefic_grasp(PlayerbotAI* botAI) { return new CastMaleficGraspAction(botAI); }
        static Action* drain_soul(PlayerbotAI* botAI) { return new CastDrainSoulAction(botAI); }
        static Action* seed_of_corruption(PlayerbotAI* botAI) { return new CastSeedOfCorruptionAction(botAI); }
        static Action* dark_soul_misery(PlayerbotAI* botAI) { return new CastDarkSoulMiseryAction(botAI); }

        static Action* shadow_bolt(PlayerbotAI* botAI) { return new CastShadowBoltAction(botAI); }
        static Action* soul_fire(PlayerbotAI* botAI) { return new CastSoulFireAction(botAI); }
        static Action* hand_of_guldan(PlayerbotAI* botAI) { return new CastHandOfGuldanAction(botAI); }
        static Action* dark_soul_knowledge(PlayerbotAI* botAI) { return new CastDarkSoulKnowledgeAction(botAI); }

        static Action* immolate(PlayerbotAI* botAI) { return new CastImmolateAction(botAI); }
        static Action* conflagrate(PlayerbotAI* botAI) { return new CastConflagrateAction(botAI); }
        static Action* incinerate(PlayerbotAI* botAI) { return new CastIncinerateAction(botAI); }
        static Action* chaos_bolt(PlayerbotAI* botAI) { return new CastChaosBoltAction(botAI); }
        static Action* shadowburn(PlayerbotAI* botAI) { return new CastShadowburnAction(botAI); }
        static Action* rain_of_fire(PlayerbotAI* botAI) { return new CastRainOfFireAction(botAI); }
        static Action* dark_soul_instability(PlayerbotAI* botAI) { return new CastDarkSoulInstabilityAction(botAI); }

        static Action* drain_life(PlayerbotAI* botAI) { return new CastDrainLifeAction(botAI); }
        static Action* unending_resolve(PlayerbotAI* botAI) { return new CastUnendingResolveAction(botAI); }
        static Action* mortal_coil(PlayerbotAI* botAI) { return new CastMortalCoilAction(botAI); }
        static Action* life_tap(PlayerbotAI* botAI) { return new CastLifeTapAction(botAI); }
    };
}

WarlockAiObjectContext::WarlockAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new WarlockStrategyFactoryInternal());
    strategyContexts.Add(new WarlockCombatStrategyFactoryInternal());
    actionContexts.Add(new WarlockActionFactoryInternal());
    triggerContexts.Add(new WarlockTriggerFactoryInternal());
}
