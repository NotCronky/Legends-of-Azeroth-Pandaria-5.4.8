/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DRUIDACTIONS_H
#define _PLAYERBOT_DRUIDACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- forms
BUFF_ACTION(CastCatFormAction, "cat form");
BUFF_ACTION(CastBearFormAction, "bear form");
BUFF_ACTION(CastMoonkinFormAction, "moonkin form");

// Leave an animal form to cast (heals, Balance spells)
class CastCasterFormAction : public CastBuffSpellAction
{
public:
    CastCasterFormAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "caster form") {}

    bool isUseful() override;
    bool isPossible() override { return true; }
    bool Execute(Event event) override;
};

// -- buffs
BUFF_ACTION(CastMarkOfTheWildAction, "mark of the wild");
BUFF_ACTION(CastBarkskinAction, "barkskin");
BUFF_ACTION(CastInnervateAction, "innervate");

// -- balance
DEBUFF_CHECKISOWNER_ACTION(CastMoonfireAction, "moonfire");
DEBUFF_CHECKISOWNER_ACTION(CastSunfireAction, "sunfire");
SPELL_ACTION(CastWrathAction, "wrath");
SPELL_ACTION(CastStarfireAction, "starfire");
SPELL_ACTION(CastStarsurgeAction, "starsurge");
BUFF_ACTION(CastStarfallAction, "starfall");
BUFF_ACTION(CastCelestialAlignmentAction, "celestial alignment");
SPELL_ACTION(CastHurricaneAction, "hurricane");

// -- feral (cat)
MELEE_ACTION(CastShredAction, "shred");
MELEE_ACTION(CastMangleAction, "mangle");
DEBUFF_CHECKISOWNER_ACTION(CastRakeAction, "rake");
MELEE_ACTION(CastRipAction, "rip");
MELEE_ACTION(CastFerociousBiteAction, "ferocious bite");
BUFF_ACTION(CastSavageRoarAction, "savage roar");
BUFF_ACTION(CastTigersFuryAction, "tiger's fury");
BUFF_ACTION(CastBerserkAction, "berserk");
MELEE_ACTION(CastThrashAction, "thrash");
MELEE_ACTION(CastSwipeAction, "swipe");
DEBUFF_ACTION(CastFaerieFireAction, "faerie fire");
MELEE_ACTION(CastSkullBashAction, "skull bash");
ENEMY_HEALER_ACTION(CastSkullBashOnEnemyHealerAction, "skull bash");
BUFF_ACTION(CastSurvivalInstinctsAction, "survival instincts");

// -- guardian (bear)
MELEE_ACTION(CastMaulAction, "maul");
DEBUFF_CHECKISOWNER_ACTION(CastLacerateAction, "lacerate");
BUFF_ACTION(CastSavageDefenseAction, "savage defense");
BUFF_ACTION(CastFrenziedRegenerationAction, "frenzied regeneration");
BUFF_ACTION(CastMightOfUrsocAction, "might of ursoc");
SPELL_ACTION(CastGrowlAction, "growl");

// -- restoration
HEAL_PARTY_ACTION(CastRejuvenationOnPartyAction, "rejuvenation", 10.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastRegrowthOnPartyAction, "regrowth", 25.0f, HealingManaEfficiency::LOW);
HEAL_PARTY_ACTION(CastHealingTouchOnPartyAction, "healing touch", 35.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastNourishOnPartyAction, "nourish", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastSwiftmendOnPartyAction, "swiftmend", 25.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastWildGrowthOnPartyAction, "wild growth", 15.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastIronbarkOnPartyAction, "ironbark", 50.0f, HealingManaEfficiency::SUPERIOR);
BUFF_ACTION(CastTranquilityAction, "tranquility");
BUFF_ACTION(CastNaturesSwiftnessAction, "nature's swiftness");
HEAL_ACTION(CastRejuvenationAction, "rejuvenation");
HEAL_ACTION(CastHealingTouchAction, "healing touch");

class CastLifebloomOnMainTankAction : public BuffOnMainTankAction
{
public:
    CastLifebloomOnMainTankAction(PlayerbotAI* botAI) : BuffOnMainTankAction(botAI, "lifebloom", true) {}
};

// -- cure and resurrect
CURE_PARTY_ACTION(CastNaturesCureMagicOnPartyAction, "nature's cure", DISPEL_MAGIC);
CURE_PARTY_ACTION(CastNaturesCureCurseOnPartyAction, "nature's cure", DISPEL_CURSE);
CURE_PARTY_ACTION(CastNaturesCurePoisonOnPartyAction, "nature's cure", DISPEL_POISON);
CURE_PARTY_ACTION(CastRemoveCorruptionCurseOnPartyAction, "remove corruption", DISPEL_CURSE);
CURE_PARTY_ACTION(CastRemoveCorruptionPoisonOnPartyAction, "remove corruption", DISPEL_POISON);
RESS_ACTION(CastReviveAction, "revive");

#endif
