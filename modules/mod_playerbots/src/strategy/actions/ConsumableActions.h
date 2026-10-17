/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CONSUMABLEACTIONS_H
#define _PLAYERBOT_CONSUMABLEACTIONS_H

#include "UseItemAction.h"

class PlayerbotAI;

// Eat or drink out of combat. With the food cheat the bot casts the eating or drinking spell of the best food
// for its level without needing any; otherwise it uses the best it has in its bags
class EatDrinkAction : public UseItemAction
{
public:
    EatDrinkAction(PlayerbotAI* botAI, std::string const name, bool drink) : UseItemAction(botAI, name), drink(drink) {}

    bool Execute(Event event) override;
    bool isPossible() override;

protected:
    // Already eating or drinking
    bool IsConsuming();

private:
    bool drink;
};

class EatAction : public EatDrinkAction
{
public:
    EatAction(PlayerbotAI* botAI) : EatDrinkAction(botAI, "food", false) {}

    bool isUseful() override;
};

class DrinkAction : public EatDrinkAction
{
public:
    DrinkAction(PlayerbotAI* botAI) : EatDrinkAction(botAI, "drink", true) {}

    bool isUseful() override;
};

// Potions and healthstones, in combat: the best usable one in the bags that isn't on cooldown
class UseConsumableAction : public UseItemAction
{
public:
    UseConsumableAction(PlayerbotAI* botAI, std::string const name) : UseItemAction(botAI, name) {}

    bool Execute(Event event) override;
    bool isUseful() override;

protected:
    virtual std::vector<Item*> FindItems();
};

class UseHealingPotionAction : public UseConsumableAction
{
public:
    UseHealingPotionAction(PlayerbotAI* botAI) : UseConsumableAction(botAI, "healing potion") {}
};

class UseManaPotionAction : public UseConsumableAction
{
public:
    UseManaPotionAction(PlayerbotAI* botAI) : UseConsumableAction(botAI, "mana potion") {}

    bool isUseful() override;
};

class UseHealthstoneAction : public UseConsumableAction
{
public:
    UseHealthstoneAction(PlayerbotAI* botAI) : UseConsumableAction(botAI, "healthstone") {}

protected:
    std::vector<Item*> FindItems() override;
};

#endif
