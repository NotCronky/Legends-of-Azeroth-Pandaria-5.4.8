/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_LOOTACTIONS_H
#define _PLAYERBOT_LOOTACTIONS_H

#include "InventoryAction.h"
#include "MovementActions.h"

class GameObject;
class LootObject;
class PlayerbotAI;

struct ItemTemplate;

// Pick the nearest lootable corpse or object as the loot target
class LootAction : public MovementAction
{
public:
    LootAction(PlayerbotAI* botAI) : MovementAction(botAI, "loot") {}

    bool Execute(Event event) override;
    bool isUseful() override;
};

class MoveToLootAction : public MovementAction
{
public:
    MoveToLootAction(PlayerbotAI* botAI) : MovementAction(botAI, "move to loot") {}

    bool Execute(Event event) override;
};

// Open the loot target: loot a corpse, skin it, open a chest or gather a node. The items are taken when the
// loot window opens ("store loot")
class OpenLootAction : public MovementAction
{
public:
    OpenLootAction(PlayerbotAI* botAI) : MovementAction(botAI, "open loot") {}

    bool Execute(Event event) override;

private:
    bool DoLoot(LootObject& lootObject);
    uint32 FindSpell(uint32 effect, uint32 miscValue, uint32 skill);
};

// Take the money and the items the loot strategy allows from every open loot window, then close them
class StoreLootAction : public InventoryAction
{
public:
    StoreLootAction(PlayerbotAI* botAI) : InventoryAction(botAI, "store loot") {}

    bool Execute(Event event) override;
    static bool IsLootAllowed(uint32 itemId, PlayerbotAI* botAI);
};

// Remember nearby corpses and objects so "loot" can choose among them
class AddAllLootAction : public Action
{
public:
    AddAllLootAction(PlayerbotAI* botAI, std::string const name = "add all loot") : Action(botAI, name) {}

    bool Execute(Event event) override;

protected:
    virtual bool AddLoot(ObjectGuid guid);
};

// Only nodes and corpses the bot can gather from (herbalism, mining, skinning)
class AddGatheringLootAction : public AddAllLootAction
{
public:
    AddGatheringLootAction(PlayerbotAI* botAI) : AddAllLootAction(botAI, "add gathering loot") {}

protected:
    bool AddLoot(ObjectGuid guid) override;
};

// Need, greed or pass on the items up for a group roll
class LootRollAction : public Action
{
public:
    LootRollAction(PlayerbotAI* botAI) : Action(botAI, "loot roll") {}

    bool Execute(Event event) override;

private:
    uint8 CalculateRollVote(ItemTemplate const* proto, int32 randomPropertyId);
};

// "ll [normal|gray|disenchant|all|?]", "ll <item>", "ll -<item>", "ll ?<item>": loot strategy and always-loot list
class LootStrategyAction : public Action
{
public:
    LootStrategyAction(PlayerbotAI* botAI) : Action(botAI, "ll") {}

    bool Execute(Event event) override;
};

#endif
