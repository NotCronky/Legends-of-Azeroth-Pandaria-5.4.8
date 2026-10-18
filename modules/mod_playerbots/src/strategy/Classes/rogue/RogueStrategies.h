/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_ROGUESTRATEGIES_H
#define _PLAYERBOT_ROGUESTRATEGIES_H

#include "MeleeCombatStrategy.h"

class PlayerbotAI;

// What every rogue does in combat: open from stealth, keep Slice and Dice up, spend five combo points,
// interrupt and survive
class GenericRogueStrategy : public MeleeCombatStrategy
{
public:
    GenericRogueStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
};

// Mutilate (Dispatch below 35% and on Blindside), Rupture, Envenom and Vendetta
class AssassinationRogueStrategy : public GenericRogueStrategy
{
public:
    AssassinationRogueStrategy(PlayerbotAI* botAI) : GenericRogueStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "assassination"; }
};

// Sinister Strike, Revealing Strike, Eviscerate, Adrenaline Rush, Killing Spree and Blade Flurry on packs
class CombatRogueStrategy : public GenericRogueStrategy
{
public:
    CombatRogueStrategy(PlayerbotAI* botAI) : GenericRogueStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "combat"; }
};

// Backstab from behind, Hemorrhage otherwise and to keep its bleed up, Rupture, Eviscerate and Shadow Dance
class SubtletyRogueStrategy : public GenericRogueStrategy
{
public:
    SubtletyRogueStrategy(PlayerbotAI* botAI) : GenericRogueStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "subtlety"; }
};

// Shadow Blades when it is ready
class RogueBoostStrategy : public Strategy
{
public:
    RogueBoostStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "boost"; }
};

// Fan of Knives on packs
class RogueAoeStrategy : public CombatStrategy
{
public:
    RogueAoeStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "aoe"; }
};

#endif
