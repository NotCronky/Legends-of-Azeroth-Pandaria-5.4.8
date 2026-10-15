/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ChatHelper.h"

#include <cstdio>
#include <map>
#include <sstream>

#include "Playerbots.h"
#include "QuestDef.h"
#include "SpellInfo.h"

namespace
{
    std::map<std::string, uint32> const itemQualities =
    {
        { "poor", ITEM_QUALITY_POOR }, { "gray", ITEM_QUALITY_POOR }, { "grey", ITEM_QUALITY_POOR },
        { "normal", ITEM_QUALITY_NORMAL }, { "common", ITEM_QUALITY_NORMAL }, { "white", ITEM_QUALITY_NORMAL },
        { "uncommon", ITEM_QUALITY_UNCOMMON }, { "green", ITEM_QUALITY_UNCOMMON },
        { "rare", ITEM_QUALITY_RARE }, { "blue", ITEM_QUALITY_RARE },
        { "epic", ITEM_QUALITY_EPIC }, { "violet", ITEM_QUALITY_EPIC }, { "purple", ITEM_QUALITY_EPIC },
        { "legendary", ITEM_QUALITY_LEGENDARY }, { "orange", ITEM_QUALITY_LEGENDARY },
        { "artifact", ITEM_QUALITY_ARTIFACT },
        { "heirloom", ITEM_QUALITY_HEIRLOOM },
    };

    std::map<std::string, uint32> const consumableSubClasses =
    {
        { "potion", ITEM_SUBCLASS_POTION },
        { "elixir", ITEM_SUBCLASS_ELIXIR },
        { "flask", ITEM_SUBCLASS_FLASK },
        { "scroll", ITEM_SUBCLASS_SCROLL },
        { "food", ITEM_SUBCLASS_FOOD_DRINK },
        { "bandage", ITEM_SUBCLASS_BANDAGE },
        { "enchant", ITEM_SUBCLASS_ITEM_ENHANCEMENT },
    };

    std::map<std::string, uint32> const tradeSubClasses =
    {
        { "cloth", ITEM_SUBCLASS_CLOTH },
        { "leather", ITEM_SUBCLASS_LEATHER },
        { "metal", ITEM_SUBCLASS_METAL_STONE }, { "stone", ITEM_SUBCLASS_METAL_STONE }, { "ore", ITEM_SUBCLASS_METAL_STONE },
        { "meat", ITEM_SUBCLASS_MEAT },
        { "herb", ITEM_SUBCLASS_HERB },
        { "elemental", ITEM_SUBCLASS_ELEMENTAL },
        { "enchanting", ITEM_SUBCLASS_ENCHANTING }, { "disenchants", ITEM_SUBCLASS_ENCHANTING },
        { "parts", ITEM_SUBCLASS_PARTS },
    };

    std::map<std::string, uint32> const slots =
    {
        { "head", EQUIPMENT_SLOT_HEAD },
        { "neck", EQUIPMENT_SLOT_NECK },
        { "shoulder", EQUIPMENT_SLOT_SHOULDERS }, { "shoulders", EQUIPMENT_SLOT_SHOULDERS },
        { "shirt", EQUIPMENT_SLOT_BODY },
        { "chest", EQUIPMENT_SLOT_CHEST },
        { "waist", EQUIPMENT_SLOT_WAIST }, { "belt", EQUIPMENT_SLOT_WAIST },
        { "legs", EQUIPMENT_SLOT_LEGS },
        { "feet", EQUIPMENT_SLOT_FEET },
        { "wrist", EQUIPMENT_SLOT_WRISTS }, { "wrists", EQUIPMENT_SLOT_WRISTS },
        { "hands", EQUIPMENT_SLOT_HANDS },
        { "finger 1", EQUIPMENT_SLOT_FINGER1 }, { "finger 2", EQUIPMENT_SLOT_FINGER2 },
        { "trinket 1", EQUIPMENT_SLOT_TRINKET1 }, { "trinket 2", EQUIPMENT_SLOT_TRINKET2 },
        { "back", EQUIPMENT_SLOT_BACK },
        { "main hand", EQUIPMENT_SLOT_MAINHAND },
        { "off hand", EQUIPMENT_SLOT_OFFHAND },
        { "tabard", EQUIPMENT_SLOT_TABARD },
    };

    std::map<std::string, uint32> const skills =
    {
        { "first aid", SKILL_FIRST_AID },
        { "fishing", SKILL_FISHING },
        { "cooking", SKILL_COOKING },
        { "archaeology", SKILL_ARCHAEOLOGY },
        { "alchemy", SKILL_ALCHEMY },
        { "enchanting", SKILL_ENCHANTING },
        { "engineering", SKILL_ENGINEERING },
        { "leatherworking", SKILL_LEATHERWORKING },
        { "blacksmithing", SKILL_BLACKSMITHING },
        { "tailoring", SKILL_TAILORING },
        { "herbalism", SKILL_HERBALISM },
        { "mining", SKILL_MINING },
        { "skinning", SKILL_SKINNING },
        { "jewelcrafting", SKILL_JEWELCRAFTING },
        { "inscription", SKILL_INSCRIPTION },
    };

    // Numbers following each occurrence of prefix, e.g. "Hitem:" in "|Hitem:6948:0:...|h"
    std::set<uint32> ParseLinkIds(std::string const& text, std::string const& prefix)
    {
        std::set<uint32> ids;
        for (size_t pos = text.find(prefix); pos != std::string::npos; pos = text.find(prefix, pos))
        {
            pos += prefix.size();
            size_t const end = text.find_first_not_of("0123456789", pos);
            std::string const number = text.substr(pos, end - pos);
            if (!number.empty() && number.size() <= 9)
                if (uint32 id = std::stoul(number))
                    ids.insert(id);
        }

        return ids;
    }
}

std::string const ChatHelper::formatMoney(uint32 copper)
{
    std::ostringstream out;
    if (!copper)
    {
        out << "0";
        return out.str();
    }

    uint32 gold = uint32(copper / 10000);
    copper -= (gold * 10000);
    uint32 silver = uint32(copper / 100);
    copper -= (silver * 100);

    bool space = false;
    if (gold > 0)
    {
        out << gold << "g";
        space = true;
    }

    if (silver > 0 && gold < 50)
    {
        if (space)
            out << " ";

        out << silver << "s";
        space = true;
    }

    if (copper > 0 && gold < 10)
    {
        if (space)
            out << " ";

        out << copper << "c";
    }

    return out.str();
}

uint32 ChatHelper::parseMoney(std::string const& text)
{
    uint64 copper = 0;
    std::string number;
    for (char c : text)
    {
        if (c >= '0' && c <= '9')
        {
            if (number.size() >= 9)
                return 0;

            number += c;
            continue;
        }

        uint32 unit = 0;
        switch (c)
        {
            case 'g': unit = 10000; break;
            case 's': unit = 100; break;
            case 'c': unit = 1; break;
            case ' ':
                if (number.empty())
                    continue;
                [[fallthrough]];
            default:
                return 0;
        }

        if (number.empty())
            return 0;

        copper += std::stoull(number) * unit;
        number.clear();
    }

    // A bare number isn't money, and the amount must fit what a trade can carry
    if (!number.empty() || copper > MAX_MONEY_AMOUNT)
        return 0;

    return uint32(copper);
}

std::set<uint32> ChatHelper::parseItems(std::string const& text) { return ParseLinkIds(text, "Hitem:"); }

std::set<uint32> ChatHelper::parseQuests(std::string const& text) { return ParseLinkIds(text, "Hquest:"); }

std::set<uint32> ChatHelper::parseSpells(std::string const& text) { return ParseLinkIds(text, "Hspell:"); }

// Same layout as client links, which ChatLink validates: |c<color>|Hitem:<id> and ten properties|h[name]|h|r
std::string const ChatHelper::FormatItem(ItemTemplate const* proto, uint32 count, uint32 total)
{
    char color[16];
    snprintf(color, sizeof(color), "%08x", ItemQualityColors[proto->Quality < MAX_ITEM_QUALITY ? proto->Quality : 0]);

    std::ostringstream out;
    out << "|c" << color << "|Hitem:" << proto->ItemId << ":0:0:0:0:0:0:0:0:0:0|h[" << proto->Name1 << "]|h|r";

    if (count > 1)
        out << "x" << count;

    if (total > 0)
        out << " (" << total << ")";

    return out.str();
}

std::string const ChatHelper::FormatQuest(Quest const* quest)
{
    if (!quest)
        return "Invalid quest";

    std::ostringstream out;
    out << "|cFFFFFF00|Hquest:" << quest->GetQuestId() << ':' << quest->GetQuestLevel() << "|h["
        << quest->GetLogTitle() << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatSpell(SpellInfo const* spellInfo)
{
    std::ostringstream out;
    out << "|cff71d5ff|Hspell:" << spellInfo->Id << "|h[" << (spellInfo->SpellName ? spellInfo->SpellName : "")
        << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatQuestObjective(std::string const& name, uint32 available, uint32 required)
{
    std::ostringstream out;
    out << "|cFFFFFFFF" << name << (available >= required ? "|cFF00FF00: " : "|cFFFF0000: ") << available << "/"
        << required << "|r";
    return out.str();
}

std::string const ChatHelper::FormatBoolean(bool flag) { return flag ? "|cff00ff00ON|r" : "|cffffff00OFF|r"; }

uint32 ChatHelper::parseItemQuality(std::string const& text)
{
    auto const i = itemQualities.find(text);
    return i != itemQualities.end() ? i->second : MAX_ITEM_QUALITY;
}

std::string const ChatHelper::FormatItemQuality(uint32 quality)
{
    switch (quality)
    {
        case ITEM_QUALITY_POOR: return "grey";
        case ITEM_QUALITY_NORMAL: return "white";
        case ITEM_QUALITY_UNCOMMON: return "green";
        case ITEM_QUALITY_RARE: return "blue";
        case ITEM_QUALITY_EPIC: return "purple";
        case ITEM_QUALITY_LEGENDARY: return "orange";
        case ITEM_QUALITY_ARTIFACT: return "artifact";
        case ITEM_QUALITY_HEIRLOOM: return "heirloom";
        default: return "unknown";
    }
}

bool ChatHelper::parseItemClass(std::string const& text, uint32* itemClass, uint32* itemSubClass)
{
    if (text == "questitem")
    {
        *itemClass = ITEM_CLASS_QUEST;
        *itemSubClass = ITEM_SUBCLASS_QUEST;
        return true;
    }

    if (auto const i = consumableSubClasses.find(text); i != consumableSubClasses.end())
    {
        *itemClass = ITEM_CLASS_CONSUMABLE;
        *itemSubClass = i->second;
        return true;
    }

    if (auto const i = tradeSubClasses.find(text); i != tradeSubClasses.end())
    {
        *itemClass = ITEM_CLASS_TRADE_GOODS;
        *itemSubClass = i->second;
        return true;
    }

    return false;
}

uint32 ChatHelper::parseSlot(std::string const& text)
{
    auto const i = slots.find(text);
    return i != slots.end() ? i->second : EQUIPMENT_SLOT_END;
}

uint32 ChatHelper::parseSkill(std::string const& text)
{
    auto const i = skills.find(text);
    return i != skills.end() ? i->second : SKILL_NONE;
}

std::string const ChatHelper::FormatSkill(uint32 skill)
{
    for (auto const& [name, id] : skills)
        if (id == skill)
            return name;

    return "";
}
