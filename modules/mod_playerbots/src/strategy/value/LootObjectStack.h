/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_LOOTOBJECTSTACK_H
#define _PLAYERBOT_LOOTOBJECTSTACK_H

#include <set>
#include <string>

#include "ObjectGuid.h"

class AiObjectContext;
class Player;
class WorldObject;

struct ItemTemplate;

// Which items the bot picks up (loot strategy "normal", "gray", "disenchant", "all")
class LootStrategy
{
public:
    virtual ~LootStrategy() {}
    virtual bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) = 0;
    virtual std::string const GetName() = 0;
};

// A corpse, chest or gathering node and what it takes to loot it
class LootObject
{
public:
    LootObject() {}
    LootObject(Player* bot, ObjectGuid guid);

    bool IsEmpty() const { return guid.IsEmpty(); }
    bool IsLootPossible(Player* bot);
    void Refresh(Player* bot, ObjectGuid guid);
    WorldObject* GetWorldObject(Player* bot);

    ObjectGuid guid;
    uint32 skillId = 0;
    uint32 reqSkillValue = 0;
    uint32 reqItem = 0;
};

class LootTarget
{
public:
    LootTarget(ObjectGuid guid);

    bool operator<(LootTarget const& other) const { return guid < other.guid; }

    ObjectGuid guid;
    time_t asOfTime;
};

class LootTargetList : public std::set<LootTarget>
{
public:
    void shrink(time_t fromTime);
};

// Corpses and objects seen recently that the bot may loot
class LootObjectStack
{
public:
    LootObjectStack(Player* bot) : bot(bot) {}

    bool Add(ObjectGuid guid);
    void Remove(ObjectGuid guid);
    void Clear() { availableLoot.clear(); }
    bool CanLoot(float maxDistance);
    LootObject GetLoot(float maxDistance = 0);

private:
    LootObject GetNearest(float maxDistance);

    Player* bot;
    LootTargetList availableLoot;
};

#endif
