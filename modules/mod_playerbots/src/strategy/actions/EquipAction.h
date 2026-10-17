/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_EQUIPACTION_H
#define _PLAYERBOT_EQUIPACTION_H

#include "InventoryAction.h"

class PlayerbotAI;

// "e <items>": equip items from the bags (bags go into a free bag slot)
class EquipAction : public InventoryAction
{
public:
    EquipAction(PlayerbotAI* botAI, std::string const name = "equip") : InventoryAction(botAI, name) {}

    bool Execute(Event event) override;

protected:
    bool EquipItem(Item* item, bool tellErrors = true);
};

// Equip the items in the bags that are upgrades, when an item arrives (loot, trade, quest reward)
class EquipUpgradesAction : public EquipAction
{
public:
    EquipUpgradesAction(PlayerbotAI* botAI) : EquipAction(botAI, "equip upgrades") {}

    bool Execute(Event event) override;
};

// "ue <items or slots>": move equipped items to the bags
class UnequipAction : public InventoryAction
{
public:
    UnequipAction(PlayerbotAI* botAI) : InventoryAction(botAI, "unequip") {}

    bool Execute(Event event) override;

private:
    bool UnequipItem(Item* item);
};

#endif
