/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MAGEACTIONS_H
#define _PLAYERBOT_MAGEACTIONS_H

#include "GenericSpellActions.h"

class PlayerbotAI;

// -- buffs (Arcane Brilliance covers the whole group)
BUFF_ACTION(CastArcaneBrillianceAction, "arcane brilliance");
BUFF_ACTION(CastMageArmorAction, "mage armor");
BUFF_ACTION(CastMoltenArmorAction, "molten armor");
BUFF_ACTION(CastFrostArmorAction, "frost armor");

// -- arcane
SPELL_ACTION(CastArcaneBlastAction, "arcane blast");
SPELL_ACTION(CastArcaneMissilesAction, "arcane missiles");
SPELL_ACTION(CastArcaneBarrageAction, "arcane barrage");
DEBUFF_CHECKISOWNER_ACTION(CastNetherTempestAction, "nether tempest");
BUFF_ACTION(CastArcanePowerAction, "arcane power");
SPELL_ACTION(CastArcaneExplosionAction, "arcane explosion");

// -- fire
SPELL_ACTION(CastFireballAction, "fireball");
SPELL_ACTION(CastPyroblastAction, "pyroblast");
SPELL_ACTION(CastInfernoBlastAction, "inferno blast");
DEBUFF_CHECKISOWNER_ACTION(CastLivingBombAction, "living bomb");
SPELL_ACTION(CastCombustionAction, "combustion");
SPELL_ACTION(CastScorchAction, "scorch");
SPELL_ACTION(CastFlamestrikeAction, "flamestrike");

// -- frost
SPELL_ACTION(CastFrostboltAction, "frostbolt");
SPELL_ACTION(CastIceLanceAction, "ice lance");
SPELL_ACTION(CastFrostfireBoltAction, "frostfire bolt");
SPELL_ACTION(CastFrozenOrbAction, "frozen orb");
BUFF_ACTION(CastIcyVeinsAction, "icy veins");
DEBUFF_CHECKISOWNER_ACTION(CastFrostBombAction, "frost bomb");
SPELL_ACTION(CastBlizzardAction, "blizzard");
BUFF_ACTION(CastSummonWaterElementalAction, "summon water elemental");

// -- every specialization
SPELL_ACTION(CastCounterspellAction, "counterspell");
ENEMY_HEALER_ACTION(CastCounterspellOnEnemyHealerAction, "counterspell");
BUFF_ACTION(CastIceBlockAction, "ice block");
BUFF_ACTION(CastEvocationAction, "evocation");
BUFF_ACTION(CastMirrorImageAction, "mirror image");
CURE_PARTY_ACTION(CastRemoveCurseOnPartyAction, "remove curse", DISPEL_CURSE);

// Frost Nova when an enemy is in melee range
class CastFrostNovaAction : public CastSpellAction
{
public:
    CastFrostNovaAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "frost nova") {}

    bool isUseful() override;
};

// Dragon's Breath, a cone, on an enemy close in front
class CastDragonsBreathAction : public CastSpellAction
{
public:
    CastDragonsBreathAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "dragon's breath") {}

    bool isUseful() override;
};

#endif
