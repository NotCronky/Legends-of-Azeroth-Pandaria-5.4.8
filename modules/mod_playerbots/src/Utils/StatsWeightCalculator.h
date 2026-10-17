#ifndef _PLAYERBOT_STATSWEIGHTCALCULATOR_H
#define _PLAYERBOT_STATSWEIGHTCALCULATOR_H

#include "Define.h"

class Item;
class Player;
class SpellInfo;

struct ItemTemplate;
struct SpellItemEnchantmentEntry;

// How much an item is worth to a bot in its current specialization, from MoP stat weights. Scores are only
// meaningful compared with each other for the same bot; 0 means the item is of no use to it
class StatsWeightCalculator
{
public:
    enum Stat
    {
        WEIGHT_STRENGTH,
        WEIGHT_AGILITY,
        WEIGHT_INTELLECT,
        WEIGHT_SPIRIT,
        WEIGHT_STAMINA,
        WEIGHT_HIT,
        WEIGHT_EXPERTISE,
        WEIGHT_CRIT,
        WEIGHT_HASTE,
        WEIGHT_MASTERY,
        WEIGHT_DODGE,
        WEIGHT_PARRY,
        WEIGHT_ARMOR,
        WEIGHT_BONUS_ARMOR,
        WEIGHT_ATTACK_POWER,
        WEIGHT_SPELL_POWER,
        WEIGHT_MELEE_DPS,
        WEIGHT_RANGED_DPS,
        WEIGHT_PVP,
        WEIGHT_MANA_REGEN,
        WEIGHT_MAX
    };

    enum Role
    {
        ROLE_STRENGTH_TANK,
        ROLE_AGILITY_TANK,
        ROLE_STRENGTH_MELEE,
        ROLE_AGILITY_MELEE,
        ROLE_HUNTER,
        ROLE_CASTER,
        ROLE_HEALER
    };

    explicit StatsWeightCalculator(Player* player);

    float CalculateItem(ItemTemplate const* proto, int32 randomPropertyId = 0);
    float CalculateItem(Item* item);
    float CalculateItem(uint32 itemId, int32 randomPropertyId = 0);

    Role GetRole() const { return role; }
    // Whether the role wants this item class/subclass in its hands at all (shields, two-handers, daggers, ...)
    bool IsPreferredWeapon(ItemTemplate const* proto) const;
    static Role GetRole(Player* player);

private:
    void InitWeights();
    void Collect(ItemTemplate const* proto, int32 randomPropertyId);
    void CollectItemStat(uint32 statType, int32 value);
    void CollectEnchant(SpellItemEnchantmentEntry const* enchant, uint32 suffixAmount);
    void CollectSpell(uint32 spellId, float coverage, uint32 depth);
    void CollectRating(uint32 ratingMask, float value);
    float TypeMultiplier(ItemTemplate const* proto) const;
    float CapMultiplier(Stat stat) const;
    uint32 BestArmorSubClass() const;

    Player* player;
    Role role;
    bool spiritIsHit;
    float weights[WEIGHT_MAX];
    float stats[WEIGHT_MAX];
};

#endif
