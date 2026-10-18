/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PALADINTRIGGERS_H
#define _PLAYERBOT_PALADINTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

// -- seals and self buffs
BUFF_TRIGGER(SealOfTruthTrigger, "seal of truth");
BUFF_TRIGGER(SealOfInsightTrigger, "seal of insight");
BUFF_TRIGGER(RighteousFuryTrigger, "righteous fury");
BUFF_TRIGGER(InquisitionTrigger, "inquisition");
BUFF_TRIGGER(SacredShieldTrigger, "sacred shield");

// No stats buff of any kind (Kings, Mark of the Wild, Legacy of the Emperor, Embrace of the Shale Spider)
class BlessingOfKingsTrigger : public Trigger
{
public:
    BlessingOfKingsTrigger(PlayerbotAI* botAI) : Trigger(botAI, "blessing of kings", 2) {}

    bool IsActive() override;
};

// Someone else covers the stats buff and nothing gives mastery yet
class BlessingOfMightTrigger : public Trigger
{
public:
    BlessingOfMightTrigger(PlayerbotAI* botAI) : Trigger(botAI, "blessing of might", 2) {}

    bool IsActive() override;
};

// -- procs
HAS_AURA_TRIGGER(GrandCrusaderTrigger, "grand crusader");
HAS_AURA_TRIGGER(DivinePurposeTrigger, "divine purpose");

// -- cooldowns and talents
CAN_CAST_TRIGGER(AvengingWrathTrigger, "avenging wrath");
CAN_CAST_TRIGGER(GuardianOfAncientKingsTrigger, "guardian of ancient kings");
CAN_CAST_TRIGGER(ExecutionSentenceTrigger, "execution sentence");
CAN_CAST_TRIGGER(HolyPrismTrigger, "holy prism");
CAN_CAST_TRIGGER(LightsHammerTrigger, "light's hammer");

class BeaconOfLightOnMainTankTrigger : public BuffOnMainTankTrigger
{
public:
    BeaconOfLightOnMainTankTrigger(PlayerbotAI* botAI) : BuffOnMainTankTrigger(botAI, "beacon of light", true) {}
};

INTERRUPT_TRIGGER(RebukeInterruptSpellTrigger, "rebuke");
INTERRUPT_HEALER_TRIGGER(RebukeInterruptEnemyHealerSpellTrigger, "rebuke");
INTERRUPT_HEALER_TRIGGER(HammerOfJusticeEnemyHealerTrigger, "hammer of justice");

CURE_PARTY_TRIGGER(CleansePoisonOnPartyTrigger, "cleanse", DISPEL_POISON);
CURE_PARTY_TRIGGER(CleanseDiseaseOnPartyTrigger, "cleanse", DISPEL_DISEASE);
CURE_PARTY_TRIGGER(CleanseMagicOnPartyTrigger, "cleanse", DISPEL_MAGIC);

#endif
