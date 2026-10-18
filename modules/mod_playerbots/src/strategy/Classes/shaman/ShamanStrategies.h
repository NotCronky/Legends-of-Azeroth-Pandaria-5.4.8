/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SHAMANSTRATEGIES_H
#define _PLAYERBOT_SHAMANSTRATEGIES_H

#include "MeleeCombatStrategy.h"
#include "RangedCombatStrategy.h"

class PlayerbotAI;

// Elemental: Lightning Shield, Flametongue Weapon, Flame Shock, Lava Burst (on Lava Surge), Earth Shock at seven
// Lightning Shield charges, Lightning Bolt; Searing Totem, Elemental Mastery, Ascendance, Fire Elemental;
// Earthquake and Chain Lightning on packs
class ElementalShamanStrategy : public RangedCombatStrategy
{
public:
    ElementalShamanStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return RangedCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "caster"; }
};

// Enhancement: Lightning Shield, Windfury Weapon, Stormstrike, Lava Lash, Unleash Elements, Flame Shock, an
// instant Lightning Bolt at five Maelstrom Weapon stacks; Searing Totem, Feral Spirit, Ascendance; Magma
// Totem and Fire Nova on packs
class EnhancementShamanStrategy : public MeleeCombatStrategy
{
public:
    EnhancementShamanStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return MeleeCombatStrategy::GetType() | STRATEGY_TYPE_DPS; }
    std::string const getName() override { return "melee"; }
};

// Restoration healer: Water Shield, Earthliving Weapon, Earth Shield on the main tank, Riptide, Healing Surge,
// Greater Healing Wave and Healing Wave by need, Chain Heal and Healing Rain for the group, Healing Stream and
// Healing Tide Totems, Mana Tide Totem
class RestorationShamanStrategy : public RangedCombatStrategy
{
public:
    RestorationShamanStrategy(PlayerbotAI* botAI);

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_HEAL | STRATEGY_TYPE_RANGED; }
    std::string const getName() override { return "heal"; }
};

// Cleanse Spirit (curses), or Purify Spirit (curses and magic) for Restoration
class ShamanCureStrategy : public Strategy
{
public:
    ShamanCureStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "cure"; }
};

#endif
