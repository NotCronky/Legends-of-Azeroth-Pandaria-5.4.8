/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CHATHELPER_H
#define _PLAYERBOT_CHATHELPER_H

#include <set>
#include <string>

#include "Define.h"

class Quest;
class SpellInfo;

struct ItemTemplate;

// Parsing of chat command text (links, money, item qualities and classes, slots, skills) and formatting of
// bot replies. Stateless, so bots on different map threads can share it.
class ChatHelper
{
public:
    static std::string const formatMoney(uint32 copper);
    // "1g 20s 5c" style amounts; 0 if the text is not money
    static uint32 parseMoney(std::string const& text);

    // Ids from |Hitem:, |Hquest: and |Hspell: links
    static std::set<uint32> parseItems(std::string const& text);
    static std::set<uint32> parseQuests(std::string const& text);
    static std::set<uint32> parseSpells(std::string const& text);

    static std::string const FormatItem(ItemTemplate const* proto, uint32 count = 0, uint32 total = 0);
    static std::string const FormatQuest(Quest const* quest);
    static std::string const FormatSpell(SpellInfo const* spellInfo);
    static std::string const FormatQuestObjective(std::string const& name, uint32 available, uint32 required);
    static std::string const FormatBoolean(bool flag);

    // MAX_ITEM_QUALITY if the text names no quality (grey, green, epic, ...)
    static uint32 parseItemQuality(std::string const& text);
    static std::string const FormatItemQuality(uint32 quality);
    // Consumable and trade goods classes: potion, food, cloth, herb, ...; also "questitem"
    static bool parseItemClass(std::string const& text, uint32* itemClass, uint32* itemSubClass);
    // EQUIPMENT_SLOT_END if the text names no slot (head, main hand, trinket 1, ...)
    static uint32 parseSlot(std::string const& text);
    // SKILL_NONE if the text names no profession
    static uint32 parseSkill(std::string const& text);
    static std::string const FormatSkill(uint32 skill);
};

#endif
