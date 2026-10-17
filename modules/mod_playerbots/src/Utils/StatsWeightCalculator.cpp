#include "StatsWeightCalculator.h"

#include "DBCStores.h"
#include "Item.h"
#include "ItemEnchantmentMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SpellAuraDefines.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

namespace
{
    // MoP caps: 7.5% hit and expertise for melee and ranged, 15% spell hit, 15% expertise for tanks (parry)
    constexpr float MELEE_HIT_CAP = 7.5f;
    constexpr float SPELL_HIT_CAP = 15.0f;
    constexpr float EXPERTISE_CAP = 7.5f;
    constexpr float TANK_EXPERTISE_CAP = 15.0f;

    // Value of a proc whose internal cooldown we don't know: assume it is up this share of the time
    constexpr float UNKNOWN_PROC_COVERAGE = 0.25f;
    constexpr uint32 MAX_SPELL_DEPTH = 2;

    bool IsTwoHand(ItemTemplate const* proto) { return proto->InventoryType == INVTYPE_2HWEAPON; }

    bool IsRangedWeapon(ItemTemplate const* proto)
    {
        return proto->Class == ITEM_CLASS_WEAPON &&
               (proto->SubClass == ITEM_SUBCLASS_WEAPON_BOW || proto->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
                proto->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW);
    }

    float WeaponDps(ItemTemplate const* proto)
    {
        if (proto->DPS > 0.0f)
            return proto->DPS;

        if (!proto->Delay)
            return 0.0f;

        return (proto->DamageMin + proto->DamageMax) / 2.0f / (proto->Delay / 1000.0f);
    }
}

StatsWeightCalculator::Role StatsWeightCalculator::GetRole(Player* player)
{
    switch (player->GetSpecialization())
    {
        case SPEC_WARRIOR_PROTECTION:
        case SPEC_PALADIN_PROTECTION:
        case SPEC_DEATH_KNIGHT_BLOOD:
            return ROLE_STRENGTH_TANK;
        case SPEC_DRUID_GUARDIAN:
        case SPEC_MONK_BREWMASTER:
            return ROLE_AGILITY_TANK;
        case SPEC_WARRIOR_ARMS:
        case SPEC_WARRIOR_FURY:
        case SPEC_PALADIN_RETRIBUTION:
        case SPEC_DEATH_KNIGHT_FROST:
        case SPEC_DEATH_KNIGHT_UNHOLY:
            return ROLE_STRENGTH_MELEE;
        case SPEC_ROGUE_ASSASSINATION:
        case SPEC_ROGUE_COMBAT:
        case SPEC_ROGUE_SUBTLETY:
        case SPEC_DRUID_FERAL:
        case SPEC_SHAMAN_ENHANCEMENT:
        case SPEC_MONK_WINDWALKER:
            return ROLE_AGILITY_MELEE;
        case SPEC_HUNTER_BEAST_MASTERY:
        case SPEC_HUNTER_MARKSMANSHIP:
        case SPEC_HUNTER_SURVIVAL:
            return ROLE_HUNTER;
        case SPEC_PALADIN_HOLY:
        case SPEC_PRIEST_DISCIPLINE:
        case SPEC_PRIEST_HOLY:
        case SPEC_DRUID_RESTORATION:
        case SPEC_SHAMAN_RESTORATION:
        case SPEC_MONK_MISTWEAVER:
            return ROLE_HEALER;
        case SPEC_MAGE_ARCANE:
        case SPEC_MAGE_FIRE:
        case SPEC_MAGE_FROST:
        case SPEC_PRIEST_SHADOW:
        case SPEC_DRUID_BALANCE:
        case SPEC_SHAMAN_ELEMENTAL:
        case SPEC_WARLOCK_AFFLICTION:
        case SPEC_WARLOCK_DEMONOLOGY:
        case SPEC_WARLOCK_DESTRUCTION:
            return ROLE_CASTER;
        default:
            break;
    }

    // No specialization before level 10
    switch (player->GetClass())
    {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
        case CLASS_DEATH_KNIGHT:
            return ROLE_STRENGTH_MELEE;
        case CLASS_ROGUE:
        case CLASS_MONK:
            return ROLE_AGILITY_MELEE;
        case CLASS_HUNTER:
            return ROLE_HUNTER;
        default:
            return ROLE_CASTER;
    }
}

StatsWeightCalculator::StatsWeightCalculator(Player* player) : player(player), role(GetRole(player))
{
    Specializations const spec = player->GetSpecialization();
    // Balance of Power, Spiritual Precision and Elemental Precision turn spirit into hit
    spiritIsHit = spec == SPEC_DRUID_BALANCE || spec == SPEC_PRIEST_SHADOW || spec == SPEC_SHAMAN_ELEMENTAL;
    InitWeights();
}

void StatsWeightCalculator::InitWeights()
{
    for (float& weight : weights)
        weight = 0.0f;

    weights[WEIGHT_STAMINA] = 0.05f;
    weights[WEIGHT_ARMOR] = 0.001f;
    weights[WEIGHT_PVP] = 0.05f;

    switch (role)
    {
        case ROLE_STRENGTH_TANK:
            weights[WEIGHT_STAMINA] = 1.0f;
            weights[WEIGHT_STRENGTH] = 0.5f;
            weights[WEIGHT_MASTERY] = 0.6f;
            weights[WEIGHT_DODGE] = 0.5f;
            weights[WEIGHT_PARRY] = 0.5f;
            weights[WEIGHT_HIT] = 0.4f;
            weights[WEIGHT_EXPERTISE] = 0.4f;
            weights[WEIGHT_HASTE] = 0.25f;
            weights[WEIGHT_CRIT] = 0.1f;
            weights[WEIGHT_ARMOR] = 0.02f;
            weights[WEIGHT_BONUS_ARMOR] = 0.05f;
            weights[WEIGHT_ATTACK_POWER] = 0.2f;
            weights[WEIGHT_MELEE_DPS] = 0.5f;
            break;
        case ROLE_AGILITY_TANK:
            weights[WEIGHT_AGILITY] = 1.0f;
            weights[WEIGHT_STAMINA] = 0.8f;
            weights[WEIGHT_MASTERY] = 0.5f;
            weights[WEIGHT_DODGE] = 0.5f;
            weights[WEIGHT_HIT] = 0.4f;
            weights[WEIGHT_EXPERTISE] = 0.4f;
            weights[WEIGHT_CRIT] = 0.3f;
            weights[WEIGHT_HASTE] = 0.3f;
            weights[WEIGHT_ARMOR] = 0.02f;
            weights[WEIGHT_BONUS_ARMOR] = 0.05f;
            weights[WEIGHT_ATTACK_POWER] = 0.3f;
            weights[WEIGHT_MELEE_DPS] = 0.5f;
            break;
        case ROLE_STRENGTH_MELEE:
        case ROLE_AGILITY_MELEE:
            weights[role == ROLE_STRENGTH_MELEE ? WEIGHT_STRENGTH : WEIGHT_AGILITY] = 1.0f;
            weights[WEIGHT_HIT] = 0.5f;
            weights[WEIGHT_EXPERTISE] = 0.45f;
            weights[WEIGHT_CRIT] = 0.4f;
            weights[WEIGHT_HASTE] = 0.35f;
            weights[WEIGHT_MASTERY] = 0.35f;
            weights[WEIGHT_ATTACK_POWER] = 0.4f;
            weights[WEIGHT_MELEE_DPS] = 3.0f;
            break;
        case ROLE_HUNTER:
            weights[WEIGHT_AGILITY] = 1.0f;
            weights[WEIGHT_HIT] = 0.5f;
            weights[WEIGHT_EXPERTISE] = 0.45f;
            weights[WEIGHT_CRIT] = 0.4f;
            weights[WEIGHT_HASTE] = 0.35f;
            weights[WEIGHT_MASTERY] = 0.35f;
            weights[WEIGHT_ATTACK_POWER] = 0.4f;
            weights[WEIGHT_RANGED_DPS] = 3.0f;
            break;
        case ROLE_CASTER:
            weights[WEIGHT_INTELLECT] = 1.0f;
            weights[WEIGHT_SPELL_POWER] = 0.8f;
            weights[WEIGHT_HIT] = 0.7f;
            weights[WEIGHT_SPIRIT] = spiritIsHit ? 0.7f : 0.0f;
            weights[WEIGHT_HASTE] = 0.5f;
            weights[WEIGHT_CRIT] = 0.4f;
            weights[WEIGHT_MASTERY] = 0.4f;
            weights[WEIGHT_MANA_REGEN] = 0.1f;
            break;
        case ROLE_HEALER:
            weights[WEIGHT_INTELLECT] = 1.0f;
            weights[WEIGHT_SPELL_POWER] = 0.8f;
            weights[WEIGHT_SPIRIT] = 0.5f;
            weights[WEIGHT_HASTE] = 0.4f;
            weights[WEIGHT_CRIT] = 0.35f;
            weights[WEIGHT_MASTERY] = 0.35f;
            weights[WEIGHT_MANA_REGEN] = 0.5f;
            break;
    }

    weights[WEIGHT_HIT] *= CapMultiplier(WEIGHT_HIT);
    weights[WEIGHT_EXPERTISE] *= CapMultiplier(WEIGHT_EXPERTISE);
    if (spiritIsHit)
        weights[WEIGHT_SPIRIT] *= CapMultiplier(WEIGHT_HIT);
}

float StatsWeightCalculator::CapMultiplier(Stat stat) const
{
    float current = 0.0f;
    float cap = 0.0f;
    if (stat == WEIGHT_HIT)
    {
        if (role == ROLE_CASTER)
        {
            current = player->GetRatingBonusValue(CR_HIT_SPELL) + player->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
            cap = SPELL_HIT_CAP;
        }
        else
        {
            current = player->GetRatingBonusValue(role == ROLE_HUNTER ? CR_HIT_RANGED : CR_HIT_MELEE) +
                      player->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
            cap = MELEE_HIT_CAP;
        }
    }
    else if (stat == WEIGHT_EXPERTISE)
    {
        current = player->GetExpertiseDodgeOrParryReduction(role == ROLE_HUNTER ? RANGED_ATTACK : BASE_ATTACK);
        cap = role == ROLE_STRENGTH_TANK || role == ROLE_AGILITY_TANK ? TANK_EXPERTISE_CAP : EXPERTISE_CAP;
    }
    else
        return 1.0f;

    // Over the cap the rating does nothing; keep a little so capped items still beat empty ones
    return current >= cap ? 0.1f : 1.0f;
}

float StatsWeightCalculator::CalculateItem(uint32 itemId, int32 randomPropertyId)
{
    return CalculateItem(sObjectMgr->GetItemTemplate(itemId), randomPropertyId);
}

float StatsWeightCalculator::CalculateItem(Item* item)
{
    if (!item)
        return 0.0f;

    return CalculateItem(item->GetTemplate(), item->GetItemRandomPropertyId());
}

float StatsWeightCalculator::CalculateItem(ItemTemplate const* proto, int32 randomPropertyId)
{
    if (!proto)
        return 0.0f;

    for (float& stat : stats)
        stat = 0.0f;

    Collect(proto, randomPropertyId);

    float score = 0.0f;
    for (uint32 i = 0; i < WEIGHT_MAX; ++i)
        score += weights[i] * stats[i];

    if (score <= 0.0f)
        return 0.0f;

    uint32 sockets = 0;
    for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
        if (proto->Socket[i].Color)
            ++sockets;

    return score * (1.0f + sockets * 0.03f) * TypeMultiplier(proto);
}

void StatsWeightCalculator::Collect(ItemTemplate const* proto, int32 randomPropertyId)
{
    if (proto->Class == ITEM_CLASS_WEAPON)
        stats[IsRangedWeapon(proto) ? WEIGHT_RANGED_DPS : WEIGHT_MELEE_DPS] += WeaponDps(proto);

    stats[WEIGHT_ARMOR] += proto->Armor;

    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatValue)
            CollectItemStat(proto->ItemStat[i].ItemStatType, proto->ItemStat[i].ItemStatValue);

    for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        _Spell const& spell = proto->Spells[i];
        if (spell.SpellId <= 0)
            continue;

        switch (spell.SpellTrigger)
        {
            case ITEM_SPELLTRIGGER_ON_EQUIP:
                CollectSpell(spell.SpellId, 1.0f, 0);
                break;
            case ITEM_SPELLTRIGGER_ON_USE:
            {
                // An on-use effect is worth its share of uptime
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell.SpellId);
                if (!spellInfo || !spellInfo->IsPositive())
                    break;

                int32 const cooldown = std::max(spell.SpellCooldown, spell.SpellCategoryCooldown);
                int32 const duration = spellInfo->GetDuration();
                if (cooldown > 0 && duration > 0)
                    CollectSpell(spell.SpellId, std::min(1.0f, float(duration) / cooldown), 0);
                break;
            }
            case ITEM_SPELLTRIGGER_CHANCE_ON_HIT:
                if (role != ROLE_CASTER && role != ROLE_HEALER)
                    CollectSpell(spell.SpellId, UNKNOWN_PROC_COVERAGE, 0);
                break;
            default:
                break;
        }
    }

    if (proto->socketBonus)
        if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(proto->socketBonus))
            CollectEnchant(enchant, 0);

    if (randomPropertyId > 0)
    {
        if (ItemRandomPropertiesEntry const* properties = sItemRandomPropertiesStore.LookupEntry(randomPropertyId))
            for (uint32 i = 0; i < MAX_ITEM_ENCHANTMENT_EFFECTS; ++i)
                if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(properties->enchant_id[i]))
                    CollectEnchant(enchant, 0);
    }
    else if (randomPropertyId < 0)
    {
        if (ItemRandomSuffixEntry const* suffix = sItemRandomSuffixStore.LookupEntry(-randomPropertyId))
        {
            uint32 const factor = GenerateEnchSuffixFactor(proto->ItemId);
            for (uint32 i = 0; i < 5; ++i)
                if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(suffix->enchant_id[i]))
                    CollectEnchant(enchant, suffix->prefix[i] * factor / 10000);
        }
    }
}

void StatsWeightCalculator::CollectItemStat(uint32 statType, int32 value)
{
    float const val = float(value);
    switch (statType)
    {
        case ITEM_MOD_AGILITY:
            stats[WEIGHT_AGILITY] += val;
            break;
        case ITEM_MOD_STRENGTH:
            stats[WEIGHT_STRENGTH] += val;
            break;
        case ITEM_MOD_INTELLECT:
            stats[WEIGHT_INTELLECT] += val;
            break;
        case ITEM_MOD_SPIRIT:
            stats[WEIGHT_SPIRIT] += val;
            break;
        case ITEM_MOD_STAMINA:
            stats[WEIGHT_STAMINA] += val;
            break;
        case ITEM_MOD_HEALTH:
            stats[WEIGHT_STAMINA] += val / 14.0f;
            break;
        case ITEM_MOD_MANA:
        case ITEM_MOD_MANA_REGENERATION:
            stats[WEIGHT_MANA_REGEN] += val;
            break;
        case ITEM_MOD_DODGE_RATING:
            stats[WEIGHT_DODGE] += val;
            break;
        case ITEM_MOD_PARRY_RATING:
            stats[WEIGHT_PARRY] += val;
            break;
        case ITEM_MOD_HIT_MELEE_RATING:
        case ITEM_MOD_HIT_RANGED_RATING:
        case ITEM_MOD_HIT_SPELL_RATING:
        case ITEM_MOD_HIT_RATING:
            stats[WEIGHT_HIT] += val;
            break;
        case ITEM_MOD_CRIT_MELEE_RATING:
        case ITEM_MOD_CRIT_RANGED_RATING:
        case ITEM_MOD_CRIT_SPELL_RATING:
        case ITEM_MOD_CRIT_RATING:
            stats[WEIGHT_CRIT] += val;
            break;
        case ITEM_MOD_HASTE_MELEE_RATING:
        case ITEM_MOD_HASTE_RANGED_RATING:
        case ITEM_MOD_HASTE_SPELL_RATING:
        case ITEM_MOD_HASTE_RATING:
            stats[WEIGHT_HASTE] += val;
            break;
        case ITEM_MOD_EXPERTISE_RATING:
            stats[WEIGHT_EXPERTISE] += val;
            break;
        case ITEM_MOD_MASTERY_RATING:
            stats[WEIGHT_MASTERY] += val;
            break;
        case ITEM_MOD_ATTACK_POWER:
            stats[WEIGHT_ATTACK_POWER] += val;
            break;
        case ITEM_MOD_RANGED_ATTACK_POWER:
            if (role == ROLE_HUNTER)
                stats[WEIGHT_ATTACK_POWER] += val;
            break;
        case ITEM_MOD_SPELL_POWER:
            stats[WEIGHT_SPELL_POWER] += val;
            break;
        case ITEM_MOD_EXTRA_ARMOR:
            stats[WEIGHT_BONUS_ARMOR] += val;
            break;
        case ITEM_MOD_RESILIENCE_RATING:
        case ITEM_MOD_PVP_POWER:
            stats[WEIGHT_PVP] += val;
            break;
        default:
            break;
    }
}

void StatsWeightCalculator::CollectEnchant(SpellItemEnchantmentEntry const* enchant, uint32 suffixAmount)
{
    for (uint32 i = 0; i < MAX_ITEM_ENCHANTMENT_EFFECTS; ++i)
    {
        switch (enchant->Type[i])
        {
            case ITEM_ENCHANTMENT_TYPE_STAT:
            {
                uint32 const amount = enchant->Amount[i] ? enchant->Amount[i] : suffixAmount;
                if (amount)
                    CollectItemStat(enchant->SpellID[i], int32(amount));
                break;
            }
            case ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL:
                CollectSpell(enchant->SpellID[i], 1.0f, 0);
                break;
            case ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL:
                if (role != ROLE_CASTER && role != ROLE_HEALER)
                    CollectSpell(enchant->SpellID[i], UNKNOWN_PROC_COVERAGE, 0);
                break;
            default:
                break;
        }
    }
}

void StatsWeightCalculator::CollectRating(uint32 ratingMask, float value)
{
    if (ratingMask & ((1 << CR_HIT_MELEE) | (1 << CR_HIT_RANGED) | (1 << CR_HIT_SPELL)))
        stats[WEIGHT_HIT] += value;
    if (ratingMask & ((1 << CR_CRIT_MELEE) | (1 << CR_CRIT_RANGED) | (1 << CR_CRIT_SPELL)))
        stats[WEIGHT_CRIT] += value;
    if (ratingMask & ((1 << CR_HASTE_MELEE) | (1 << CR_HASTE_RANGED) | (1 << CR_HASTE_SPELL)))
        stats[WEIGHT_HASTE] += value;
    if (ratingMask & (1 << CR_DODGE))
        stats[WEIGHT_DODGE] += value;
    if (ratingMask & (1 << CR_PARRY))
        stats[WEIGHT_PARRY] += value;
    if (ratingMask & (1 << CR_EXPERTISE))
        stats[WEIGHT_EXPERTISE] += value;
    if (ratingMask & (1 << CR_MASTERY))
        stats[WEIGHT_MASTERY] += value;
    if (ratingMask & ((1 << CR_PVP_POWER) | (1 << CR_RESILIENCE_PLAYER_DAMAGE_TAKEN)))
        stats[WEIGHT_PVP] += value;
}

void StatsWeightCalculator::CollectSpell(uint32 spellId, float coverage, uint32 depth)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo || depth > MAX_SPELL_DEPTH || coverage <= 0.0f)
        return;

    // Form-only auras (feral idols and the like)
    if (spellInfo->Stances && player->GetClass() != CLASS_DRUID)
        return;

    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const& effect = spellInfo->Effects[i];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA)
            continue;

        float const value = effect.CalcValue(player) * coverage;
        switch (effect.ApplyAuraName)
        {
            case SPELL_AURA_MOD_STAT:
                switch (effect.MiscValue)
                {
                    case ::STAT_STRENGTH:
                        stats[WEIGHT_STRENGTH] += value;
                        break;
                    case ::STAT_AGILITY:
                        stats[WEIGHT_AGILITY] += value;
                        break;
                    case ::STAT_STAMINA:
                        stats[WEIGHT_STAMINA] += value;
                        break;
                    case ::STAT_INTELLECT:
                        stats[WEIGHT_INTELLECT] += value;
                        break;
                    case ::STAT_SPIRIT:
                        stats[WEIGHT_SPIRIT] += value;
                        break;
                    case -1:
                        stats[WEIGHT_STRENGTH] += value;
                        stats[WEIGHT_AGILITY] += value;
                        stats[WEIGHT_STAMINA] += value;
                        stats[WEIGHT_INTELLECT] += value;
                        stats[WEIGHT_SPIRIT] += value;
                        break;
                    default:
                        break;
                }
                break;
            case SPELL_AURA_MOD_RATING:
                CollectRating(uint32(effect.MiscValue), value);
                break;
            case SPELL_AURA_MOD_ATTACK_POWER:
                stats[WEIGHT_ATTACK_POWER] += value;
                break;
            case SPELL_AURA_MOD_RANGED_ATTACK_POWER:
                if (role == ROLE_HUNTER)
                    stats[WEIGHT_ATTACK_POWER] += value;
                break;
            case SPELL_AURA_MOD_DAMAGE_DONE:
            case SPELL_AURA_MOD_HEALING_DONE:
                if (effect.MiscValue & SPELL_SCHOOL_MASK_MAGIC)
                    stats[WEIGHT_SPELL_POWER] += value;
                break;
            case SPELL_AURA_MOD_RESISTANCE:
                if (effect.MiscValue & SPELL_SCHOOL_MASK_NORMAL)
                    stats[WEIGHT_BONUS_ARMOR] += value;
                break;
            case SPELL_AURA_MOD_INCREASE_HEALTH:
            case SPELL_AURA_MOD_INCREASE_HEALTH_2:
                stats[WEIGHT_STAMINA] += value / 14.0f;
                break;
            case SPELL_AURA_PROC_TRIGGER_SPELL:
            {
                // A proc is worth the share of time its buff is up: its duration over its duration plus internal cooldown
                SpellInfo const* triggered = sSpellMgr->GetSpellInfo(effect.TriggerSpell);
                if (!triggered || !triggered->IsPositive())
                    break;

                float procCoverage = UNKNOWN_PROC_COVERAGE;
                SpellProcEntry const* procEntry = sSpellMgr->GetSpellProcEntry(spellId);
                int32 const duration = triggered->GetDuration();
                if (procEntry && procEntry->cooldown && duration > 0)
                    procCoverage = float(duration) / (duration + procEntry->cooldown * IN_MILLISECONDS);

                CollectSpell(effect.TriggerSpell, coverage * procCoverage, depth + 1);
                break;
            }
            default:
                break;
        }
    }
}

uint32 StatsWeightCalculator::BestArmorSubClass() const
{
    if (player->HasSkill(SKILL_PLATE_MAIL))
        return ITEM_SUBCLASS_ARMOR_PLATE;
    if (player->HasSkill(SKILL_MAIL))
        return ITEM_SUBCLASS_ARMOR_MAIL;
    if (player->HasSkill(SKILL_LEATHER))
        return ITEM_SUBCLASS_ARMOR_LEATHER;
    return ITEM_SUBCLASS_ARMOR_CLOTH;
}

bool StatsWeightCalculator::IsPreferredWeapon(ItemTemplate const* proto) const
{
    if (proto->Class == ITEM_CLASS_ARMOR)
    {
        if (proto->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
            return role == ROLE_STRENGTH_TANK ? player->GetSpecialization() != SPEC_DEATH_KNIGHT_BLOOD
                                              : role == ROLE_CASTER || role == ROLE_HEALER;

        if (proto->InventoryType == INVTYPE_HOLDABLE)
            return role == ROLE_CASTER || role == ROLE_HEALER;

        return true;
    }

    if (proto->Class != ITEM_CLASS_WEAPON)
        return true;

    // Bows, guns and crossbows take the main hand in MoP: only hunters want them
    if (IsRangedWeapon(proto))
        return role == ROLE_HUNTER;
    if (role == ROLE_HUNTER)
        return false;

    if (proto->SubClass == ITEM_SUBCLASS_WEAPON_WAND)
        return role == ROLE_CASTER || role == ROLE_HEALER;

    Specializations const spec = player->GetSpecialization();
    switch (spec)
    {
        // Mutilate and Backstab need daggers
        case SPEC_ROGUE_ASSASSINATION:
        case SPEC_ROGUE_SUBTLETY:
            return proto->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER;
        // Two-handed specs
        case SPEC_WARRIOR_ARMS:
        case SPEC_PALADIN_RETRIBUTION:
        case SPEC_DEATH_KNIGHT_UNHOLY:
        case SPEC_DEATH_KNIGHT_BLOOD:
        case SPEC_DRUID_FERAL:
        case SPEC_DRUID_GUARDIAN:
            return IsTwoHand(proto);
        // One hand and a shield, or dual wielding
        case SPEC_WARRIOR_PROTECTION:
        case SPEC_PALADIN_PROTECTION:
        case SPEC_SHAMAN_ENHANCEMENT:
            return !IsTwoHand(proto);
        default:
            break;
    }

    return true;
}

float StatsWeightCalculator::TypeMultiplier(ItemTemplate const* proto) const
{
    float multiplier = 1.0f;

    // Items made for another role: an intellect helm with crit is not a strength user's upgrade
    bool const hasStrength = stats[WEIGHT_STRENGTH] > 0.0f;
    bool const hasAgility = stats[WEIGHT_AGILITY] > 0.0f;
    bool const hasIntellect = stats[WEIGHT_INTELLECT] > 0.0f;
    if (hasStrength || hasAgility || hasIntellect)
    {
        bool wanted;
        switch (role)
        {
            case ROLE_STRENGTH_TANK:
            case ROLE_STRENGTH_MELEE:
                wanted = hasStrength;
                break;
            case ROLE_AGILITY_TANK:
            case ROLE_AGILITY_MELEE:
            case ROLE_HUNTER:
                wanted = hasAgility;
                break;
            default:
                wanted = hasIntellect;
                break;
        }

        if (!wanted)
            multiplier *= 0.1f;
    }

    // Wear the best armor type the class can: from level 50 armor specialization depends on it
    if (proto->Class == ITEM_CLASS_ARMOR && proto->InventoryType != INVTYPE_CLOAK &&
        proto->SubClass >= ITEM_SUBCLASS_ARMOR_CLOTH && proto->SubClass <= ITEM_SUBCLASS_ARMOR_PLATE &&
        proto->SubClass < BestArmorSubClass())
        multiplier *= player->GetLevel() >= 50 ? 0.3f : 0.7f;

    if (!IsPreferredWeapon(proto))
        multiplier *= 0.1f;

    return multiplier;
}
