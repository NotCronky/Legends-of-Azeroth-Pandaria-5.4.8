/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ListSpellsAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "ItemVisitors.h"
#include "Playerbots.h"

namespace
{
    bool IsSkillSpell(uint32 spellId, uint32 skill)
    {
        SkillLineAbilityMapBounds const bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
        for (auto i = bounds.first; i != bounds.second; ++i)
            if (i->second->SkillLine == skill)
                return true;

        return false;
    }
}

bool ListSpellsAction::Execute(Event event)
{
    std::string filter = event.getParam();

    // A leading profession name ("first aid" is two words)
    uint32 skill = SKILL_NONE;
    for (size_t end = filter.find(' '); skill == SKILL_NONE; end = filter.find(' ', end + 1))
    {
        skill = ChatHelper::parseSkill(filter.substr(0, end));
        if (skill != SKILL_NONE)
            filter = end == std::string::npos ? "" : filter.substr(end + 1);

        if (end == std::string::npos)
            break;
    }

    static std::set<std::string> const ignored = { "Opening", "Closing", "Stuck", "Remove Insignia",
        "Opening - No Text", "Grovel", "Duel", "Honorless Target" };

    std::map<std::string, std::string> lines;
    for (auto const& [spellId, spell] : bot->GetSpellMap())
    {
        if (spell->state == PLAYERSPELL_REMOVED || !spell->active || spell->disabled)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo || spellInfo->IsPassive() || !spellInfo->SpellName || ignored.count(spellInfo->SpellName))
            continue;

        if (skill != SKILL_NONE && !IsSkillSpell(spellId, skill))
            continue;

        std::string const name = spellInfo->SpellName;
        if ((!filter.empty() && !strstri(name.c_str(), filter.c_str())) || lines.count(name))
            continue;

        std::ostringstream out;
        ItemTemplate const* created = nullptr;
        if (skill != SKILL_NONE)
            for (SpellEffectInfo const& effect : spellInfo->Effects)
                if (effect.Effect == SPELL_EFFECT_CREATE_ITEM && effect.ItemType)
                    created = sObjectMgr->GetItemTemplate(effect.ItemType);

        out << (created ? ChatHelper::FormatItem(created) : ChatHelper::FormatSpell(spellInfo));

        bool first = true;
        for (uint32 i = 0; i < MAX_SPELL_REAGENTS; ++i)
        {
            if (spellInfo->Reagent[i] <= 0 || !spellInfo->ReagentCount[i])
                continue;

            ItemTemplate const* reagent = sObjectMgr->GetItemTemplate(spellInfo->Reagent[i]);
            if (!reagent)
                continue;

            out << (first ? ": " : ", ") << ChatHelper::FormatItem(reagent, spellInfo->ReagentCount[i]);
            if (uint32 have = bot->GetItemCount(reagent->ItemId))
                out << " |cffffff00(" << have << ")|r";

            first = false;
        }

        lines[name] = out.str();
    }

    if (lines.empty())
    {
        botAI->TellMaster("No spells found");
        return true;
    }

    botAI->TellMaster("=== Spells ===");
    for (auto const& [name, line] : lines)
        botAI->TellMasterNoFacing(line);

    return true;
}
