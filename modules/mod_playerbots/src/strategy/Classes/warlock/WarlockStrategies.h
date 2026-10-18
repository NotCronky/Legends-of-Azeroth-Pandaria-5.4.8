/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_WARLOCKSTRATEGIES_H
#define _PLAYERBOT_WARLOCKSTRATEGIES_H

#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Affliction: Agony, Corruption and Unstable Affliction kept up, Haunt, Malefic Grasp, Drain Soul below 20%,
// Dark Soul: Misery; Seed of Corruption on packs
class AfflictionWarlockStrategy : public RangedCombatStrategy
{
public:
    AfflictionWarlockStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "affli"; }
};

// Demonology: Felguard, Corruption, Hand of Gul'dan, Soul Fire on Molten Core, Shadow Bolt, Dark Soul:
// Knowledge (without Metamorphosis); Seed of Corruption on packs
class DemonologyWarlockStrategy : public RangedCombatStrategy
{
public:
    DemonologyWarlockStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "demono"; }
};

// Destruction: Immolate, Conflagrate, Chaos Bolt at three Burning Embers, Shadowburn below 20%, Incinerate,
// Dark Soul: Instability; Rain of Fire on packs
class DestructionWarlockStrategy : public RangedCombatStrategy
{
public:
    DestructionWarlockStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "destru"; }
};

#endif
