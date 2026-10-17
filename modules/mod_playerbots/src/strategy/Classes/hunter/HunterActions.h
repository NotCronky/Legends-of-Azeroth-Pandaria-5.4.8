/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_HUNTERACTIONS_H
#define _PLAYERBOT_HUNTERACTIONS_H

#include "AiObject.h"
#include "GenericSpellActions.h"

class PlayerbotAI;

// Start shooting; it repeats on its own until the target changes
class CastAutoShotAction : public CastSpellAction
{
public:
    CastAutoShotAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "auto shot") {}

    ActionThreatType getThreatType() override { return ActionThreatType::None; }
    bool isUseful() override;
};

class StopAutoShotAction : public Action
{
public:
    StopAutoShotAction(PlayerbotAI* botAI) : Action(botAI, "stop auto shot") {}

    bool Execute(Event event) override;
};

// -- shots
SPELL_ACTION(CastArcaneShotAction, "arcane shot");
SPELL_ACTION(CastSteadyShotAction, "steady shot");
SPELL_ACTION(CastCobraShotAction, "cobra shot");
SPELL_ACTION(CastKillShotAction, "kill shot");
SPELL_ACTION(CastMultiShotAction, "multi-shot");
SPELL_ACTION(CastChimeraShotAction, "chimera shot");
SPELL_ACTION(CastAimedShotAction, "aimed shot");
SPELL_ACTION(CastExplosiveShotAction, "explosive shot");
DEBUFF_CHECKISOWNER_ACTION(CastBlackArrowAction, "black arrow");
DEBUFF_CHECKISOWNER_ACTION(CastSerpentStingAction, "serpent sting");
DEBUFF_ACTION(CastHuntersMarkAction, "hunter's mark");
SNARE_ACTION(CastConcussiveShotAction, "concussive shot");
SPELL_ACTION(CastCounterShotAction, "counter shot");
SPELL_ACTION(CastTranquilizingShotAction, "tranquilizing shot");

// -- talents
SPELL_ACTION(CastGlaiveTossAction, "glaive toss");
SPELL_ACTION(CastPowershotAction, "powershot");
SPELL_ACTION(CastBarrageAction, "barrage");
SPELL_ACTION(CastMurderOfCrowsAction, "a murder of crows");
SPELL_ACTION(CastLynxRushAction, "lynx rush");
SPELL_ACTION(CastDireBeastAction, "dire beast");
BUFF_ACTION(CastFervorAction, "fervor");

// -- cooldowns
BUFF_ACTION(CastRapidFireAction, "rapid fire");
BUFF_ACTION(CastFocusFireAction, "focus fire");
SPELL_ACTION(CastStampedeAction, "stampede");
BUFF_ACTION(CastDeterrenceAction, "deterrence");
BUFF_ACTION(CastFeignDeathAction, "feign death");

// Bestial Wrath enrages the pet; cast on the bot
class CastBestialWrathAction : public CastBuffSpellAction
{
public:
    CastBestialWrathAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "bestial wrath") {}

    bool isUseful() override;
};

// Kill Command: the pet attacks the target
class CastKillCommandAction : public CastSpellAction
{
public:
    CastKillCommandAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "kill command") {}

    bool isUseful() override;
};

// -- aspects: Hawk, or Iron Hawk with the talent
class CastAspectOfTheHawkAction : public CastBuffSpellAction
{
public:
    CastAspectOfTheHawkAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "aspect of the hawk") {}

    bool isUseful() override;
};

BUFF_ACTION(CastAspectOfTheIronHawkAction, "aspect of the iron hawk");

// -- pet
// Call the pet in the first slot; if a call a moment ago brought nothing, it is dead: revive it
class CastCallPetAction : public CastBuffSpellAction
{
public:
    CastCallPetAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "call pet 1") {}

    bool Execute(Event event) override;
    bool isUseful() override;

private:
    uint32 lastCall = 0;
};
BUFF_ACTION(CastRevivePetAction, "revive pet");

class CastMendPetAction : public CastAuraSpellAction
{
public:
    CastMendPetAction(PlayerbotAI* botAI) : CastAuraSpellAction(botAI, "mend pet") {}

    std::string const GetTargetName() override { return "pet target"; }
};

// Threat from the bot's shots goes to the tank
class CastMisdirectionOnMainTankAction : public BuffOnMainTankAction
{
public:
    CastMisdirectionOnMainTankAction(PlayerbotAI* botAI) : BuffOnMainTankAction(botAI, "misdirection", true) {}
};

#endif
