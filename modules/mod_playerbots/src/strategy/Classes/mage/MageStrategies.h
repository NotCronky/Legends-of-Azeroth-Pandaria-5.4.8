/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MAGESTRATEGIES_H
#define _PLAYERBOT_MAGESTRATEGIES_H

#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Arcane: Mage Armor, Arcane Blast, Arcane Missiles on its proc, Arcane Barrage at four charges when mana is low,
// Nether Tempest, Arcane Power; Arcane Explosion on packs
class ArcaneMageStrategy : public RangedCombatStrategy
{
public:
    ArcaneMageStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "arcane"; }
};

// Fire: Molten Armor, Fireball, Pyroblast on Pyroblast!, Inferno Blast on Heating Up, Living Bomb, Combustion;
// Flamestrike and Dragon's Breath on packs
class FireMageStrategy : public RangedCombatStrategy
{
public:
    FireMageStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "fire"; }
};

// Frost: Frost Armor, Water Elemental, Frostbolt, Ice Lance on Fingers of Frost, Frostfire Bolt on Brain Freeze,
// Frozen Orb, Icy Veins, Frost Bomb; Blizzard on packs
class FrostMageStrategy : public RangedCombatStrategy
{
public:
    FrostMageStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "frost"; }
};

// Remove Curse in the group
class MageCureStrategy : public Strategy
{
public:
    MageCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
