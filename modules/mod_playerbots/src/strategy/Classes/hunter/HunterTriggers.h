/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_HUNTERTRIGGERS_H
#define _PLAYERBOT_HUNTERTRIGGERS_H

#include "GenericTriggers.h"

class PlayerbotAI;

// Focus at or above an amount: spend it on Arcane Shot instead of building more
class HunterFocusTrigger : public Trigger
{
public:
    HunterFocusTrigger(PlayerbotAI* botAI, std::string const name, uint32 amount) : Trigger(botAI, name), amount(amount) {}

    bool IsActive() override;

private:
    uint32 amount;
};

class HighFocusTrigger : public HunterFocusTrigger
{
public:
    HighFocusTrigger(PlayerbotAI* botAI) : HunterFocusTrigger(botAI, "high focus", 60) {}
};

// No aspect up: Hawk or Iron Hawk
class HunterNoAspectTrigger : public Trigger
{
public:
    HunterNoAspectTrigger(PlayerbotAI* botAI) : Trigger(botAI, "no aspect", 2) {}

    bool IsActive() override;
};

class HuntersPetDeadTrigger : public Trigger
{
public:
    HuntersPetDeadTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hunter pet dead", 2) {}

    bool IsActive() override;
};

class HuntersPetLowHealthTrigger : public Trigger
{
public:
    HuntersPetLowHealthTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hunter pet low health", 2) {}

    bool IsActive() override;
};

class HunterNoPetTrigger : public Trigger
{
public:
    HunterNoPetTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hunter no pet", 2) {}

    bool IsActive() override;
};

// Auto Shot keeps repeating at a target that died
class AutoShotDeadTargetTrigger : public Trigger
{
public:
    AutoShotDeadTargetTrigger(PlayerbotAI* botAI) : Trigger(botAI, "auto shot dead target") {}

    bool IsActive() override;
};

DEBUFF_CHECKISOWNER_TRIGGER(SerpentStingTrigger, "serpent sting");
DEBUFF_CHECKISOWNER_TRIGGER(BlackArrowTrigger, "black arrow");
DEBUFF_TRIGGER(HuntersMarkTrigger, "hunter's mark");
INTERRUPT_TRIGGER(CounterShotTrigger, "counter shot");
HAS_AURA_TRIGGER(LockAndLoadTrigger, "lock and load");
HAS_AURA_TRIGGER(FireProcTrigger, "fire!");
HAS_AURA_TRIGGER(ThrillOfTheHuntTrigger, "thrill of the hunt");

// Five stacks of Frenzy on the pet: Focus Fire turns them into haste
class FrenzyFiveStacksTrigger : public HasAuraStackTrigger
{
public:
    FrenzyFiveStacksTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "frenzy", 5) {}

    Value<Unit*>* GetTargetValue() override;
    bool IsActive() override;
};

// Misdirection is missing from the main tank, in a group with one
class MisdirectionOnMainTankTrigger : public BuffOnMainTankTrigger
{
public:
    MisdirectionOnMainTankTrigger(PlayerbotAI* botAI) : BuffOnMainTankTrigger(botAI, "misdirection", true) {}
};

#endif
