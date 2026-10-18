/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_MONKSTRATEGIES_H
#define _PLAYERBOT_MONKSTRATEGIES_H

#include "MeleeCombatStrategy.h"

class PlayerbotAI;

// Brewmaster tank: Stance of the Sturdy Ox, Keg Smash, Blackout Kick for Shuffle, Tiger Palm, Jab and Expel Harm
// for chi, Purifying Brew on heavy Stagger, Guard, Breath of Fire on packs, Provoke, Fortifying Brew
class BrewmasterMonkStrategy : public MeleeCombatStrategy
{
public:
    BrewmasterMonkStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_TANK | STRATEGY_TYPE_MELEE; }
    std::string const getName() override { return "tank"; }
};

// Windwalker: Stance of the Fierce Tiger, Tiger Palm for Tiger Power, Rising Sun Kick, Fists of Fury, Blackout
// Kick, Jab, Combo Breaker procs, Energizing Brew, Invoke Xuen; Spinning Crane Kick on packs
class WindwalkerMonkStrategy : public MeleeCombatStrategy
{
public:
    WindwalkerMonkStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "melee"; }
};

// Mistweaver healer: Stance of the Wise Serpent, Renewing Mist, Enveloping Mist and Surging Mist by need, Uplift
// and Revival for the group, Life Cocoon; Jab and Tiger Palm with nothing to heal
class MistweaverMonkStrategy : public MeleeCombatStrategy
{
public:
    MistweaverMonkStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_MELEE; }
    std::string const getName() override { return "heal"; }
};

// Detox magic (Mistweaver), poisons and diseases in the group
class MonkCureStrategy : public Strategy
{
public:
    MonkCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
