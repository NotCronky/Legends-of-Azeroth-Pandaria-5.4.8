/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "RogueAiObjectContext.h"

#include "GenericRogueNonCombatStrategy.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"
#include "RogueActions.h"
#include "RogueStrategies.h"
#include "RogueTriggers.h"

namespace
{
    class RogueStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        RogueStrategyFactoryInternal()
        {
            creators["nc"] = &RogueStrategyFactoryInternal::nc;
            creators["aoe"] = &RogueStrategyFactoryInternal::aoe;
            creators["boost"] = &RogueStrategyFactoryInternal::boost;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericRogueNonCombatStrategy(botAI); }
        static Strategy* aoe(PlayerbotAI* botAI) { return new RogueAoeStrategy(botAI); }
        static Strategy* boost(PlayerbotAI* botAI) { return new RogueBoostStrategy(botAI); }
    };

    // One specialization at a time
    class RogueCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        RogueCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["assassination"] = &RogueCombatStrategyFactoryInternal::assassination;
            creators["combat"] = &RogueCombatStrategyFactoryInternal::combat;
            creators["subtlety"] = &RogueCombatStrategyFactoryInternal::subtlety;
        }

    private:
        static Strategy* assassination(PlayerbotAI* botAI) { return new AssassinationRogueStrategy(botAI); }
        static Strategy* combat(PlayerbotAI* botAI) { return new CombatRogueStrategy(botAI); }
        static Strategy* subtlety(PlayerbotAI* botAI) { return new SubtletyRogueStrategy(botAI); }
    };

    class RogueTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        RogueTriggerFactoryInternal()
        {
            creators["finisher"] = &RogueTriggerFactoryInternal::finisher;
            creators["combo points target dying"] = &RogueTriggerFactoryInternal::target_dying;
            creators["slice and dice"] = &RogueTriggerFactoryInternal::slice_and_dice;
            creators["rupture"] = &RogueTriggerFactoryInternal::rupture;
            creators["revealing strike"] = &RogueTriggerFactoryInternal::revealing_strike;
            creators["hemorrhage"] = &RogueTriggerFactoryInternal::hemorrhage;
            creators["blindside"] = &RogueTriggerFactoryInternal::blindside;
            creators["stealthed"] = &RogueTriggerFactoryInternal::stealthed;
            creators["kick"] = &RogueTriggerFactoryInternal::kick;
            creators["kick on enemy healer"] = &RogueTriggerFactoryInternal::kick_on_enemy_healer;
            creators["vendetta"] = &RogueTriggerFactoryInternal::vendetta;
            creators["adrenaline rush"] = &RogueTriggerFactoryInternal::adrenaline_rush;
            creators["killing spree"] = &RogueTriggerFactoryInternal::killing_spree;
            creators["shadow dance"] = &RogueTriggerFactoryInternal::shadow_dance;
            creators["shadow blades"] = &RogueTriggerFactoryInternal::shadow_blades;
            creators["lethal poison"] = &RogueTriggerFactoryInternal::lethal_poison;
            creators["non-lethal poison"] = &RogueTriggerFactoryInternal::non_lethal_poison;
            creators["tricks of the trade on main tank"] = &RogueTriggerFactoryInternal::tricks_of_the_trade_on_main_tank;
        }

    private:
        static Trigger* finisher(PlayerbotAI* botAI) { return new RogueFinisherTrigger(botAI); }
        static Trigger* target_dying(PlayerbotAI* botAI) { return new RogueTargetDyingTrigger(botAI); }
        static Trigger* slice_and_dice(PlayerbotAI* botAI) { return new SliceAndDiceTrigger(botAI); }
        static Trigger* rupture(PlayerbotAI* botAI) { return new RuptureTrigger(botAI); }
        static Trigger* revealing_strike(PlayerbotAI* botAI) { return new RevealingStrikeTrigger(botAI); }
        static Trigger* hemorrhage(PlayerbotAI* botAI) { return new HemorrhageTrigger(botAI); }
        static Trigger* blindside(PlayerbotAI* botAI) { return new BlindsideTrigger(botAI); }
        static Trigger* stealthed(PlayerbotAI* botAI) { return new RogueStealthedTrigger(botAI); }
        static Trigger* kick(PlayerbotAI* botAI) { return new KickInterruptSpellTrigger(botAI); }
        static Trigger* kick_on_enemy_healer(PlayerbotAI* botAI) { return new KickInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* vendetta(PlayerbotAI* botAI) { return new VendettaTrigger(botAI); }
        static Trigger* adrenaline_rush(PlayerbotAI* botAI) { return new AdrenalineRushTrigger(botAI); }
        static Trigger* killing_spree(PlayerbotAI* botAI) { return new KillingSpreeTrigger(botAI); }
        static Trigger* shadow_dance(PlayerbotAI* botAI) { return new ShadowDanceTrigger(botAI); }
        static Trigger* shadow_blades(PlayerbotAI* botAI) { return new ShadowBladesTrigger(botAI); }
        static Trigger* lethal_poison(PlayerbotAI* botAI) { return new RoguePoisonTrigger(botAI, "lethal poison", true); }
        static Trigger* non_lethal_poison(PlayerbotAI* botAI) { return new RoguePoisonTrigger(botAI, "non-lethal poison", false); }
        static Trigger* tricks_of_the_trade_on_main_tank(PlayerbotAI* botAI) { return new TricksOfTheTradeOnMainTankTrigger(botAI); }
    };

    class RogueActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        RogueActionFactoryInternal()
        {
            creators["sinister strike"] = &RogueActionFactoryInternal::sinister_strike;
            creators["mutilate"] = &RogueActionFactoryInternal::mutilate;
            creators["dispatch"] = &RogueActionFactoryInternal::dispatch;
            creators["backstab"] = &RogueActionFactoryInternal::backstab;
            creators["hemorrhage"] = &RogueActionFactoryInternal::hemorrhage;
            creators["revealing strike"] = &RogueActionFactoryInternal::revealing_strike;
            creators["ambush"] = &RogueActionFactoryInternal::ambush;
            creators["garrote"] = &RogueActionFactoryInternal::garrote;
            creators["cheap shot"] = &RogueActionFactoryInternal::cheap_shot;

            creators["eviscerate"] = &RogueActionFactoryInternal::eviscerate;
            creators["envenom"] = &RogueActionFactoryInternal::envenom;
            creators["rupture"] = &RogueActionFactoryInternal::rupture;
            creators["slice and dice"] = &RogueActionFactoryInternal::slice_and_dice;

            creators["vendetta"] = &RogueActionFactoryInternal::vendetta;
            creators["adrenaline rush"] = &RogueActionFactoryInternal::adrenaline_rush;
            creators["killing spree"] = &RogueActionFactoryInternal::killing_spree;
            creators["shadow dance"] = &RogueActionFactoryInternal::shadow_dance;
            creators["shadow blades"] = &RogueActionFactoryInternal::shadow_blades;
            creators["premeditation"] = &RogueActionFactoryInternal::premeditation;

            creators["kick"] = &RogueActionFactoryInternal::kick;
            creators["kick on enemy healer"] = &RogueActionFactoryInternal::kick_on_enemy_healer;
            creators["evasion"] = &RogueActionFactoryInternal::evasion;
            creators["feint"] = &RogueActionFactoryInternal::feint;
            creators["cloak of shadows"] = &RogueActionFactoryInternal::cloak_of_shadows;
            creators["sprint"] = &RogueActionFactoryInternal::sprint;
            creators["stealth"] = &RogueActionFactoryInternal::stealth;
            creators["vanish"] = &RogueActionFactoryInternal::vanish;
            creators["unstealth"] = &RogueActionFactoryInternal::unstealth;
            creators["tricks of the trade on main tank"] = &RogueActionFactoryInternal::tricks_of_the_trade_on_main_tank;

            creators["fan of knives"] = &RogueActionFactoryInternal::fan_of_knives;
            creators["crimson tempest"] = &RogueActionFactoryInternal::crimson_tempest;
            creators["blade flurry"] = &RogueActionFactoryInternal::blade_flurry;

            creators["deadly poison"] = &RogueActionFactoryInternal::deadly_poison;
            creators["wound poison"] = &RogueActionFactoryInternal::wound_poison;
            creators["crippling poison"] = &RogueActionFactoryInternal::crippling_poison;
            creators["mind-numbing poison"] = &RogueActionFactoryInternal::mind_numbing_poison;
        }

    private:
        static Action* sinister_strike(PlayerbotAI* botAI) { return new CastSinisterStrikeAction(botAI); }
        static Action* mutilate(PlayerbotAI* botAI) { return new CastMutilateAction(botAI); }
        static Action* dispatch(PlayerbotAI* botAI) { return new CastDispatchAction(botAI); }
        static Action* backstab(PlayerbotAI* botAI) { return new CastBackstabAction(botAI); }
        static Action* hemorrhage(PlayerbotAI* botAI) { return new CastHemorrhageAction(botAI); }
        static Action* revealing_strike(PlayerbotAI* botAI) { return new CastRevealingStrikeAction(botAI); }
        static Action* ambush(PlayerbotAI* botAI) { return new CastAmbushAction(botAI); }
        static Action* garrote(PlayerbotAI* botAI) { return new CastGarroteAction(botAI); }
        static Action* cheap_shot(PlayerbotAI* botAI) { return new CastCheapShotAction(botAI); }

        static Action* eviscerate(PlayerbotAI* botAI) { return new CastEviscerateAction(botAI); }
        static Action* envenom(PlayerbotAI* botAI) { return new CastEnvenomAction(botAI); }
        static Action* rupture(PlayerbotAI* botAI) { return new CastRuptureAction(botAI); }
        static Action* slice_and_dice(PlayerbotAI* botAI) { return new CastSliceAndDiceAction(botAI); }

        static Action* vendetta(PlayerbotAI* botAI) { return new CastVendettaAction(botAI); }
        static Action* adrenaline_rush(PlayerbotAI* botAI) { return new CastAdrenalineRushAction(botAI); }
        static Action* killing_spree(PlayerbotAI* botAI) { return new CastKillingSpreeAction(botAI); }
        static Action* shadow_dance(PlayerbotAI* botAI) { return new CastShadowDanceAction(botAI); }
        static Action* shadow_blades(PlayerbotAI* botAI) { return new CastShadowBladesAction(botAI); }
        static Action* premeditation(PlayerbotAI* botAI) { return new CastPremeditationAction(botAI); }

        static Action* kick(PlayerbotAI* botAI) { return new CastKickAction(botAI); }
        static Action* kick_on_enemy_healer(PlayerbotAI* botAI) { return new CastKickOnEnemyHealerAction(botAI); }
        static Action* evasion(PlayerbotAI* botAI) { return new CastEvasionAction(botAI); }
        static Action* feint(PlayerbotAI* botAI) { return new CastFeintAction(botAI); }
        static Action* cloak_of_shadows(PlayerbotAI* botAI) { return new CastCloakOfShadowsAction(botAI); }
        static Action* sprint(PlayerbotAI* botAI) { return new CastSprintAction(botAI); }
        static Action* stealth(PlayerbotAI* botAI) { return new CastStealthAction(botAI); }
        static Action* vanish(PlayerbotAI* botAI) { return new CastVanishAction(botAI); }
        static Action* unstealth(PlayerbotAI* botAI) { return new UnstealthAction(botAI); }
        static Action* tricks_of_the_trade_on_main_tank(PlayerbotAI* botAI) { return new CastTricksOfTheTradeOnMainTankAction(botAI); }

        static Action* fan_of_knives(PlayerbotAI* botAI) { return new CastFanOfKnivesAction(botAI); }
        static Action* crimson_tempest(PlayerbotAI* botAI) { return new CastCrimsonTempestAction(botAI); }
        static Action* blade_flurry(PlayerbotAI* botAI) { return new CastBladeFlurryAction(botAI); }

        static Action* deadly_poison(PlayerbotAI* botAI) { return new CastDeadlyPoisonAction(botAI); }
        static Action* wound_poison(PlayerbotAI* botAI) { return new CastWoundPoisonAction(botAI); }
        static Action* crippling_poison(PlayerbotAI* botAI) { return new CastCripplingPoisonAction(botAI); }
        static Action* mind_numbing_poison(PlayerbotAI* botAI) { return new CastMindNumbingPoisonAction(botAI); }
    };
}

RogueAiObjectContext::RogueAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new RogueStrategyFactoryInternal());
    strategyContexts.Add(new RogueCombatStrategyFactoryInternal());
    actionContexts.Add(new RogueActionFactoryInternal());
    triggerContexts.Add(new RogueTriggerFactoryInternal());
}
