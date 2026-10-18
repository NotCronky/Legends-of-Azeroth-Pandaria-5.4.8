/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_WARLOCKACTIONS_H
#define _PLAYERBOT_WARLOCKACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- buffs and demons (Dark Intent covers the whole group)
BUFF_ACTION(CastDarkIntentAction, "dark intent");
BUFF_ACTION(CastSummonImpAction, "summon imp");
BUFF_ACTION(CastSummonVoidwalkerAction, "summon voidwalker");
BUFF_ACTION(CastSummonFelhunterAction, "summon felhunter");
BUFF_ACTION(CastSummonFelguardAction, "summon felguard");
BUFF_ACTION(CastCreateHealthstoneAction, "create healthstone");

// Summon the specialization's demon (Felhunter, Felguard, Imp), or the best one known before that
class CastSummonDemonAction : public Action
{
public:
    CastSummonDemonAction(PlayerbotAI* botAI) : Action(botAI, "summon demon") {}

    bool isUseful() override;
    bool Execute(Event event) override;
};

// -- affliction
DEBUFF_CHECKISOWNER_ACTION(CastAgonyAction, "agony");
DEBUFF_CHECKISOWNER_ACTION(CastCorruptionAction, "corruption");
DEBUFF_CHECKISOWNER_ACTION(CastUnstableAfflictionAction, "unstable affliction");
DEBUFF_CHECKISOWNER_ACTION(CastHauntAction, "haunt");
SPELL_ACTION(CastMaleficGraspAction, "malefic grasp");
SPELL_ACTION(CastDrainSoulAction, "drain soul");
SPELL_ACTION(CastSeedOfCorruptionAction, "seed of corruption");
BUFF_ACTION(CastDarkSoulMiseryAction, "dark soul: misery");

// -- demonology
SPELL_ACTION(CastShadowBoltAction, "shadow bolt");
SPELL_ACTION(CastSoulFireAction, "soul fire");
SPELL_ACTION(CastHandOfGuldanAction, "hand of gul'dan");
BUFF_ACTION(CastDarkSoulKnowledgeAction, "dark soul: knowledge");

// -- destruction
DEBUFF_CHECKISOWNER_ACTION(CastImmolateAction, "immolate");
SPELL_ACTION(CastConflagrateAction, "conflagrate");
SPELL_ACTION(CastIncinerateAction, "incinerate");
SPELL_ACTION(CastChaosBoltAction, "chaos bolt");
SPELL_ACTION(CastShadowburnAction, "shadowburn");
SPELL_ACTION(CastRainOfFireAction, "rain of fire");
BUFF_ACTION(CastDarkSoulInstabilityAction, "dark soul: instability");

// -- every specialization
SPELL_ACTION(CastDrainLifeAction, "drain life");
BUFF_ACTION(CastUnendingResolveAction, "unending resolve");
SPELL_ACTION(CastMortalCoilAction, "mortal coil");

// Life Tap turns health into mana: not when the warlock is hurt
class CastLifeTapAction : public CastBuffSpellAction
{
public:
    CastLifeTapAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "life tap") {}

    bool isUseful() override;
};

#endif
