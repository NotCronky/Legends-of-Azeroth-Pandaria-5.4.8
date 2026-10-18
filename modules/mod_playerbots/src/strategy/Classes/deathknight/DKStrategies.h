/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DKSTRATEGIES_H
#define _PLAYERBOT_DKSTRATEGIES_H

#include "MeleeCombatStrategy.h"

class PlayerbotAI;

// What every death knight does in combat: keep both diseases up (Outbreak, else Icy Touch and Plague Strike),
// Soul Reaper below 35%, Mind Freeze, Empower Rune Weapon when out of runes, Blood Tap, and survive
class GenericDKStrategy : public MeleeCombatStrategy
{
public:
    GenericDKStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
};

// Two-handed Frost: Obliterate (on Killing Machine first), Howling Blast (free on Rime), Frost Strike to spend
// runic power, Pillar of Frost
class FrostDKStrategy : public GenericDKStrategy
{
public:
    FrostDKStrategy(PlayerbotAI* botAI) : GenericDKStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "frost"; }
};

// Scourge Strike, Festering Strike, Death Coil (on Sudden Doom), Dark Transformation at five Shadow Infusion
// stacks, Summon Gargoyle and Unholy Frenzy; the ghoul is a pet
class UnholyDKStrategy : public GenericDKStrategy
{
public:
    UnholyDKStrategy(PlayerbotAI* botAI) : GenericDKStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "unholy"; }
};

// Tank: Bone Shield, Death Strike, Heart Strike, Blood Boil on Crimson Scourge, Rune Strike for runic power,
// Dancing Rune Weapon, Vampiric Blood and Rune Tap, Dark Command when a mob gets away
class BloodDKStrategy : public GenericDKStrategy
{
public:
    BloodDKStrategy(PlayerbotAI* botAI) : GenericDKStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_TANK | STRATEGY_TYPE_MELEE; }
    std::string const getName() override { return "blood"; }
};

// Death and Decay and Blood Boil on packs
class DKAoeStrategy : public CombatStrategy
{
public:
    DKAoeStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "aoe"; }
};

#endif
