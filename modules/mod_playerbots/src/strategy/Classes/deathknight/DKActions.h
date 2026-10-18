/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DKACTIONS_H
#define _PLAYERBOT_DKACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- presences and buffs
BUFF_ACTION(CastBloodPresenceAction, "blood presence");
BUFF_ACTION(CastFrostPresenceAction, "frost presence");
BUFF_ACTION(CastUnholyPresenceAction, "unholy presence");
BUFF_ACTION(CastHornOfWinterAction, "horn of winter");
BUFF_ACTION(CastBoneShieldAction, "bone shield");

// -- diseases
SPELL_ACTION(CastOutbreakAction, "outbreak");

// Icy Touch for Frost Fever and Plague Strike for Blood Plague, when Outbreak didn't put them up
class CastIcyTouchAction : public CastSpellAction
{
public:
    CastIcyTouchAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "icy touch") {}

    bool isUseful() override;
};

class CastPlagueStrikeAction : public CastMeleeSpellAction
{
public:
    CastPlagueStrikeAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "plague strike") {}

    bool isUseful() override;
};

SPELL_ACTION(CastPlagueLeechAction, "plague leech");

// -- frost
MELEE_ACTION(CastObliterateAction, "obliterate");
MELEE_ACTION(CastFrostStrikeAction, "frost strike");
SPELL_ACTION(CastHowlingBlastAction, "howling blast");
BUFF_ACTION(CastPillarOfFrostAction, "pillar of frost");

// -- unholy
MELEE_ACTION(CastScourgeStrikeAction, "scourge strike");
MELEE_ACTION(CastFesteringStrikeAction, "festering strike");
SPELL_ACTION(CastDeathCoilAction, "death coil");
SPELL_ACTION(CastSummonGargoyleAction, "summon gargoyle");
BUFF_ACTION(CastUnholyFrenzyAction, "unholy frenzy");

// Dark Transformation is cast on the ghoul
class CastDarkTransformationAction : public CastBuffSpellAction
{
public:
    CastDarkTransformationAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "dark transformation") {}

    std::string const GetTargetName() override { return "pet target"; }
};

// -- blood
MELEE_ACTION(CastHeartStrikeAction, "heart strike");
MELEE_ACTION(CastDeathStrikeAction, "death strike");
MELEE_ACTION(CastRuneStrikeAction, "rune strike");
SPELL_ACTION(CastBloodBoilAction, "blood boil");
BUFF_ACTION(CastVampiricBloodAction, "vampiric blood");
SPELL_ACTION(CastDancingRuneWeaponAction, "dancing rune weapon");
BUFF_ACTION(CastRuneTapAction, "rune tap");
SPELL_ACTION(CastDarkCommandAction, "dark command");

// -- every specialization
MELEE_ACTION(CastSoulReaperAction, "soul reaper");
SPELL_ACTION(CastDeathAndDecayAction, "death and decay");
BUFF_ACTION(CastEmpowerRuneWeaponAction, "empower rune weapon");
BUFF_ACTION(CastBloodTapAction, "blood tap");
MELEE_ACTION(CastMindFreezeAction, "mind freeze");
ENEMY_HEALER_ACTION(CastMindFreezeOnEnemyHealerAction, "mind freeze");
BUFF_ACTION(CastIceboundFortitudeAction, "icebound fortitude");
BUFF_ACTION(CastAntiMagicShellAction, "anti-magic shell");
BUFF_ACTION(CastRemorselessWinterAction, "remorseless winter");

// The ghoul: a pet for Unholy, a minute-long guardian for the others
class CastRaiseDeadAction : public CastBuffSpellAction
{
public:
    CastRaiseDeadAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "raise dead") {}

    bool isUseful() override;
};

#endif
