/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "SpellNameAudit.h"

#include <algorithm>
#include <mutex>
#include <set>
#include <sstream>

#include "AiObjectContext.h"
#include "DBCStores.h"
#include "GenericSpellActions.h"
#include "Helper.h"
#include "Playerbots.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

namespace
{
    std::string ToLower(std::string text)
    {
        std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
        return text;
    }

    void AddSpellName(std::set<std::string>& names, uint32 spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (spellInfo && spellInfo->SpellName && !spellInfo->IsPassive())
            names.insert(ToLower(spellInfo->SpellName));
    }

    bool IsClassSkill(uint32 skillId, uint8 cls)
    {
        SkillLineEntry const* skillLine = sSkillLineStore.LookupEntry(skillId);
        if (!skillLine || skillLine->CategoryID != SKILL_CATEGORY_CLASS)
            return false;

        for (uint8 race = 1; race < MAX_RACES; ++race)
            if (sDBCManager.GetSkillRaceClassInfo(skillId, race, cls))
                return true;

        return false;
    }

    // Names of the active spells a class can learn: class skill line abilities, talents and specialization spells
    std::set<std::string> GetLearnableSpellNames(uint8 cls)
    {
        std::set<std::string> names;
        uint32 const classMask = 1 << (cls - 1);

        for (SkillLineAbilityEntry const* ability : sSkillLineAbilityStore)
        {
            if (ability->ClassMask & classMask)
                AddSpellName(names, ability->Spell);
            else if (!ability->ClassMask && IsClassSkill(ability->SkillLine, cls))
                AddSpellName(names, ability->Spell);
        }

        for (TalentEntry const* talent : sTalentStore)
            if (talent->PlayerClass == cls)
                AddSpellName(names, talent->SpellID);

        for (uint32 specId : dbc::GetClassSpecializations(cls))
        {
            if (!specId)
                continue;

            if (std::vector<SpecializationSpellsEntry const*> const* specSpells = sDBCManager.GetSpecializationSpells(specId))
                for (SpecializationSpellsEntry const* specSpell : *specSpells)
                    AddSpellName(names, specSpell->SpellID);
        }

        return names;
    }
}

void SpellNameAudit::CheckClass(uint8 cls, AiObjectContext* context)
{
    static std::mutex lock;
    static std::set<uint8> checked;
    {
        std::lock_guard<std::mutex> guard(lock);
        if (!checked.insert(cls).second)
            return;
    }

    std::set<std::string> const learnable = GetLearnableSpellNames(cls);
    std::set<std::string> missing;
    uint32 castActions = 0;
    for (std::string const& actionName : context->GetSupportedActions())
    {
        CastSpellAction* action = dynamic_cast<CastSpellAction*>(context->GetAction(actionName));
        if (!action || action->GetSpellName().empty())
            continue;

        ++castActions;
        std::string const spellName = ToLower(action->GetSpellName());
        if (!learnable.count(spellName))
            missing.insert(spellName);
    }

    if (missing.empty())
    {
        TC_LOG_INFO("playerbots", "Spell check ({}): all {} cast actions use spells this class can learn",
            ClassToString((Classes)cls), castActions);
        return;
    }

    std::ostringstream out;
    for (std::string const& name : missing)
        out << (&name == &*missing.begin() ? "" : ", ") << name;

    TC_LOG_WARN("playerbots", "Spell check ({}): {} of {} cast actions use spells this class can't learn in this client: {}",
        ClassToString((Classes)cls), missing.size(), castActions, out.str());
}
