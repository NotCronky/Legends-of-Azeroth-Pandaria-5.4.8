/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "UseItemAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"

bool UseItemAction::Execute(Event event)
{
    if (!CheckManager(event))
        return false;

    std::string const text = event.getParam();
    if (text.empty())
    {
        botAI->TellMaster("Usage: u <item link or name> [target item link]");
        return false;
    }

    // "u [item] [target item]": links in the order given
    std::vector<uint32> linked;
    for (size_t pos = text.find("|Hitem:"); pos != std::string::npos; pos = text.find("|Hitem:", pos + 1))
    {
        std::set<uint32> const ids = ChatHelper::parseItems(text.substr(pos, text.find("|h", pos) - pos));
        if (!ids.empty())
            linked.push_back(*ids.begin());
    }

    Item* item = nullptr;
    Item* itemTarget = nullptr;
    if (!linked.empty())
    {
        FindItemByIdVisitor visitor(linked[0]);
        IterateItems(&visitor, IterateItemsMask(ITERATE_ITEMS_IN_BAGS | ITERATE_ITEMS_IN_EQUIP));
        if (!visitor.GetResult().empty())
            item = visitor.GetResult().front();

        if (linked.size() > 1)
        {
            FindItemByIdVisitor targetVisitor(linked[1]);
            IterateItems(&targetVisitor, IterateItemsMask(ITERATE_ITEMS_IN_BAGS | ITERATE_ITEMS_IN_EQUIP));
            if (targetVisitor.GetResult().empty())
            {
                botAI->TellMaster("I don't have the item to use it on");
                return false;
            }

            itemTarget = targetVisitor.GetResult().front();
        }
    }
    else
    {
        std::vector<Item*> const items = parseItems(text);
        if (!items.empty())
            item = items.front();
    }

    if (!item)
    {
        botAI->TellMaster("I don't have " + text);
        return false;
    }

    return UseItem(item, itemTarget);
}

bool UseItemAction::UseItem(Item* item, Item* itemTarget)
{
    ItemTemplate const* proto = item->GetTemplate();
    std::string const link = ChatHelper::FormatItem(proto);

    if (proto->Flags & ITEM_FLAG_HAS_LOOT)
    {
        botAI->TellMaster("I can't open " + link + " yet");
        return false;
    }

    SpellInfo const* spellInfo = nullptr;
    for (_Spell const& spell : proto->Spells)
        if (spell.SpellId > 0 && spell.SpellTrigger == ITEM_SPELLTRIGGER_ON_USE)
            if ((spellInfo = sSpellMgr->GetSpellInfo(spell.SpellId)))
                break;

    if (!spellInfo)
    {
        botAI->TellMaster(link + " can't be used");
        return false;
    }

    // The checks WorldSession::HandleUseItemOpcode makes before casting
    if (proto->InventoryType != INVTYPE_NON_EQUIP && !item->IsEquipped())
    {
        botAI->TellMaster("I have to equip " + link + " to use it");
        return false;
    }

    if (bot->CanUseItem(item) != EQUIP_ERR_OK)
    {
        botAI->TellMaster("I can't use " + link);
        return false;
    }

    if (bot->IsInCombat() && !spellInfo->CanBeUsedInCombat())
    {
        botAI->TellMaster("I can't use " + link + " in combat");
        return false;
    }

    SpellCastTargets targets;
    if (itemTarget)
        targets.SetItemTarget(itemTarget);
    else if (!spellInfo->IsPositive())
    {
        Unit* target = FindHarmfulTarget();
        if (!target)
        {
            botAI->TellMaster("I have no target for " + link);
            return false;
        }

        targets.SetUnitTarget(target);
    }
    else
        targets.SetUnitTarget(bot);

    if (proto->Bonding == BIND_ON_USE || proto->Bonding == BIND_ON_ACQUIRE || proto->Bonding == BIND_QUEST)
    {
        if (!item->IsSoulBound())
        {
            item->SetState(ITEM_CHANGED, bot);
            item->SetBinding(true);
        }
    }

    botAI->TellMaster("Using " + link);
    if (!sScriptMgr->OnItemUse(bot, item, targets))
        bot->CastItemUseSpell(item, targets, 0, 0);

    return true;
}

Unit* UseItemAction::FindHarmfulTarget()
{
    if (Unit* victim = bot->GetVictim())
        return victim;

    if (Player* master = GetMaster())
        if (Unit* target = ObjectAccessor::GetUnit(*bot, master->GetTarget()))
            if (bot->IsValidAttackTarget(target))
                return target;

    return nullptr;
}
