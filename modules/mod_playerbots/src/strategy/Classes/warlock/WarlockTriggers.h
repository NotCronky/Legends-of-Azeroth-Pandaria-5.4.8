/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_WARLOCKTRIGGERS_H
#define _PLAYERBOT_WARLOCKTRIGGERS_H

#include "GenericTriggers.h"

class PlayerbotAI;

BUFF_TRIGGER(DarkIntentTrigger, "dark intent");

// No healthstone in the bags, and the warlock can make one and has room for it
class NoHealthstoneTrigger : public Trigger
{
public:
    NoHealthstoneTrigger(PlayerbotAI* botAI) : Trigger(botAI, "no healthstone", 5) {}

    bool IsActive() override;
};

// -- affliction
DEBUFF_CHECKISOWNER_TRIGGER(AgonyTrigger, "agony");
DEBUFF_CHECKISOWNER_TRIGGER(CorruptionTrigger, "corruption");
DEBUFF_CHECKISOWNER_TRIGGER(UnstableAfflictionTrigger, "unstable affliction");
DEBUFF_CHECKISOWNER_TRIGGER(HauntTrigger, "haunt");
CAN_CAST_TRIGGER(DarkSoulMiseryTrigger, "dark soul: misery");

// The target is below 20%: Drain Soul
class DrainSoulTrigger : public Trigger
{
public:
    DrainSoulTrigger(PlayerbotAI* botAI) : Trigger(botAI, "drain soul") {}

    bool IsActive() override;
};

// -- demonology
HAS_AURA_TRIGGER(MoltenCoreTrigger, "molten core");
DEBUFF_CHECKISOWNER_TRIGGER(ShadowflameTrigger, "shadowflame");
CAN_CAST_TRIGGER(DarkSoulKnowledgeTrigger, "dark soul: knowledge");

// -- destruction
DEBUFF_CHECKISOWNER_TRIGGER(ImmolateTrigger, "immolate");
CAN_CAST_TRIGGER(ConflagrateTrigger, "conflagrate");
CAN_CAST_TRIGGER(DarkSoulInstabilityTrigger, "dark soul: instability");

// Three Burning Embers: Chaos Bolt
class BurningEmbersTrigger : public Trigger
{
public:
    BurningEmbersTrigger(PlayerbotAI* botAI) : Trigger(botAI, "burning embers") {}

    bool IsActive() override;
};

#endif
