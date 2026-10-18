/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PALADINSTRATEGIES_H
#define _PLAYERBOT_PALADINSTRATEGIES_H

#include "MeleeCombatStrategy.h"
#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Retribution: Seal of Truth, Inquisition, Templar's Verdict, Hammer of Wrath, Exorcism, Crusader Strike,
// Judgment; Avenging Wrath, Guardian of Ancient Kings and Execution Sentence; Divine Storm on packs
class RetributionPaladinStrategy : public MeleeCombatStrategy
{
public:
    RetributionPaladinStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "dps"; }
};

// Protection tank: Righteous Fury, Seal of Insight, Shield of the Righteous, Avenger's Shield (on Grand
// Crusader), Crusader Strike, Judgment, Consecration, Word of Glory on itself, Ardent Defender, Reckoning
class ProtectionPaladinStrategy : public MeleeCombatStrategy
{
public:
    ProtectionPaladinStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_TANK | STRATEGY_TYPE_MELEE; }
    std::string const getName() override { return "tank"; }
};

// Holy healer: Beacon of Light on the main tank, Holy Shock, Word of Glory and Light of Dawn with holy power,
// Flash of Light, Holy Light and Divine Light by need, Holy Radiance for the group, Divine Plea for mana
class HolyPaladinStrategy : public RangedCombatStrategy
{
public:
    HolyPaladinStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_RANGED; }
    std::string const getName() override { return "heal"; }
};

// Cleanse poisons, diseases and (Holy) magic in the group
class PaladinCureStrategy : public Strategy
{
public:
    PaladinCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
