/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "LootActions.h"

#include "Bag.h"
#include "ChatHelper.h"
#include "Event.h"
#include "Group.h"
#include "ItemUsageValue.h"
#include "LootMgr.h"
#include "LootValues.h"
#include "Playerbots.h"
#include "ServerFacade.h"
#include "SpellMgr.h"

namespace
{
    // CMSG_LOOT, CMSG_LOOT_RELEASE and CMSG_AUTOSTORE_LOOT_ITEM in the order the LootHandler reads them
    void SendLoot(Player* bot, ObjectGuid guid)
    {
        WorldPacket packet(CMSG_LOOT, 9);
        packet.WriteBit(guid[4]);
        packet.WriteBit(guid[5]);
        packet.WriteBit(guid[2]);
        packet.WriteBit(guid[7]);
        packet.WriteBit(guid[0]);
        packet.WriteBit(guid[1]);
        packet.WriteBit(guid[3]);
        packet.WriteBit(guid[6]);
        packet.FlushBits();
        packet.WriteByteSeq(guid[3]);
        packet.WriteByteSeq(guid[5]);
        packet.WriteByteSeq(guid[0]);
        packet.WriteByteSeq(guid[6]);
        packet.WriteByteSeq(guid[4]);
        packet.WriteByteSeq(guid[1]);
        packet.WriteByteSeq(guid[7]);
        packet.WriteByteSeq(guid[2]);
        bot->GetSession()->HandleLootOpcode(packet);
    }

    void SendAutostoreLootItems(Player* bot, std::vector<std::pair<ObjectGuid, uint8>> const& slots)
    {
        WorldPacket packet(CMSG_AUTOSTORE_LOOT_ITEM, 3 + slots.size() * 10);
        packet.WriteBits(slots.size(), 23);
        for (auto const& [guid, slot] : slots)
        {
            packet.WriteBit(guid[2]);
            packet.WriteBit(guid[7]);
            packet.WriteBit(guid[0]);
            packet.WriteBit(guid[6]);
            packet.WriteBit(guid[5]);
            packet.WriteBit(guid[3]);
            packet.WriteBit(guid[1]);
            packet.WriteBit(guid[4]);
        }
        packet.FlushBits();
        for (auto const& [guid, slot] : slots)
        {
            packet.WriteByteSeq(guid[0]);
            packet.WriteByteSeq(guid[4]);
            packet.WriteByteSeq(guid[1]);
            packet.WriteByteSeq(guid[7]);
            packet.WriteByteSeq(guid[6]);
            packet.WriteByteSeq(guid[5]);
            packet.WriteByteSeq(guid[3]);
            packet.WriteByteSeq(guid[2]);
            packet << uint8(slot);
        }
        bot->GetSession()->HandleAutostoreLootItemOpcode(packet);
    }

    void SendLootRoll(Player* bot, ObjectGuid guid, uint8 slot, uint8 rollType)
    {
        WorldPacket packet(CMSG_LOOT_ROLL, 11);
        packet << uint8(slot) << uint8(rollType);
        packet.WriteBit(guid[7]);
        packet.WriteBit(guid[1]);
        packet.WriteBit(guid[2]);
        packet.WriteBit(guid[0]);
        packet.WriteBit(guid[6]);
        packet.WriteBit(guid[3]);
        packet.WriteBit(guid[4]);
        packet.WriteBit(guid[5]);
        packet.FlushBits();
        packet.WriteByteSeq(guid[0]);
        packet.WriteByteSeq(guid[2]);
        packet.WriteByteSeq(guid[7]);
        packet.WriteByteSeq(guid[3]);
        packet.WriteByteSeq(guid[1]);
        packet.WriteByteSeq(guid[5]);
        packet.WriteByteSeq(guid[4]);
        packet.WriteByteSeq(guid[6]);
        bot->GetSession()->HandleLootRoll(packet);
    }

    Loot* GetLoot(Player* bot, ObjectGuid sourceGuid, Object*& source)
    {
        source = nullptr;
        if (sourceGuid.IsGameObject())
        {
            GameObject* go = bot->GetMap()->GetGameObject(sourceGuid);
            source = go;
            return go ? &go->loot : nullptr;
        }

        if (sourceGuid.IsItem())
        {
            Item* item = bot->GetItemByGuid(sourceGuid);
            source = item;
            return item ? &item->loot : nullptr;
        }

        if (sourceGuid.IsCreatureOrVehicle())
        {
            Creature* creature = bot->GetMap()->GetCreature(sourceGuid);
            source = creature;
            return creature ? &creature->loot : nullptr;
        }

        return nullptr;
    }

    // What the loot window would let this player take, as LootView::WriteData decides it
    bool CanTakeLootItem(Player* bot, Loot* loot, LootItem* item, Object* source, bool special)
    {
        if (item->is_blocked)
            return false;

        if (special)
            return true;

        if (!item->AllowedForPlayer(bot, source))
            return false;

        // Round robin: only the player whose turn it is
        return !loot->roundRobinPlayer || loot->roundRobinPlayer == bot->GetGUID().GetRawValue();
    }

    uint32 CountFreeBagSlots(Player* bot)
    {
        uint32 free = 0;
        for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
            if (!bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                ++free;

        for (uint8 bagSlot = INVENTORY_SLOT_BAG_START; bagSlot < INVENTORY_SLOT_BAG_END; ++bagSlot)
            if (Bag* bag = bot->GetBagByPos(bagSlot))
                if (bag->GetTemplate()->SubClass == ITEM_SUBCLASS_CONTAINER)
                    free += bag->GetFreeSlots();

        return free;
    }

    bool IsUniqueAndOwned(Player* bot, ItemTemplate const* proto)
    {
        return (proto->Flags & ITEM_PROTO_FLAG_UNIQUE_EQUIPPED) && bot->GetItemCount(proto->ItemId) > 0;
    }
}

bool LootAction::Execute(Event /*event*/)
{
    if (!AI_VALUE(bool, "has available loot"))
        return false;

    LootObject const prevLoot = AI_VALUE(LootObject, "loot target");
    LootObject const lootObject = AI_VALUE(LootObjectStack*, "available loot")->GetLoot(sPlayerbotAIConfig->lootDistance);
    if (lootObject.IsEmpty())
        return false;

    if (!prevLoot.IsEmpty() && prevLoot.guid != lootObject.guid)
        bot->GetSession()->DoLootReleaseAll();

    context->GetValue<LootObject>("loot target")->Set(lootObject);
    return true;
}

bool LootAction::isUseful()
{
    // With free for all loot a player would have to race the bots for it
    Group* group = bot->GetGroup();
    return sPlayerbotAIConfig->freeMethodLoot || !group || group->GetLootMethod() != FREE_FOR_ALL;
}

bool MoveToLootAction::Execute(Event /*event*/)
{
    LootObject loot = AI_VALUE(LootObject, "loot target");
    if (!loot.IsLootPossible(bot))
        return false;

    WorldObject* target = loot.GetWorldObject(bot);
    if (!target)
        return false;

    return MoveNear(target, sPlayerbotAIConfig->contactDistance);
}

bool OpenLootAction::Execute(Event /*event*/)
{
    LootObject lootObject = AI_VALUE(LootObject, "loot target");
    bool const result = DoLoot(lootObject);

    // Opened, or impossible to open: either way don't come back to it
    if (result || !lootObject.IsLootPossible(bot))
    {
        AI_VALUE(LootObjectStack*, "available loot")->Remove(lootObject.guid);
        context->GetValue<LootObject>("loot target")->Set(LootObject());
    }

    return result;
}

uint32 OpenLootAction::FindSpell(uint32 effect, uint32 miscValue, uint32 skill)
{
    uint32 fallback = 0;
    for (auto const& [spellId, playerSpell] : bot->GetSpellMap())
    {
        if (playerSpell->state == PLAYERSPELL_REMOVED || playerSpell->disabled)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo || spellInfo->IsPassive())
            continue;

        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (spellInfo->Effects[i].Effect != effect)
                continue;

            if (effect == SPELL_EFFECT_OPEN_LOCK && uint32(spellInfo->Effects[i].MiscValue) != miscValue)
                continue;

            // Skinning-type spells: prefer the one of the creature's skill (skinning, herbalism, mining, engineering)
            if (effect == SPELL_EFFECT_SKINNING && skill)
            {
                SkillLineAbilityMapBounds const bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
                for (auto itr = bounds.first; itr != bounds.second; ++itr)
                    if (itr->second->SkillLine == skill)
                        return spellId;

                if (!fallback)
                    fallback = spellId;
                continue;
            }

            return spellId;
        }
    }

    return fallback;
}

bool OpenLootAction::DoLoot(LootObject& lootObject)
{
    if (lootObject.IsEmpty())
        return false;

    WorldObject* target = lootObject.GetWorldObject(bot);
    if (!target || bot->GetDistance(target) > INTERACTION_DISTANCE - 2.0f)
        return false;

    if (bot->IsMounted())
    {
        bot->Dismount();
        bot->RemoveAurasByType(SPELL_AURA_MOUNTED);
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
    }

    Creature* creature = lootObject.guid.IsCreatureOrVehicle() ? target->ToCreature() : nullptr;
    if (creature && creature->HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE))
    {
        // The loot window opens with SMSG_LOOT_RESPONSE, which triggers "store loot"
        SendLoot(bot, lootObject.guid);
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
        return true;
    }

    // Everything else is a cast with a cast time: stand still first
    if (bot->isMoving())
    {
        bot->StopMoving();
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
        return false;
    }

    if (bot->IsNonMeleeSpellCasted(false))
        return false;

    if (creature)
    {
        uint32 const spellId = FindSpell(SPELL_EFFECT_SKINNING, 0, lootObject.skillId);
        if (!spellId)
            return false;

        bot->SetSelection(creature->GetGUID());
        bot->CastSpell(creature, spellId, false);
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
        return bot->IsNonMeleeSpellCasted(false);
    }

    GameObject* go = target->ToGameObject();
    if (!go || go->GetGoState() != GO_STATE_READY)
        return false;

    // Chests without a lock open on use
    uint32 const lockId = go->GetGOInfo()->GetLockId();
    LockEntry const* lock = lockId ? sLockStore.LookupEntry(lockId) : nullptr;
    if (!lock)
    {
        if (!bot->GetGameObjectIfCanInteractWith(go->GetGUID(), GAMEOBJECT_TYPE_CHEST))
            return false;

        go->Use(bot);
        botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
        return true;
    }

    // Herbs, ore, locked chests: the spell that opens this kind of lock
    uint32 spellId = 0;
    for (uint32 i = 0; i < MAX_LOCK_CASE && !spellId; ++i)
        if (lock->Type[i] == LOCK_KEY_SKILL)
            spellId = FindSpell(SPELL_EFFECT_OPEN_LOCK, lock->Index[i], 0);

    if (!spellId && !lootObject.skillId)
        spellId = sPlayerbotAIConfig->openGoSpell;

    if (!spellId)
        return false;

    bot->CastSpell(go, spellId, false);
    botAI->SetNextCheckDelay(sPlayerbotAIConfig->lootDelay);
    return bot->IsNonMeleeSpellCasted(false);
}

bool StoreLootAction::Execute(Event /*event*/)
{
    // Copy: storing items and releasing loot change the view
    std::unordered_map<ObjectGuid, ObjectGuid> const view = bot->GetLootView();
    if (view.empty())
        return false;

    bool hasMoney = false;
    std::vector<std::pair<ObjectGuid, uint8>> slots;
    uint32 freeSlots = CountFreeBagSlots(bot);
    for (auto const& [lootGuid, sourceGuid] : view)
    {
        Object* source = nullptr;
        Loot* loot = GetLoot(bot, sourceGuid, source);
        if (!loot)
            continue;

        if (loot->gold)
            hasMoney = true;

        uint32 const maxSlot = loot->GetMaxSlotInLootFor(bot);
        for (uint32 slot = 0; slot < maxSlot; ++slot)
        {
            QuestItem* questItem = nullptr;
            QuestItem* ffaItem = nullptr;
            QuestItem* conditionalItem = nullptr;
            QuestItem* currency = nullptr;
            QuestItem* personal = nullptr;
            LootItem* item = loot->LootItemInSlot(slot, bot, &questItem, &ffaItem, &conditionalItem, &currency, &personal);
            if (!item)
                continue;

            bool const special = questItem || ffaItem || conditionalItem || currency || personal;
            if (!CanTakeLootItem(bot, loot, item, source, special))
                continue;

            if (currency)
            {
                slots.emplace_back(lootGuid, uint8(slot));
                continue;
            }

            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(item->itemid);
            if (!proto)
                continue;

            // Skins, herbs and ore are what the bot gathered them for
            if (loot->loot_type != LOOT_SKINNING && !IsLootAllowed(item->itemid, botAI))
                continue;

            ItemPosCountVec dest;
            if (bot->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->itemid, item->count) != EQUIP_ERR_OK)
                continue;

            // Leave a slot free for what matters more than vendor trash
            if (freeSlots <= 1 && proto->Quality <= ITEM_QUALITY_POOR && !questItem)
                continue;

            if (freeSlots)
                --freeSlots;

            slots.emplace_back(lootGuid, uint8(slot));
        }
    }

    if (hasMoney)
    {
        WorldPacket packet(CMSG_LOOT_MONEY, 0);
        bot->GetSession()->HandleLootMoneyOpcode(packet);
    }

    if (!slots.empty())
        SendAutostoreLootItems(bot, slots);

    LootObjectStack* availableLoot = AI_VALUE(LootObjectStack*, "available loot");
    for (auto const& [lootGuid, sourceGuid] : view)
        availableLoot->Remove(sourceGuid);

    bot->GetSession()->DoLootReleaseAll();
    return true;
}

bool StoreLootAction::IsLootAllowed(uint32 itemId, PlayerbotAI* botAI)
{
    AiObjectContext* context = botAI->GetAiObjectContext();
    Player* bot = botAI->GetBot();

    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
    if (!proto)
        return false;

    std::set<uint32>& alwaysLoot = AI_VALUE(std::set<uint32>&, "always loot list");
    if (alwaysLoot.count(itemId))
        return true;

    if (proto->MaxCount > 0 && bot->HasItemCount(itemId, proto->MaxCount, true))
        return false;

    if (proto->StartQuest || bot->HasQuestForItem(itemId))
        return true;

    return AI_VALUE(LootStrategy*, "loot strategy")->CanLoot(proto, context);
}

bool AddAllLootAction::Execute(Event /*event*/)
{
    bool added = false;

    for (ObjectGuid const guid : AI_VALUE(GuidVector, "nearest game objects"))
        added |= AddLoot(guid);

    for (ObjectGuid const guid : AI_VALUE(GuidVector, "nearest corpses"))
        added |= AddLoot(guid);

    return added;
}

bool AddAllLootAction::AddLoot(ObjectGuid guid) { return AI_VALUE(LootObjectStack*, "available loot")->Add(guid); }

bool AddGatheringLootAction::AddLoot(ObjectGuid guid)
{
    LootObject loot(bot, guid);
    if (loot.IsEmpty() || loot.skillId == SKILL_NONE || !loot.IsLootPossible(bot))
        return false;

    return AddAllLootAction::AddLoot(guid);
}

bool LootRollAction::Execute(Event /*event*/)
{
    std::vector<PendingLootRoll> const rolls = botAI->TakePendingLootRolls();
    Group* group = bot->GetGroup();
    if (!group)
        return false;

    for (PendingLootRoll const& roll : rolls)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(roll.itemId);
        uint8 vote = proto ? CalculateRollVote(proto, roll.randomPropertyId) : ROLL_PASS;

        // Votes the roll doesn't allow (need on an item the class can't use, disenchant without enchanters)
        if (vote == ROLL_NEED && !(roll.voteMask & ROLL_FLAG_TYPE_NEED))
            vote = ROLL_GREED;
        if (vote == ROLL_DISENCHANT && !(roll.voteMask & ROLL_FLAG_TYPE_DISENCHANT))
            vote = ROLL_GREED;
        if (vote == ROLL_GREED && !(roll.voteMask & ROLL_FLAG_TYPE_GREED))
            vote = ROLL_PASS;

        SendLootRoll(bot, roll.lootGuid, roll.slot, vote);
    }

    return !rolls.empty();
}

uint8 LootRollAction::CalculateRollVote(ItemTemplate const* proto, int32 randomPropertyId)
{
    ItemUsage const usage = AI_VALUE2(ItemUsage, "item usage", ItemUsageValue::Qualify(proto->ItemId, randomPropertyId));

    uint8 vote = ROLL_PASS;
    // Tier tokens: junk class, epic, restricted to the classes that can turn them in
    if (proto->Class == ITEM_CLASS_MISCELLANEOUS && proto->SubClass == ITEM_SUBCLASS_JUNK && proto->Quality == ITEM_QUALITY_EPIC &&
        proto->AllowableClass > 0)
        vote = (proto->AllowableClass & bot->GetClassMask()) ? ROLL_NEED : ROLL_GREED;
    else if (usage == ITEM_USAGE_DISENCHANT)
        vote = sPlayerbotAIConfig->lootRollDisenchant ? ROLL_DISENCHANT : ROLL_GREED;
    else if (proto->Class == ITEM_CLASS_WEAPON || proto->Class == ITEM_CLASS_ARMOR)
    {
        if (usage == ITEM_USAGE_EQUIP || usage == ITEM_USAGE_REPLACE || usage == ITEM_USAGE_BROKEN_EQUIP)
            vote = ROLL_NEED;
        else if (usage != ITEM_USAGE_NONE)
            vote = ROLL_GREED;
    }
    else if (proto->Class == ITEM_CLASS_RECIPE)
    {
        if (!sPlayerbotAIConfig->lootRollRecipe)
            return ROLL_PASS;

        if (usage == ITEM_USAGE_SKILL)
            vote = ROLL_NEED;
        else if (proto->Bonding != BIND_ON_ACQUIRE)
            vote = ROLL_GREED;
    }
    else if (StoreLootAction::IsLootAllowed(proto->ItemId, botAI))
    {
        switch (usage)
        {
            case ITEM_USAGE_EQUIP:
            case ITEM_USAGE_REPLACE:
            case ITEM_USAGE_QUEST:
                vote = ROLL_NEED;
                break;
            case ITEM_USAGE_SKILL:
            case ITEM_USAGE_USE:
            case ITEM_USAGE_AH:
            case ITEM_USAGE_VENDOR:
            case ITEM_USAGE_KEEP:
                vote = ROLL_GREED;
                break;
            default:
                break;
        }
    }

    if (vote == ROLL_NEED)
    {
        if (!sPlayerbotAIConfig->lootNeedRollLevel || IsUniqueAndOwned(bot, proto))
            vote = ROLL_PASS;
        else if (sPlayerbotAIConfig->lootNeedRollLevel == 1)
            vote = ROLL_GREED;
    }
    else if ((vote == ROLL_GREED || vote == ROLL_DISENCHANT) && !sPlayerbotAIConfig->lootGreedRollLevel)
        vote = ROLL_PASS;

    return vote;
}

bool LootStrategyAction::Execute(Event event)
{
    std::string const param = event.getParam();
    std::set<uint32>& alwaysLoot = AI_VALUE(std::set<uint32>&, "always loot list");
    Value<LootStrategy*>* lootStrategy = context->GetValue<LootStrategy*>("loot strategy");

    if (param.empty() || param == "?")
    {
        std::ostringstream out;
        out << "Loot strategy: " << lootStrategy->Get()->GetName();
        if (!alwaysLoot.empty())
        {
            out << ". Always loot:";
            for (uint32 itemId : alwaysLoot)
                if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId))
                    out << " " << ChatHelper::FormatItem(proto);
        }
        botAI->TellMaster(out);
        return true;
    }

    std::set<uint32> const items = ChatHelper::parseItems(param);
    if (items.empty())
    {
        lootStrategy->Set(LootStrategyValue::instance(param));
        botAI->TellMaster("Loot strategy set to " + lootStrategy->Get()->GetName());
        return true;
    }

    bool const remove = param[0] == '-';
    bool const query = param[0] == '?';
    for (uint32 itemId : items)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
        if (!proto)
            continue;

        if (query)
            botAI->TellMaster((StoreLootAction::IsLootAllowed(itemId, botAI) ? "I will loot " : "I won't loot ") +
                              ChatHelper::FormatItem(proto));
        else if (remove)
        {
            alwaysLoot.erase(itemId);
            botAI->TellMaster(ChatHelper::FormatItem(proto) + " removed from the always loot list");
        }
        else
        {
            alwaysLoot.insert(itemId);
            botAI->TellMaster(ChatHelper::FormatItem(proto) + " added to the always loot list");
        }
    }

    return true;
}
