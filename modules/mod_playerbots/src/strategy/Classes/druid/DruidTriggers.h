/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DRUIDTRIGGERS_H
#define _PLAYERBOT_DRUIDTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"

class PlayerbotAI;

// -- forms and buffs
BUFF_TRIGGER(CatFormTrigger, "cat form");
BUFF_TRIGGER(BearFormTrigger, "bear form");
BUFF_TRIGGER(MoonkinFormTrigger, "moonkin form");

// In cat, bear or travel form: casters leave it to cast
class InAnimalFormTrigger : public Trigger
{
public:
    InAnimalFormTrigger(PlayerbotAI* botAI) : Trigger(botAI, "in animal form") {}

    bool IsActive() override;
};

// No stats buff of any kind (Mark of the Wild, Kings, Legacy of the Emperor, Embrace of the Shale Spider)
class MarkOfTheWildTrigger : public Trigger
{
public:
    MarkOfTheWildTrigger(PlayerbotAI* botAI) : Trigger(botAI, "mark of the wild", 2) {}

    bool IsActive() override;
};

// -- balance
DEBUFF_CHECKISOWNER_TRIGGER(MoonfireTrigger, "moonfire");
DEBUFF_CHECKISOWNER_TRIGGER(SunfireTrigger, "sunfire");
HAS_AURA_TRIGGER(ShootingStarsTrigger, "shooting stars");
CAN_CAST_TRIGGER(StarfallTrigger, "starfall");
CAN_CAST_TRIGGER(CelestialAlignmentTrigger, "celestial alignment");

// Starfire rather than Wrath: in Lunar Eclipse, or on the way up to Solar (Starfire moves the bar towards it)
class StarfirePhaseTrigger : public Trigger
{
public:
    StarfirePhaseTrigger(PlayerbotAI* botAI) : Trigger(botAI, "starfire phase") {}

    bool IsActive() override;
};

// -- feral
// Five combo points on the current target
class DruidFinisherTrigger : public Trigger
{
public:
    DruidFinisherTrigger(PlayerbotAI* botAI, std::string const name = "druid finisher") : Trigger(botAI, name) {}

    bool IsActive() override;
};

// Five combo points and our Rip missing on a target that will live long enough
class RipTrigger : public DruidFinisherTrigger
{
public:
    RipTrigger(PlayerbotAI* botAI) : DruidFinisherTrigger(botAI, "rip") {}

    bool IsActive() override;
};

// Savage Roar is missing or about to end, and there is a combo point to renew it with
class SavageRoarTrigger : public Trigger
{
public:
    SavageRoarTrigger(PlayerbotAI* botAI) : Trigger(botAI, "savage roar") {}

    bool IsActive() override;
};

// Tiger's Fury is ready and energy is low
class TigersFuryTrigger : public SpellCanBeCastTrigger
{
public:
    TigersFuryTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "tiger's fury") {}

    bool IsActive() override;
};

DEBUFF_CHECKISOWNER_TRIGGER(RakeTrigger, "rake");
DEBUFF_TRIGGER(FaerieFireTrigger, "faerie fire");
CAN_CAST_TRIGGER(BerserkTrigger, "berserk");
INTERRUPT_TRIGGER(SkullBashInterruptSpellTrigger, "skull bash");
INTERRUPT_HEALER_TRIGGER(SkullBashInterruptEnemyHealerSpellTrigger, "skull bash");

// -- guardian
DEBUFF_CHECKISOWNER_TRIGGER(LacerateTrigger, "lacerate");
DEBUFF_CHECKISOWNER_TRIGGER(ThrashTrigger, "thrash");

// Rage above an amount (60 for Savage Defense, 80 to dump into Maul)
class DruidRageTrigger : public Trigger
{
public:
    DruidRageTrigger(PlayerbotAI* botAI, std::string const name, uint32 amount) : Trigger(botAI, name), amount(amount) {}

    bool IsActive() override;

private:
    uint32 amount;
};

// -- restoration
class LifebloomOnMainTankTrigger : public BuffOnMainTankTrigger
{
public:
    LifebloomOnMainTankTrigger(PlayerbotAI* botAI) : BuffOnMainTankTrigger(botAI, "lifebloom", true) {}
};

CURE_PARTY_TRIGGER(NaturesCureMagicOnPartyTrigger, "nature's cure", DISPEL_MAGIC);
CURE_PARTY_TRIGGER(NaturesCureCurseOnPartyTrigger, "nature's cure", DISPEL_CURSE);
CURE_PARTY_TRIGGER(NaturesCurePoisonOnPartyTrigger, "nature's cure", DISPEL_POISON);
CURE_PARTY_TRIGGER(RemoveCorruptionCurseOnPartyTrigger, "remove corruption", DISPEL_CURSE);
CURE_PARTY_TRIGGER(RemoveCorruptionPoisonOnPartyTrigger, "remove corruption", DISPEL_POISON);

#endif
