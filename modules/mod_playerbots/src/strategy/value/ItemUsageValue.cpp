#include "ItemUsageValue.h"

#include "Bag.h"
#include "Playerbots.h"
#include "SpellMgr.h"
#include "StatsWeightCalculator.h"

namespace
{
    // Gathering and crafting tools that have to be in the bags
    bool IsProfessionTool(Player* bot, uint32 itemId)
    {
        switch (itemId)
        {
            case 2901:   // Mining Pick
            case 20723:  // Brann's Trusty Pick
                return bot->HasSkill(SKILL_MINING);
            case 7005:   // Skinning Knife
            case 12709:  // Finkle's Skinner
            case 19901:  // Zulian Slicer
                return bot->HasSkill(SKILL_SKINNING);
            case 40772:  // Gnomish Army Knife
                return bot->HasSkill(SKILL_MINING) || bot->HasSkill(SKILL_SKINNING) ||
                       bot->HasSkill(SKILL_ENGINEERING) || bot->HasSkill(SKILL_BLACKSMITHING);
            case 40892:  // Hammer Pick
                return bot->HasSkill(SKILL_MINING) || bot->HasSkill(SKILL_BLACKSMITHING);
            case 40893:  // Bladed Pickaxe
                return bot->HasSkill(SKILL_MINING) || bot->HasSkill(SKILL_SKINNING);
            case 5956:   // Blacksmith Hammer
                return bot->HasSkill(SKILL_BLACKSMITHING) || bot->HasSkill(SKILL_ENGINEERING);
            case 6219:   // Arclight Spanner
                return bot->HasSkill(SKILL_ENGINEERING);
            case 6256:   // Fishing Pole
                return bot->HasSkill(SKILL_FISHING);
            default:
                return false;
        }
    }

    // The skill a recipe teaches
    uint32 RecipeSkill(uint32 subClass)
    {
        switch (subClass)
        {
            case ITEM_SUBCLASS_LEATHERWORKING_PATTERN: return SKILL_LEATHERWORKING;
            case ITEM_SUBCLASS_TAILORING_PATTERN: return SKILL_TAILORING;
            case ITEM_SUBCLASS_ENGINEERING_SCHEMATIC: return SKILL_ENGINEERING;
            case ITEM_SUBCLASS_BLACKSMITHING: return SKILL_BLACKSMITHING;
            case ITEM_SUBCLASS_COOKING_RECIPE: return SKILL_COOKING;
            case ITEM_SUBCLASS_ALCHEMY_RECIPE: return SKILL_ALCHEMY;
            case ITEM_SUBCLASS_FIRST_AID_MANUAL: return SKILL_FIRST_AID;
            case ITEM_SUBCLASS_ENCHANTING_FORMULA: return SKILL_ENCHANTING;
            case ITEM_SUBCLASS_FISHING_MANUAL: return SKILL_FISHING;
            case ITEM_SUBCLASS_JEWELCRAFTING_RECIPE: return SKILL_JEWELCRAFTING;
            case ITEM_SUBCLASS_INSCRIPTION_TECHNIQUE: return SKILL_INSCRIPTION;
            default: return 0;
        }
    }

    // The spell a recipe teaches: its learn spell's first learn effect
    uint32 RecipeSpell(ItemTemplate const* proto)
    {
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        {
            if (proto->Spells[i].SpellTrigger != ITEM_SPELLTRIGGER_LEARN_SPELL_ID || proto->Spells[i].SpellId <= 0)
                continue;

            return uint32(proto->Spells[i].SpellId);
        }

        for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
                for (uint32 e = 0; e < MAX_SPELL_EFFECTS; ++e)
                    if (spellInfo->Effects[e].Effect == SPELL_EFFECT_LEARN_SPELL)
                        return spellInfo->Effects[e].TriggerSpell;

        return 0;
    }

    // A known recipe of a profession, using this item, still gives skill points
    bool SpellGivesSkillUp(Player* bot, uint32 spellId)
    {
        SkillLineAbilityMapBounds const bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
        {
            SkillLineAbilityEntry const* ability = itr->second;
            if (!ability->SkillLine || !bot->HasSkill(ability->SkillLine))
                continue;

            // Grey from max_value (m_trivialSkillLineRankHigh) on
            if (ability->max_value && bot->GetPureSkillValue(ability->SkillLine) < ability->max_value)
                return true;
        }

        return false;
    }

    void ForEachBagItem(Player* bot, std::function<void(Item*)> const& visit)
    {
        for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
            if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                visit(item);

        for (uint8 bagSlot = INVENTORY_SLOT_BAG_START; bagSlot < INVENTORY_SLOT_BAG_END; ++bagSlot)
            if (Bag* bag = bot->GetBagByPos(bagSlot))
                for (uint32 slot = 0; slot < bag->GetBagSize(); ++slot)
                    if (Item* item = bag->GetItemByPos(slot))
                        visit(item);
    }

    bool IsBroken(Item* item)
    {
        return item && item->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY) > 0 &&
               item->GetUInt32Value(ITEM_FIELD_DURABILITY) == 0;
    }
}

std::string const ItemUsageValue::Qualify(uint32 itemId, int32 randomPropertyId)
{
    if (!randomPropertyId)
        return std::to_string(itemId);

    return std::to_string(itemId) + "," + std::to_string(randomPropertyId);
}

ItemUsage ItemUsageValue::Calculate()
{
    uint32 itemId = 0;
    int32 randomPropertyId = 0;
    size_t const comma = qualifier.find(',');
    itemId = uint32(atoi(qualifier.substr(0, comma).c_str()));
    if (comma != std::string::npos)
        randomPropertyId = atoi(qualifier.substr(comma + 1).c_str());

    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
    if (!proto)
        return ITEM_USAGE_NONE;

    if (IsItemNeededForSkill(proto))
        return CurrentStacks(proto) < 1.0f ? ITEM_USAGE_SKILL : ITEM_USAGE_KEEP;

    if (IsItemUsefulForSkill(proto))
        return ITEM_USAGE_SKILL;

    if (proto->Class == ITEM_CLASS_KEY)
        return ITEM_USAGE_USE;

    if (proto->Class == ITEM_CLASS_CONSUMABLE && (proto->MaxCount <= 0 || bot->GetItemCount(itemId) < uint32(proto->MaxCount)))
    {
        std::string const type = GetConsumableType(proto, bot->GetMaxPower(POWER_MANA) > 0);
        if (!type.empty() && bot->CanUseItem(proto) == EQUIP_ERR_OK)
        {
            // Carry two stacks of the best food, drink and potions
            float const stacks = BetterStacks(proto) + CurrentStacks(proto);
            if (stacks < 2.0f)
                return ITEM_USAGE_USE;
            if (stacks < 3.0f)
                return ITEM_USAGE_KEEP;
        }
    }

    ItemUsage const equip = QueryItemUsageForEquip(proto, randomPropertyId);
    if (equip != ITEM_USAGE_NONE)
        return equip;

    if (IsItemUsefulForQuest(proto))
        return ITEM_USAGE_QUEST;

    Item* item = bot->GetItemByEntry(itemId);
    bool const soulbound = item && item->IsSoulBound();
    if ((proto->Class == ITEM_CLASS_ARMOR || proto->Class == ITEM_CLASS_WEAPON) && proto->Quality >= ITEM_QUALITY_UNCOMMON &&
        proto->DisenchantID && bot->HasSkill(SKILL_ENCHANTING) &&
        bot->GetSkillValue(SKILL_ENCHANTING) >= proto->RequiredDisenchantSkill &&
        (proto->Bonding == BIND_ON_ACQUIRE || soulbound))
        return ITEM_USAGE_DISENCHANT;

    if (proto->SellPrice > 0)
    {
        if (proto->Quality >= ITEM_QUALITY_NORMAL && !soulbound && proto->Bonding != BIND_ON_ACQUIRE &&
            proto->Bonding != BIND_QUEST)
            return ITEM_USAGE_AH;

        return ITEM_USAGE_VENDOR;
    }

    return ITEM_USAGE_NONE;
}

ItemUsage ItemUsageValue::QueryItemUsageForEquip(ItemTemplate const* proto, int32 randomPropertyId)
{
    if (proto->InventoryType == INVTYPE_NON_EQUIP || bot->CanUseItem(proto) != EQUIP_ERR_OK)
        return ITEM_USAGE_NONE;

    if (proto->Class == ITEM_CLASS_CONTAINER)
    {
        if (proto->SubClass != ITEM_SUBCLASS_CONTAINER || GetSmallestBagSize() >= proto->ContainerSlots)
            return ITEM_USAGE_NONE;

        return ITEM_USAGE_EQUIP;
    }

    if (proto->Class == ITEM_CLASS_QUIVER)
        return ITEM_USAGE_NONE;

    // Fishing poles and other tools
    if (proto->Class == ITEM_CLASS_WEAPON &&
        (proto->SubClass == ITEM_SUBCLASS_WEAPON_MISCELLANEOUS || proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE))
        return ITEM_USAGE_NONE;

    uint8 const slot = bot->FindEquipSlot(proto, NULL_SLOT, true);
    if (slot == NULL_SLOT)
        return ITEM_USAGE_NONE;

    // One unique item at a time
    if (proto->Flags & ITEM_PROTO_FLAG_UNIQUE_EQUIPPED)
        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
            if (Item* equipped = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                if (equipped->GetEntry() == proto->ItemId)
                    return ITEM_USAGE_NONE;

    // An off-hand item while wielding a two-hander
    if (slot == EQUIPMENT_SLOT_OFFHAND && !bot->CanTitanGrip())
        if (Item* mainHand = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
            if (mainHand->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                return ITEM_USAGE_NONE;

    StatsWeightCalculator calculator(bot);
    float const score = calculator.CalculateItem(proto, randomPropertyId);

    float oldScore = 0.0f;
    uint8 const upgradeSlot = FindUpgradeSlot(bot, proto, calculator, oldScore);
    if (upgradeSlot == NULL_SLOT)
        return ITEM_USAGE_NONE;

    Item* oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, upgradeSlot);
    if (!oldItem)
        return score > 0.0f ? ITEM_USAGE_EQUIP : ITEM_USAGE_BAD_EQUIP;

    if (IsBroken(oldItem) && score > 0.0f)
        return ITEM_USAGE_BROKEN_EQUIP;

    if (score > oldScore * sPlayerbotAIConfig->equipUpgradeThreshold)
        return ITEM_USAGE_REPLACE;

    return ITEM_USAGE_NONE;
}

uint8 ItemUsageValue::FindUpgradeSlot(Player* bot, ItemTemplate const* proto, StatsWeightCalculator& calculator,
                                      float& oldScore)
{
    uint8 const slot = bot->FindEquipSlot(proto, NULL_SLOT, true);
    if (slot == NULL_SLOT)
        return NULL_SLOT;

    // Slots the item could go in: both rings, both trinkets, both hands for dual wielders
    std::vector<uint8> slots = { slot };
    if (slot == EQUIPMENT_SLOT_FINGER1 || slot == EQUIPMENT_SLOT_FINGER2)
        slots = { EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_FINGER2 };
    else if (slot == EQUIPMENT_SLOT_TRINKET1 || slot == EQUIPMENT_SLOT_TRINKET2)
        slots = { EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_TRINKET2 };
    else if ((slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND) && bot->CanDualWield() &&
             proto->InventoryType == INVTYPE_WEAPON)
        slots = { EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_OFFHAND };

    // The weakest of them; an empty one is always worth filling
    uint8 weakest = NULL_SLOT;
    oldScore = -1.0f;
    for (uint8 candidate : slots)
    {
        Item* equipped = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, candidate);
        if (!equipped)
        {
            oldScore = 0.0f;
            return candidate;
        }

        // Already wearing one: a second copy is no upgrade
        if (equipped->GetEntry() == proto->ItemId)
            return NULL_SLOT;

        float equippedScore = calculator.CalculateItem(equipped);
        // A two-hander replaces the off hand too
        if (candidate == EQUIPMENT_SLOT_MAINHAND && proto->InventoryType == INVTYPE_2HWEAPON && !bot->CanTitanGrip())
            if (Item* offHand = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                equippedScore += calculator.CalculateItem(offHand);

        if (weakest == NULL_SLOT || equippedScore < oldScore)
        {
            oldScore = equippedScore;
            weakest = candidate;
        }
    }

    return weakest;
}

bool ItemUsageValue::IsItemNeededForSkill(ItemTemplate const* proto) { return IsProfessionTool(bot, proto->ItemId); }

bool ItemUsageValue::IsItemUsefulForSkill(ItemTemplate const* proto)
{
    if (proto->Class == ITEM_CLASS_RECIPE)
    {
        uint32 const skill = RecipeSkill(proto->SubClass);
        uint32 const spell = RecipeSpell(proto);
        return skill && spell && bot->HasSkill(skill) && !bot->HasSpell(spell) && bot->CanUseItem(proto) == EQUIP_ERR_OK;
    }

    if (proto->Class != ITEM_CLASS_TRADE_GOODS && proto->Class != ITEM_CLASS_REAGENT && proto->Class != ITEM_CLASS_GEM &&
        proto->Class != ITEM_CLASS_MISCELLANEOUS)
        return false;

    // A reagent of a known recipe that still gives skill points
    for (auto const& [spellId, playerSpell] : bot->GetSpellMap())
    {
        if (playerSpell->state == PLAYERSPELL_REMOVED || !playerSpell->active)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo || spellInfo->IsPassive() || spellInfo->Effects[0].Effect != SPELL_EFFECT_CREATE_ITEM)
            continue;

        for (uint32 i = 0; i < MAX_SPELL_REAGENTS; ++i)
            if (spellInfo->Reagent[i] > 0 && uint32(spellInfo->Reagent[i]) == proto->ItemId &&
                SpellGivesSkillUp(bot, spellId))
                return true;
    }

    return false;
}

bool ItemUsageValue::IsItemUsefulForQuest(ItemTemplate const* proto)
{
    if (bot->HasQuestForItem(proto->ItemId))
        return true;

    // Items that start a quest the bot hasn't done
    if (proto->StartQuest)
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(proto->StartQuest))
            return bot->GetQuestStatus(proto->StartQuest) == QUEST_STATUS_NONE && bot->CanTakeQuest(quest, false);

    return false;
}

uint32 ItemUsageValue::GetSmallestBagSize()
{
    uint32 smallest = 0;
    for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; ++slot)
    {
        Bag* bag = bot->GetBagByPos(slot);
        if (!bag)
            return 0;

        if (bag->GetTemplate()->SubClass != ITEM_SUBCLASS_CONTAINER)
            continue;

        if (!smallest || bag->GetBagSize() < smallest)
            smallest = bag->GetBagSize();
    }

    return smallest;
}

float ItemUsageValue::CurrentStacks(ItemTemplate const* proto)
{
    uint32 count = 0;
    ForEachBagItem(bot, [&](Item* item) {
        if (item->GetEntry() == proto->ItemId)
            count += item->GetCount();
    });

    return float(count) / proto->GetMaxStackSize();
}

float ItemUsageValue::BetterStacks(ItemTemplate const* proto)
{
    std::string const type = GetConsumableType(proto, bot->GetMaxPower(POWER_MANA) > 0);
    float stacks = 0.0f;
    ForEachBagItem(bot, [&](Item* item) {
        ItemTemplate const* other = item->GetTemplate();
        if (other->ItemId == proto->ItemId || other->Class != proto->Class || other->SubClass != proto->SubClass ||
            other->ItemLevel < proto->ItemLevel)
            return;

        if (GetConsumableType(other, bot->GetMaxPower(POWER_MANA) > 0) != type)
            return;

        stacks += float(item->GetCount()) / other->GetMaxStackSize();
    });

    return stacks;
}

std::string const ItemUsageValue::GetConsumableType(ItemTemplate const* proto, bool hasMana)
{
    if (proto->Class != ITEM_CLASS_CONSUMABLE)
        return "";

    if (proto->SubClass == ITEM_SUBCLASS_CONSUMABLE || proto->SubClass == ITEM_SUBCLASS_FOOD_DRINK)
    {
        // Spell categories of eating and drinking
        if (proto->Spells[0].SpellCategory == 11)
            return "food";
        if (proto->Spells[0].SpellCategory == 59 && hasMana)
            return "drink";
    }

    if (proto->SubClass == ITEM_SUBCLASS_POTION)
    {
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
                for (uint32 e = 0; e < MAX_SPELL_EFFECTS; ++e)
                {
                    if (spellInfo->Effects[e].Effect == SPELL_EFFECT_ENERGIZE && spellInfo->Effects[e].MiscValue == POWER_MANA)
                        return hasMana ? "mana potion" : "";
                    if (spellInfo->Effects[e].Effect == SPELL_EFFECT_HEAL)
                        return "healing potion";
                }
    }

    if (proto->SubClass == ITEM_SUBCLASS_BANDAGE)
        return "bandage";

    return "";
}
