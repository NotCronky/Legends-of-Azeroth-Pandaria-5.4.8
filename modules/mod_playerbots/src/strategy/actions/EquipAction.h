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
    EquipAction(PlayerbotAI* botAI) : InventoryAction(botAI, "equip") {}

    bool Execute(Event event) override;

private:
    bool EquipItem(Item* item);
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
