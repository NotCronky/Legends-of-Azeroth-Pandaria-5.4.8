/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MONKACTIONS_H
#define _PLAYERBOT_MONKACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- stances and buffs (Legacy of the Emperor covers the whole group)
BUFF_ACTION(CastStanceOfTheSturdyOxAction, "stance of the sturdy ox");
BUFF_ACTION(CastStanceOfTheFierceTigerAction, "stance of the fierce tiger");
BUFF_ACTION(CastStanceOfTheWiseSerpentAction, "stance of the wise serpent");
BUFF_ACTION(CastLegacyOfTheEmperorAction, "legacy of the emperor");

// -- chi builders and spenders
MELEE_ACTION(CastJabAction, "jab");
SPELL_ACTION(CastExpelHarmAction, "expel harm");
MELEE_ACTION(CastTigerPalmAction, "tiger palm");
MELEE_ACTION(CastBlackoutKickAction, "blackout kick");
MELEE_ACTION(CastRisingSunKickAction, "rising sun kick");
MELEE_ACTION(CastFistsOfFuryAction, "fists of fury");
MELEE_ACTION(CastTouchOfDeathAction, "touch of death");
SPELL_ACTION(CastSpinningCraneKickAction, "spinning crane kick");
SPELL_ACTION(CastChiWaveAction, "chi wave");
MELEE_ACTION(CastSpearHandStrikeAction, "spear hand strike");
ENEMY_HEALER_ACTION(CastSpearHandStrikeOnEnemyHealerAction, "spear hand strike");

// -- brewmaster
MELEE_ACTION(CastKegSmashAction, "keg smash");
SPELL_ACTION(CastBreathOfFireAction, "breath of fire");
BUFF_ACTION(CastPurifyingBrewAction, "purifying brew");
BUFF_ACTION(CastGuardAction, "guard");
SPELL_ACTION(CastProvokeAction, "provoke");

// -- windwalker
BUFF_ACTION(CastEnergizingBrewAction, "energizing brew");
SPELL_ACTION(CastInvokeXuenAction, "invoke xuen, the white tiger");

// -- survival
BUFF_ACTION(CastFortifyingBrewAction, "fortifying brew");

// -- mistweaver
HEAL_PARTY_ACTION(CastRenewingMistOnPartyAction, "renewing mist", 10.0f, HealingManaEfficiency::VERY_HIGH);
HEAL_PARTY_ACTION(CastEnvelopingMistOnPartyAction, "enveloping mist", 25.0f, HealingManaEfficiency::HIGH);
HEAL_PARTY_ACTION(CastSurgingMistOnPartyAction, "surging mist", 25.0f, HealingManaEfficiency::LOW);
HEAL_PARTY_ACTION(CastLifeCocoonOnPartyAction, "life cocoon", 40.0f, HealingManaEfficiency::SUPERIOR);
SPELL_ACTION(CastUpliftAction, "uplift");
BUFF_ACTION(CastRevivalAction, "revival");
HEAL_ACTION(CastSurgingMistAction, "surging mist");

// -- cure and resurrect
CURE_PARTY_ACTION(CastDetoxMagicOnPartyAction, "detox", DISPEL_MAGIC);
CURE_PARTY_ACTION(CastDetoxPoisonOnPartyAction, "detox", DISPEL_POISON);
CURE_PARTY_ACTION(CastDetoxDiseaseOnPartyAction, "detox", DISPEL_DISEASE);
RESS_ACTION(CastResuscitateAction, "resuscitate");

#endif
