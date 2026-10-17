/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "LootObjectStack.h"

#include "Playerbots.h"

namespace
{
    constexpr size_t MAX_LOOT_OBJECT_COUNT = 200;
    // Corpses and objects are forgotten this long after they were seen
    constexpr time_t LOOT_TARGET_LIFETIME = 30;

    // Skill needed to skin (or herb, mine, salvage) a creature of this level, as Spell::CheckCast computes it
    uint32 SkinningSkillRequired(uint32 level)
    {
        int32 const value = level < 10 ? 0 :
                            level < 20 ? (level - 10) * 10 :
                            level < 74 ? level * 5 :
                            level < 80 ? (level * 2 - 73) * 5 :
                            level < 85 ? level * 5 + 35 :
                            level < 88 ? level * 5 + 35 + (level - 84) * 10 :
                                         level * 5 + 35 + (level - 85) * 15;
        return uint32(std::max(value, 1));
    }
}

LootTarget::LootTarget(ObjectGuid guid) : guid(guid), asOfTime(time(nullptr)) {}

void LootTargetList::shrink(time_t fromTime)
{
    for (auto i = begin(); i != end();)
    {
        if (i->asOfTime <= fromTime)
            i = erase(i);
        else
            ++i;
    }
}

LootObject::LootObject(Player* bot, ObjectGuid guid) { Refresh(bot, guid); }

void LootObject::Refresh(Player* bot, ObjectGuid lootGuid)
{
    skillId = SKILL_NONE;
    reqSkillValue = 0;
    reqItem = 0;
    guid.Clear();

    if (lootGuid.IsCreatureOrVehicle())
    {
        Creature* creature = ObjectAccessor::GetCreature(*bot, lootGuid);
        if (!creature || creature->IsAlive())
            return;

        if (creature->HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE) && bot->IsAllowedToLoot(creature))
        {
            guid = lootGuid;
            return;
        }

        // Skinning, or herbalism, mining and engineering on creatures that allow it, once the corpse is looted
        if (creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE) && creature->loot.isLooted())
        {
            uint32 const skill = creature->GetCreatureTemplate()->GetRequiredLootSkill();
            uint32 const required = SkinningSkillRequired(creature->GetLevel());
            if (bot->HasSkill(skill) && bot->GetSkillValue(skill) >= required)
            {
                guid = lootGuid;
                skillId = skill;
                reqSkillValue = required;
            }
        }

        return;
    }

    if (!lootGuid.IsGameObject())
        return;

    GameObject* go = ObjectAccessor::GetGameObject(*bot, lootGuid);
    if (!go || !go->isSpawned() || go->GetGoState() != GO_STATE_READY || go->GetGoType() != GAMEOBJECT_TYPE_CHEST)
        return;

    // Raid chests before their event is done, quest objects for quests the bot doesn't have
    if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE | GO_FLAG_IN_USE))
        return;
    if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND) && !go->ActivateToQuest(bot))
        return;

    uint32 const lockId = go->GetGOInfo()->GetLockId();
    LockEntry const* lock = lockId ? sLockStore.LookupEntry(lockId) : nullptr;
    if (!lock)
    {
        guid = lootGuid;
        return;
    }

    for (uint32 i = 0; i < MAX_LOCK_CASE; ++i)
    {
        switch (lock->Type[i])
        {
            case LOCK_KEY_ITEM:
                if (lock->Index[i])
                {
                    reqItem = lock->Index[i];
                    guid = lootGuid;
                }
                break;
            case LOCK_KEY_SKILL:
            {
                uint32 const skill = SkillByLockType(LockType(lock->Index[i]));
                if (skill != SKILL_NONE)
                {
                    skillId = skill;
                    reqSkillValue = std::max(1u, lock->Skill[i]);
                }
                guid = lootGuid;
                break;
            }
            case LOCK_KEY_NONE:
                guid = lootGuid;
                break;
            default:
                break;
        }
    }
}

WorldObject* LootObject::GetWorldObject(Player* bot)
{
    Refresh(bot, guid);
    if (guid.IsEmpty())
        return nullptr;

    if (guid.IsGameObject())
        return ObjectAccessor::GetGameObject(*bot, guid);

    return ObjectAccessor::GetCreature(*bot, guid);
}

bool LootObject::IsLootPossible(Player* bot)
{
    if (IsEmpty() || !bot)
        return false;

    WorldObject* object = GetWorldObject(bot);
    if (!object || !object->IsInWorld())
        return false;

    if (reqItem && !bot->HasItemCount(reqItem, 1))
        return false;

    if (std::abs(object->GetPositionZ() - bot->GetPositionZ()) > INTERACTION_DISTANCE - 2.0f)
        return false;

    if (skillId == SKILL_NONE)
        return true;

    // Fishing pools need a pole and casting; archaeology finds are another system
    if (skillId == SKILL_FISHING || skillId == SKILL_ARCHAEOLOGY)
        return false;

    return bot->HasSkill(skillId) && bot->GetSkillValue(skillId) >= reqSkillValue;
}

bool LootObjectStack::Add(ObjectGuid guid)
{
    if (availableLoot.size() >= MAX_LOOT_OBJECT_COUNT)
        availableLoot.shrink(time(nullptr) - LOOT_TARGET_LIFETIME);

    if (availableLoot.size() >= MAX_LOOT_OBJECT_COUNT)
        availableLoot.clear();

    return availableLoot.insert(LootTarget(guid)).second;
}

void LootObjectStack::Remove(ObjectGuid guid)
{
    auto i = availableLoot.find(LootTarget(guid));
    if (i != availableLoot.end())
        availableLoot.erase(i);
}

bool LootObjectStack::CanLoot(float maxDistance) { return !GetNearest(maxDistance).IsEmpty(); }

LootObject LootObjectStack::GetLoot(float maxDistance) { return GetNearest(maxDistance); }

LootObject LootObjectStack::GetNearest(float maxDistance)
{
    availableLoot.shrink(time(nullptr) - LOOT_TARGET_LIFETIME);

    LootObject nearest;
    float nearestDistance = std::numeric_limits<float>::max();

    // IsLootPossible refreshes objects; iterate a copy since nothing here may change the list
    LootTargetList const targets(availableLoot);
    for (LootTarget const& target : targets)
    {
        WorldObject* object = ObjectAccessor::GetWorldObject(*bot, target.guid);
        if (!object)
            continue;

        float const distance = bot->GetDistance(object);
        if (distance >= nearestDistance || (maxDistance && distance > maxDistance))
            continue;

        LootObject lootObject(bot, target.guid);
        if (!lootObject.IsLootPossible(bot))
            continue;

        nearestDistance = distance;
        nearest = lootObject;
    }

    return nearest;
}
