/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PRIESTTRIGGERS_H
#define _PLAYERBOT_PRIESTTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

// No stamina buff of any kind (Power Word: Fortitude, Commanding Shout, Blood Pact, Qiraji Fortitude)
class PowerWordFortitudeTrigger : public Trigger
{
public:
    PowerWordFortitudeTrigger(PlayerbotAI* botAI) : Trigger(botAI, "power word: fortitude", 2) {}

    bool IsActive() override;
};

BUFF_TRIGGER(InnerFireTrigger, "inner fire");
BUFF_TRIGGER(ShadowformTrigger, "shadowform");
BUFF_TRIGGER(ChakraSerenityTrigger, "chakra: serenity");

DEBUFF_CHECKISOWNER_TRIGGER(ShadowWordPainTrigger, "shadow word: pain");
DEBUFF_CHECKISOWNER_TRIGGER(VampiricTouchTrigger, "vampiric touch");
HAS_AURA_TRIGGER(SurgeOfDarknessTrigger, "surge of darkness");
CAN_CAST_TRIGGER(HaloTrigger, "halo");

// Mindbender (level 45 talent) or Shadowfiend is ready
class ShadowfiendTrigger : public Trigger
{
public:
    ShadowfiendTrigger(PlayerbotAI* botAI) : Trigger(botAI, "shadowfiend", 2) {}

    bool IsActive() override;
};

INTERRUPT_TRIGGER(SilenceInterruptSpellTrigger, "silence");
INTERRUPT_HEALER_TRIGGER(SilenceInterruptEnemyHealerSpellTrigger, "silence");

CURE_PARTY_TRIGGER(PurifyMagicOnPartyTrigger, "purify", DISPEL_MAGIC);
CURE_PARTY_TRIGGER(PurifyDiseaseOnPartyTrigger, "purify", DISPEL_DISEASE);

#endif
