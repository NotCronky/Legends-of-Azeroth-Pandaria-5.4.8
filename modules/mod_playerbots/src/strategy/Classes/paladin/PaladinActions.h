/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PALADINACTIONS_H
#define _PLAYERBOT_PALADINACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- seals, blessings and self buffs (a MoP blessing covers the whole group)
BUFF_ACTION(CastSealOfTruthAction, "seal of truth");
BUFF_ACTION(CastSealOfInsightAction, "seal of insight");
BUFF_ACTION(CastSealOfRighteousnessAction, "seal of righteousness");
BUFF_ACTION(CastBlessingOfKingsAction, "blessing of kings");
BUFF_ACTION(CastBlessingOfMightAction, "blessing of might");
BUFF_ACTION(CastRighteousFuryAction, "righteous fury");
BUFF_ACTION(CastInquisitionAction, "inquisition");
BUFF_ACTION(CastSacredShieldAction, "sacred shield");

// -- holy power builders and spenders
MELEE_ACTION(CastCrusaderStrikeAction, "crusader strike");
MELEE_ACTION(CastHammerOfTheRighteousAction, "hammer of the righteous");
SPELL_ACTION(CastJudgmentAction, "judgment");
SPELL_ACTION(CastExorcismAction, "exorcism");
SPELL_ACTION(CastHammerOfWrathAction, "hammer of wrath");
MELEE_ACTION(CastTemplarsVerdictAction, "templar's verdict");
MELEE_ACTION(CastDivineStormAction, "divine storm");
MELEE_ACTION(CastShieldOfTheRighteousAction, "shield of the righteous");
SPELL_ACTION(CastAvengersShieldAction, "avenger's shield");
SPELL_ACTION(CastConsecrationAction, "consecration");
SPELL_ACTION(CastHolyWrathAction, "holy wrath");

// -- level 90 talents and cooldowns
SPELL_ACTION(CastExecutionSentenceAction, "execution sentence");
SPELL_ACTION(CastHolyPrismAction, "holy prism");
SPELL_ACTION(CastLightsHammerAction, "light's hammer");
BUFF_ACTION(CastAvengingWrathAction, "avenging wrath");
BUFF_ACTION(CastGuardianOfAncientKingsAction, "guardian of ancient kings");
BUFF_ACTION(CastDivinePleaAction, "divine plea");
BUFF_ACTION(CastDivineFavorAction, "divine favor");

// -- survival and threat
BUFF_ACTION(CastArdentDefenderAction, "ardent defender");
BUFF_ACTION(CastDivineProtectionAction, "divine protection");
BUFF_ACTION(CastDivineShieldAction, "divine shield");
SPELL_ACTION(CastReckoningAction, "reckoning");
MELEE_ACTION(CastRebukeAction, "rebuke");
ENEMY_HEALER_ACTION(CastRebukeOnEnemyHealerAction, "rebuke");
ENEMY_HEALER_ACTION(CastHammerOfJusticeOnEnemyHealerAction, "hammer of justice");

// -- heals on the paladin
HEAL_ACTION(CastWordOfGloryAction, "word of glory");
HEAL_ACTION(CastEternalFlameAction, "eternal flame");
HEAL_ACTION(CastLayOnHandsAction, "lay on hands");
HEAL_ACTION(CastFlashOfLightAction, "flash of light");

// -- heals on the group
HEAL_PARTY_ACTION(CastHolyShockOnPartyAction, "holy shock", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastFlashOfLightOnPartyAction, "flash of light", 20.0f, HealingManaEfficiency::LOW);
HEAL_PARTY_ACTION(CastHolyLightOnPartyAction, "holy light", 15.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastDivineLightOnPartyAction, "divine light", 35.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastWordOfGloryOnPartyAction, "word of glory", 25.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastEternalFlameOnPartyAction, "eternal flame", 25.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastHolyRadianceOnPartyAction, "holy radiance", 20.0f, HealingManaEfficiency::MEDIUM);
HEAL_PARTY_ACTION(CastLayOnHandsOnPartyAction, "lay on hands", 100.0f, HealingManaEfficiency::SUPERIOR);
SPELL_ACTION(CastLightOfDawnAction, "light of dawn");

class CastBeaconOfLightOnMainTankAction : public BuffOnMainTankAction
{
public:
    CastBeaconOfLightOnMainTankAction(PlayerbotAI* botAI) : BuffOnMainTankAction(botAI, "beacon of light", true) {}
};

// -- cleanse and resurrect
CURE_PARTY_ACTION(CastCleansePoisonOnPartyAction, "cleanse", DISPEL_POISON);
CURE_PARTY_ACTION(CastCleanseDiseaseOnPartyAction, "cleanse", DISPEL_DISEASE);
CURE_PARTY_ACTION(CastCleanseMagicOnPartyAction, "cleanse", DISPEL_MAGIC);
RESS_ACTION(CastRedemptionAction, "redemption");

#endif
