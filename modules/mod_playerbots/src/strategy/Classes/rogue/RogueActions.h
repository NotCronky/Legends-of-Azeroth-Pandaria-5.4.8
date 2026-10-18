/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_ROGUEACTIONS_H
#define _PLAYERBOT_ROGUEACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- combo point builders
MELEE_ACTION(CastSinisterStrikeAction, "sinister strike");
MELEE_ACTION(CastMutilateAction, "mutilate");
MELEE_ACTION(CastDispatchAction, "dispatch");
MELEE_ACTION(CastBackstabAction, "backstab");
MELEE_ACTION(CastHemorrhageAction, "hemorrhage");
MELEE_ACTION(CastRevealingStrikeAction, "revealing strike");
MELEE_ACTION(CastAmbushAction, "ambush");
MELEE_ACTION(CastGarroteAction, "garrote");
MELEE_ACTION(CastCheapShotAction, "cheap shot");

// -- finishers
MELEE_ACTION(CastEviscerateAction, "eviscerate");
MELEE_ACTION(CastEnvenomAction, "envenom");
MELEE_ACTION(CastRuptureAction, "rupture");
BUFF_ACTION(CastSliceAndDiceAction, "slice and dice");

// -- cooldowns
MELEE_ACTION(CastVendettaAction, "vendetta");
BUFF_ACTION(CastAdrenalineRushAction, "adrenaline rush");
MELEE_ACTION(CastKillingSpreeAction, "killing spree");
BUFF_ACTION(CastShadowDanceAction, "shadow dance");
BUFF_ACTION(CastShadowBladesAction, "shadow blades");
SPELL_ACTION(CastPremeditationAction, "premeditation");

// -- utility and survival
MELEE_ACTION(CastKickAction, "kick");
ENEMY_HEALER_ACTION(CastKickOnEnemyHealerAction, "kick");
BUFF_ACTION(CastEvasionAction, "evasion");
BUFF_ACTION(CastFeintAction, "feint");
BUFF_ACTION(CastCloakOfShadowsAction, "cloak of shadows");
BUFF_ACTION(CastSprintAction, "sprint");

// -- area
class CastFanOfKnivesAction : public CastMeleeSpellAction
{
public:
    CastFanOfKnivesAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "fan of knives") {}

    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastCrimsonTempestAction : public CastMeleeSpellAction
{
public:
    CastCrimsonTempestAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "crimson tempest") {}

    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

// A toggle: casting it again while it is up turns it off, which BUFF_ACTION's isUseful prevents
BUFF_ACTION(CastBladeFlurryAction, "blade flurry");

// -- poisons (self buffs in MoP)
BUFF_ACTION(CastDeadlyPoisonAction, "deadly poison");
BUFF_ACTION(CastWoundPoisonAction, "wound poison");
BUFF_ACTION(CastCripplingPoisonAction, "crippling poison");
BUFF_ACTION(CastMindNumbingPoisonAction, "mind-numbing poison");

// Stealth to open on a target; not with a battleground flag
class CastStealthAction : public CastBuffSpellAction
{
public:
    CastStealthAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "stealth") {}

    bool isPossible() override;
};

// Vanish to drop threat; not with a battleground flag
class CastVanishAction : public CastBuffSpellAction
{
public:
    CastVanishAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "vanish") {}

    bool isUseful() override;
};

class UnstealthAction : public Action
{
public:
    UnstealthAction(PlayerbotAI* botAI) : Action(botAI, "unstealth") {}

    bool Execute(Event event) override;
};

class CastTricksOfTheTradeOnMainTankAction : public BuffOnMainTankAction
{
public:
    CastTricksOfTheTradeOnMainTankAction(PlayerbotAI* botAI) : BuffOnMainTankAction(botAI, "tricks of the trade", true) {}

    bool isUseful() override;
};

#endif
