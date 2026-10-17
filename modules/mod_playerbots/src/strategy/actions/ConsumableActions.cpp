/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ConsumableActions.h"

#include "Event.h"
#include "Playerbots.h"
#include "SpellAuraDefines.h"
#include "SpellHistory.h"
#include "SpellMgr.h"

namespace
{
    struct ConsumableSpell
    {
        uint32 requiredLevel;
        uint32 spellId;
    };

    bool IsRegenSpell(SpellInfo const* spellInfo, bool drink)
    {
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            SpellEffectInfo const& effect = spellInfo->Effects[i];
            if (effect.Effect != SPELL_EFFECT_APPLY_AURA)
                continue;

            if (drink && ((effect.ApplyAuraName == SPELL_AURA_MOD_POWER_REGEN ||
                           effect.ApplyAuraName == SPELL_AURA_OBS_MOD_POWER) && effect.MiscValue == POWER_MANA))
                return true;

            if (!drink && (effect.ApplyAuraName == SPELL_AURA_MOD_REGEN || effect.ApplyAuraName == SPELL_AURA_OBS_MOD_HEALTH))
                return true;
        }

        return false;
    }

    // The eating or drinking spells of vendor food without a well fed buff, by required level
    std::vector<ConsumableSpell> BuildConsumableSpells(bool drink)
    {
        uint32 const category = drink ? SPELL_CATEGORY_DRINK : SPELL_CATEGORY_FOOD;
        std::vector<ConsumableSpell> spells;
        for (auto const& [itemId, proto] : *sObjectMgr->GetItemTemplateStore())
        {
            if (proto.Class != ITEM_CLASS_CONSUMABLE || proto.SubClass != ITEM_SUBCLASS_FOOD_DRINK || !proto.BuyPrice)
                continue;

            if (proto.Spells[0].SpellId <= 0 || proto.Spells[0].SpellCategory != category || proto.Spells[1].SpellId > 0)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto.Spells[0].SpellId);
            if (!spellInfo || !IsRegenSpell(spellInfo, drink))
                continue;

            spells.push_back({ proto.RequiredLevel, uint32(proto.Spells[0].SpellId) });
        }

        std::sort(spells.begin(), spells.end(),
                  [](ConsumableSpell const& a, ConsumableSpell const& b) { return a.requiredLevel < b.requiredLevel; });
        return spells;
    }

    uint32 FindCheatSpell(uint32 level, bool drink)
    {
        // Built once, read from every map thread
        static std::vector<ConsumableSpell> const foodSpells = BuildConsumableSpells(false);
        static std::vector<ConsumableSpell> const drinkSpells = BuildConsumableSpells(true);

        uint32 spellId = 0;
        for (ConsumableSpell const& spell : drink ? drinkSpells : foodSpells)
        {
            if (spell.requiredLevel > level)
                break;

            spellId = spell.spellId;
        }

        return spellId;
    }

    bool IsDisallowedShapeshiftForm(Player* bot)
    {
        switch (bot->GetShapeshiftForm())
        {
            case FORM_TRAVEL:
            case FORM_AQUA:
            case FORM_FLIGHT:
            case FORM_FLIGHT_EPIC:
            case FORM_BEAR:
            case FORM_CAT:
            case FORM_SPIRITOFREDEMPTION:
                return true;
            default:
                return false;
        }
    }

    // The highest level one the bot can use
    Item* BestItem(Player* bot, std::vector<Item*> const& items)
    {
        Item* best = nullptr;
        for (Item* item : items)
        {
            ItemTemplate const* proto = item->GetTemplate();
            if (bot->CanUseItem(proto) != EQUIP_ERR_OK)
                continue;

            if (!best || proto->RequiredLevel > best->GetTemplate()->RequiredLevel ||
                (proto->RequiredLevel == best->GetTemplate()->RequiredLevel && proto->ItemLevel > best->GetTemplate()->ItemLevel))
                best = item;
        }

        return best;
    }
}

bool EatDrinkAction::IsConsuming()
{
    if (drink)
        return bot->HasAuraType(SPELL_AURA_MOD_POWER_REGEN) || bot->HasAuraType(SPELL_AURA_OBS_MOD_POWER);

    return bot->HasAuraType(SPELL_AURA_MOD_REGEN) || bot->HasAuraType(SPELL_AURA_OBS_MOD_HEALTH);
}

bool EatDrinkAction::Execute(Event /*event*/)
{
    if (IsConsuming())
        return false;

    if (bot->isMoving())
        bot->StopMoving();

    bool consumed = false;
    if (sPlayerbotAIConfig->HasCheat(BotCheatMask::food))
    {
        uint32 const spellId = FindCheatSpell(bot->GetLevel(), drink);
        if (!spellId)
            return false;

        bot->CastSpell(bot, spellId, true);
        consumed = IsConsuming();
    }
    else if (Item* item = BestItem(bot, parseItems(drink ? "drink" : "food")))
        consumed = UseItem(item, nullptr, true);

    if (!consumed)
        return false;

    bot->SetStandState(UNIT_STAND_STATE_SIT);

    // Sit until full, about 20 seconds from empty
    float const percent = drink ? bot->GetPowerPct(POWER_MANA) : bot->GetHealthPct();
    botAI->SetNextCheckDelay(uint32(18000.0f * (100.0f - percent) / 100.0f));
    return true;
}

bool EatDrinkAction::isPossible()
{
    if (bot->IsInCombat() || bot->IsMounted() || bot->IsInWater() || IsDisallowedShapeshiftForm(bot))
        return false;

    if (sPlayerbotAIConfig->HasCheat(BotCheatMask::food))
        return true;

    return BestItem(bot, parseItems(drink ? "drink" : "food")) != nullptr;
}

bool EatAction::isUseful() { return bot->GetHealthPct() < 100.0f && !IsConsuming(); }

bool DrinkAction::isUseful()
{
    return bot->GetMaxPower(POWER_MANA) > 0 && bot->GetPowerPct(POWER_MANA) < 100.0f && !IsConsuming();
}

std::vector<Item*> UseConsumableAction::FindItems() { return parseItems(getName()); }

bool UseConsumableAction::Execute(Event /*event*/)
{
    Item* item = BestItem(bot, FindItems());
    return item && UseItem(item, nullptr, true);
}

bool UseConsumableAction::isUseful()
{
    if (!bot->IsInCombat())
        return false;

    for (Item* item : FindItems())
        for (_Spell const& spell : item->GetTemplate()->Spells)
            if (spell.SpellId > 0 && spell.SpellTrigger == ITEM_SPELLTRIGGER_ON_USE &&
                bot->GetSpellHistory()->IsReady(uint32(spell.SpellId), item->GetEntry()))
                return true;

    return false;
}

bool UseManaPotionAction::isUseful() { return bot->GetMaxPower(POWER_MANA) > 0 && UseConsumableAction::isUseful(); }

std::vector<Item*> UseHealthstoneAction::FindItems()
{
    // Warlock healthstones: one item in MoP, whatever the warlock's level
    FindNamedItemVisitor visitor(bot, "Healthstone");
    IterateItems(&visitor);
    return visitor.GetResult();
}
