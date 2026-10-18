/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DRUIDSTRATEGIES_H
#define _PLAYERBOT_DRUIDSTRATEGIES_H

#include "MeleeCombatStrategy.h"
#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Balance: Moonkin Form, Moonfire and Sunfire, Starsurge (on Shooting Stars too), Starfire or Wrath by the
// Eclipse bar, Starfall and Celestial Alignment; Hurricane on packs
class BalanceDruidStrategy : public RangedCombatStrategy
{
public:
    BalanceDruidStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "caster"; }
};

// Feral: Cat Form, Savage Roar, Rake, Rip and Ferocious Bite at five combo points, Shred (Mangle when not
// behind), Tiger's Fury, Berserk, Faerie Fire; Thrash and Swipe on packs
class FeralDruidStrategy : public MeleeCombatStrategy
{
public:
    FeralDruidStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "cat"; }
};

// Guardian tank: Bear Form, Mangle, Thrash, Lacerate, Maul for spare rage, Savage Defense, Frenzied
// Regeneration, Survival Instincts, Might of Ursoc, Growl when a mob gets away
class GuardianDruidStrategy : public MeleeCombatStrategy
{
public:
    GuardianDruidStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_TANK | STRATEGY_TYPE_MELEE; }
    std::string const getName() override { return "bear"; }
};

// Restoration healer: Lifebloom on the main tank, Rejuvenation, Swiftmend, Regrowth, Healing Touch and Nourish
// by need, Wild Growth and Tranquility for the group, Ironbark, Innervate
class RestorationDruidStrategy : public RangedCombatStrategy
{
public:
    RestorationDruidStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_RANGED; }
    std::string const getName() override { return "heal"; }
};

// Nature's Cure (Restoration: magic, curses, poisons) or Remove Corruption (curses, poisons)
class DruidCureStrategy : public Strategy
{
public:
    DruidCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
