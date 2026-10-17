#ifndef _PLAYERBOT_LOOTVALUES_H
#define _PLAYERBOT_LOOTVALUES_H

#include <set>

#include "LootObjectStack.h"
#include "PlayerbotAIConfig.h"
#include "Value.h"

class PlayerbotAI;

class AvailableLootValue : public ManualSetValue<LootObjectStack*>
{
public:
    AvailableLootValue(PlayerbotAI* botAI, std::string const name = "available loot");
    ~AvailableLootValue() override;
};

class LootTargetValue : public ManualSetValue<LootObject>
{
public:
    LootTargetValue(PlayerbotAI* botAI, std::string const name = "loot target")
        : ManualSetValue<LootObject>(botAI, LootObject(), name)
    {
    }
};

// The loot target is in reach and can be looted
class CanLootValue : public BoolCalculatedValue
{
public:
    CanLootValue(PlayerbotAI* botAI) : BoolCalculatedValue(botAI, "can loot") {}

    bool Calculate() override;
};

// Something in the available loot can be looted, other than the target in reach
class HasAvailableLootValue : public BoolCalculatedValue
{
public:
    HasAvailableLootValue(PlayerbotAI* botAI) : BoolCalculatedValue(botAI, "has available loot") {}

    bool Calculate() override;
};

class LootStrategyValue : public ManualSetValue<LootStrategy*>
{
public:
    LootStrategyValue(PlayerbotAI* botAI) : ManualSetValue<LootStrategy*>(botAI, normal, "loot strategy") {}

    std::string const Save() override;
    bool Load(std::string const text) override;

    static LootStrategy* instance(std::string const name);

    static LootStrategy* normal;
    static LootStrategy* gray;
    static LootStrategy* disenchant;
    static LootStrategy* all;
};

// Items the bot always loots, whatever the loot strategy
class AlwaysLootListValue : public ManualSetValue<std::set<uint32>&>
{
public:
    AlwaysLootListValue(PlayerbotAI* botAI) : ManualSetValue<std::set<uint32>&>(botAI, list, "always loot list") {}

    std::string const Save() override;
    bool Load(std::string const text) override;

private:
    std::set<uint32> list;
};

// Dead creatures in loot range
class NearestCorpsesValue : public ObjectGuidListCalculatedValue
{
public:
    NearestCorpsesValue(PlayerbotAI* botAI, float range = sPlayerbotAIConfig->lootDistance)
        : ObjectGuidListCalculatedValue(botAI, "nearest corpses", 1), range(range)
    {
    }

    GuidVector Calculate() override;

private:
    float range;
};

// Chests, herbs and ore in loot range
class NearestLootableObjectsValue : public ObjectGuidListCalculatedValue
{
public:
    NearestLootableObjectsValue(PlayerbotAI* botAI, float range = sPlayerbotAIConfig->lootDistance)
        : ObjectGuidListCalculatedValue(botAI, "nearest game objects", 1), range(range)
    {
    }

    GuidVector Calculate() override;

private:
    float range;
};

#endif
