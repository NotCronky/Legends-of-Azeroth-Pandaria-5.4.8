/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_HUNTERSTRATEGIES_H
#define _PLAYERBOT_HUNTERSTRATEGIES_H

#include "RangedCombatStrategy.h"

class PlayerbotAI;

// What every hunter does in combat: shoot, keep an aspect, the pet and Serpent Sting up, Kill Shot,
// interrupt, slow and survive
class GenericHunterStrategy : public RangedCombatStrategy
{
public:
    GenericHunterStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
};

// Kill Command, Bestial Wrath and Focus Fire; Cobra Shot fills
class BeastMasteryHunterStrategy : public GenericHunterStrategy
{
public:
    BeastMasteryHunterStrategy(PlayerbotAI* botAI) : GenericHunterStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "bm"; }
};

// Chimera Shot, Aimed Shot on Fire! procs; Steady Shot fills
class MarksmanshipHunterStrategy : public GenericHunterStrategy
{
public:
    MarksmanshipHunterStrategy(PlayerbotAI* botAI) : GenericHunterStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "mm"; }
};

// Explosive Shot (with Lock and Load) and Black Arrow; Cobra Shot fills
class SurvivalHunterStrategy : public GenericHunterStrategy
{
public:
    SurvivalHunterStrategy(PlayerbotAI* botAI) : GenericHunterStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    std::string const getName() override { return "sv"; }
};

// Rapid Fire and Stampede when they are ready
class HunterBoostStrategy : public Strategy
{
public:
    HunterBoostStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "boost"; }
};

// Multi-Shot and the AoE talents on packs
class HunterAoeStrategy : public CombatStrategy
{
public:
    HunterAoeStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "aoe"; }
};

#endif
