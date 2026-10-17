#include "LootValues.h"

#include "CellImpl.h"
#include "GridObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Helper.h"
#include "ItemUsageValue.h"
#include "Playerbots.h"
#include "ServerFacade.h"

namespace
{
    // Picks up what the bot has a use for, including selling
    class NormalLootStrategy : public LootStrategy
    {
    public:
        bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
        {
            return AI_VALUE2(ItemUsage, "item usage", ItemUsageValue::Qualify(proto->ItemId)) != ITEM_USAGE_NONE;
        }

        std::string const GetName() override { return "normal"; }
    };

    class GrayLootStrategy : public NormalLootStrategy
    {
    public:
        bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
        {
            return NormalLootStrategy::CanLoot(proto, context) || proto->Quality == ITEM_QUALITY_POOR;
        }

        std::string const GetName() override { return "gray"; }
    };

    class DisenchantLootStrategy : public NormalLootStrategy
    {
    public:
        bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
        {
            return NormalLootStrategy::CanLoot(proto, context) ||
                   (proto->Quality >= ITEM_QUALITY_UNCOMMON && proto->Bonding != BIND_ON_ACQUIRE &&
                    (proto->Class == ITEM_CLASS_ARMOR || proto->Class == ITEM_CLASS_WEAPON));
        }

        std::string const GetName() override { return "disenchant"; }
    };

    class AllLootStrategy : public LootStrategy
    {
    public:
        bool CanLoot(ItemTemplate const* /*proto*/, AiObjectContext* /*context*/) override { return true; }

        std::string const GetName() override { return "all"; }
    };

    struct DeadCreatureInRangeCheck
    {
        DeadCreatureInRangeCheck(WorldObject const* source, float range) : source(source), range(range) {}

        bool operator()(Creature* creature) const
        {
            return !creature->IsAlive() && source->IsWithinDistInMap(creature, range);
        }

        WorldObject const* source;
        float range;
    };

    struct ChestInRangeCheck
    {
        ChestInRangeCheck(WorldObject const* source, float range) : source(source), range(range) {}

        bool operator()(GameObject* go) const
        {
            return go->GetGoType() == GAMEOBJECT_TYPE_CHEST && go->isSpawned() &&
                   source->IsWithinDistInMap(go, range);
        }

        WorldObject const* source;
        float range;
    };
}

LootStrategy* LootStrategyValue::normal = new NormalLootStrategy();
LootStrategy* LootStrategyValue::gray = new GrayLootStrategy();
LootStrategy* LootStrategyValue::disenchant = new DisenchantLootStrategy();
LootStrategy* LootStrategyValue::all = new AllLootStrategy();

AvailableLootValue::AvailableLootValue(PlayerbotAI* botAI, std::string const name)
    : ManualSetValue<LootObjectStack*>(botAI, nullptr, name)
{
    value = new LootObjectStack(botAI->GetBot());
    defaultValue = value;
}

AvailableLootValue::~AvailableLootValue() { delete value; }

bool CanLootValue::Calculate()
{
    LootObject loot = AI_VALUE(LootObject, "loot target");
    return !loot.IsEmpty() && loot.IsLootPossible(bot) &&
           sServerFacade->IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", "loot target"), INTERACTION_DISTANCE - 2.0f);
}

bool HasAvailableLootValue::Calculate()
{
    return !AI_VALUE(bool, "can loot") && AI_VALUE(LootObjectStack*, "available loot")->CanLoot(sPlayerbotAIConfig->lootDistance);
}

LootStrategy* LootStrategyValue::instance(std::string const name)
{
    if (name == "*" || name == "all")
        return all;

    if (name == "g" || name == "gray" || name == "grey")
        return gray;

    if (name == "d" || name == "e" || name == "disenchant" || name == "enchant")
        return disenchant;

    return normal;
}

std::string const LootStrategyValue::Save() { return value ? value->GetName() : "?"; }

bool LootStrategyValue::Load(std::string const text)
{
    value = instance(text);
    return true;
}

std::string const AlwaysLootListValue::Save()
{
    std::ostringstream out;
    bool first = true;
    for (uint32 itemId : list)
    {
        if (!first)
            out << ",";
        out << itemId;
        first = false;
    }

    return out.str();
}

bool AlwaysLootListValue::Load(std::string const text)
{
    list.clear();
    for (std::string const& id : split(text, ','))
        if (uint32 const itemId = uint32(atoi(id.c_str())))
            list.insert(itemId);

    return true;
}

GuidVector NearestCorpsesValue::Calculate()
{
    std::list<Creature*> corpses;
    DeadCreatureInRangeCheck check(bot, range);
    Trinity::CreatureListSearcher<DeadCreatureInRangeCheck> searcher(bot, corpses, check);
    Cell::VisitAllObjects(bot, searcher, range);

    GuidVector result;
    for (Creature* creature : corpses)
        result.push_back(creature->GetGUID());

    return result;
}

GuidVector NearestLootableObjectsValue::Calculate()
{
    std::list<GameObject*> objects;
    ChestInRangeCheck check(bot, range);
    Trinity::GameObjectListSearcher<ChestInRangeCheck> searcher(bot, objects, check);
    Cell::VisitAllObjects(bot, searcher, range);

    GuidVector result;
    for (GameObject* go : objects)
        result.push_back(go->GetGUID());

    return result;
}
