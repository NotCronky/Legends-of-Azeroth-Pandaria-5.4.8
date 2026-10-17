#ifndef _PLAYERBOT_ITEMUSAGEVALUE_H
#define _PLAYERBOT_ITEMUSAGEVALUE_H

#include "ItemVisitors.h"
#include "NamedObjectContext.h"
#include "Value.h"

class Item;
class Player;
class PlayerbotAI;
class StatsWeightCalculator;

struct ItemTemplate;

// What an item is to the bot. Qualifier: "<item id>" or "<item id>,<random property id>"
class ItemUsageValue : public CalculatedValue<ItemUsage>, public Qualified
{
public:
    ItemUsageValue(PlayerbotAI* botAI, std::string const name = "item usage") : CalculatedValue<ItemUsage>(botAI, name) {}

    ItemUsage Calculate() override;

    static std::string const Qualify(uint32 itemId, int32 randomPropertyId = 0);
    // "food", "drink", "healing potion", "mana potion", "bandage" or empty
    static std::string const GetConsumableType(ItemTemplate const* proto, bool hasMana);
    // The equipment slot an item would replace: an empty one, or the weakest of the slots it fits (rings, trinkets,
    // dual wielded weapons), with the score of what is there now. NULL_SLOT if it doesn't fit or is already worn
    static uint8 FindUpgradeSlot(Player* bot, ItemTemplate const* proto, StatsWeightCalculator& calculator, float& oldScore);

protected:
    ItemUsage QueryItemUsageForEquip(ItemTemplate const* proto, int32 randomPropertyId);

private:
    bool IsItemNeededForSkill(ItemTemplate const* proto);
    bool IsItemUsefulForSkill(ItemTemplate const* proto);
    bool IsItemUsefulForQuest(ItemTemplate const* proto);
    uint32 GetSmallestBagSize();
    // Stacks of this item, and of better items of the same kind, in the bags
    float CurrentStacks(ItemTemplate const* proto);
    float BetterStacks(ItemTemplate const* proto);
};

#endif
