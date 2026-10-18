/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PRIESTSTRATEGIES_H
#define _PLAYERBOT_PRIESTSTRATEGIES_H

#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Shadow: Shadowform, Shadow Word: Pain and Vampiric Touch, Devouring Plague at three Shadow Orbs, Mind Blast,
// Shadow Word: Death below 20%, Mind Flay, Mind Spike on Surge of Darkness, Mindbender, Halo; Mind Sear on packs
class ShadowPriestStrategy : public RangedCombatStrategy
{
public:
    ShadowPriestStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "dps"; }
};

// Discipline healer: Power Word: Shield, Penance, Flash Heal, Greater Heal and Heal by need, Prayer of Mending,
// Prayer of Healing and Halo for the group, Pain Suppression
class DisciplinePriestStrategy : public RangedCombatStrategy
{
public:
    DisciplinePriestStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_RANGED; }
    std::string const getName() override { return "heal"; }
};

// Holy healer: Chakra: Serenity, Renew, Prayer of Mending, Flash Heal, Greater Heal and Heal
// by need, Circle of Healing, Prayer of Healing and Divine Hymn for the group, Guardian Spirit
class HolyPriestStrategy : public RangedCombatStrategy
{
public:
    HolyPriestStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_RANGED; }
    std::string const getName() override { return "holy"; }
};

// Discipline and Holy without a group: Smite, Holy Fire, Penance, Mind Blast and Shadow Word: Pain
class HolyDpsPriestStrategy : public RangedCombatStrategy
{
public:
    HolyDpsPriestStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "holy dps"; }
};

// Purify magic and diseases in the group (Discipline and Holy)
class PriestCureStrategy : public Strategy
{
public:
    PriestCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
