/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PRIESTACTIONS_H
#define _PLAYERBOT_PRIESTACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- buffs (Power Word: Fortitude covers the whole group)
BUFF_ACTION(CastPowerWordFortitudeAction, "power word: fortitude");
BUFF_ACTION(CastInnerFireAction, "inner fire");
BUFF_ACTION(CastShadowformAction, "shadowform");
BUFF_ACTION(CastChakraSerenityAction, "chakra: serenity");
BUFF_ACTION(CastFadeAction, "fade");

// -- shadow
DEBUFF_CHECKISOWNER_ACTION(CastShadowWordPainAction, "shadow word: pain");
DEBUFF_CHECKISOWNER_ACTION(CastVampiricTouchAction, "vampiric touch");
SPELL_ACTION(CastDevouringPlagueAction, "devouring plague");
SPELL_ACTION(CastMindBlastAction, "mind blast");
SPELL_ACTION(CastShadowWordDeathAction, "shadow word: death");
SPELL_ACTION(CastMindFlayAction, "mind flay");
SPELL_ACTION(CastMindSpikeAction, "mind spike");
SPELL_ACTION(CastMindSearAction, "mind sear");
SPELL_ACTION(CastShadowfiendAction, "shadowfiend");
SPELL_ACTION(CastMindbenderAction, "mindbender");
BUFF_ACTION(CastDispersionAction, "dispersion");
SPELL_ACTION(CastSilenceAction, "silence");
ENEMY_HEALER_ACTION(CastSilenceOnEnemyHealerAction, "silence");

// -- holy and discipline damage
SPELL_ACTION(CastSmiteAction, "smite");
SPELL_ACTION(CastHolyFireAction, "holy fire");
SPELL_ACTION(CastPenanceAction, "penance");
SPELL_ACTION(CastHaloAction, "halo");

// Power Word: Shield, but not on someone with Weakened Soul (a shield in the last 15 seconds)
class CastPowerWordShieldOnPartyAction : public HealPartyMemberAction
{
public:
    CastPowerWordShieldOnPartyAction(PlayerbotAI* botAI)
        : HealPartyMemberAction(botAI, "power word: shield", 15.0f, HealingManaEfficiency::HIGH)
    {
    }

    bool isUseful() override;
};

class CastPowerWordShieldAction : public CastBuffSpellAction
{
public:
    CastPowerWordShieldAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "power word: shield") {}

    bool isUseful() override;
};

// -- heals
HEAL_PARTY_ACTION(CastPenanceOnPartyAction, "penance", 25.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastFlashHealOnPartyAction, "flash heal", 25.0f, HealingManaEfficiency::LOW);
HEAL_PARTY_ACTION(CastGreaterHealOnPartyAction, "greater heal", 35.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastHealOnPartyAction, "heal", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastRenewOnPartyAction, "renew", 10.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastPrayerOfMendingOnPartyAction, "prayer of mending", 15.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastCircleOfHealingOnPartyAction, "circle of healing", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastPrayerOfHealingOnPartyAction, "prayer of healing", 20.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastPainSuppressionOnPartyAction, "pain suppression", 40.0f, HealingManaEfficiency::SUPERIOR);
HEAL_PARTY_ACTION(CastGuardianSpiritOnPartyAction, "guardian spirit", 40.0f, HealingManaEfficiency::SUPERIOR);
BUFF_ACTION(CastDivineHymnAction, "divine hymn");
BUFF_ACTION(CastHymnOfHopeAction, "hymn of hope");
HEAL_ACTION(CastFlashHealAction, "flash heal");

// -- cure and resurrect
CURE_PARTY_ACTION(CastPurifyMagicOnPartyAction, "purify", DISPEL_MAGIC);
CURE_PARTY_ACTION(CastPurifyDiseaseOnPartyAction, "purify", DISPEL_DISEASE);
RESS_ACTION(CastResurrectionAction, "resurrection");

#endif
