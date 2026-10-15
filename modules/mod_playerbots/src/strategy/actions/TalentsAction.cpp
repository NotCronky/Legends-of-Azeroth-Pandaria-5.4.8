/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "TalentsAction.h"

#include <algorithm>

#include "BotFactory.h"
#include "ChatHelper.h"
#include "DBCStores.h"
#include "Event.h"
#include "Playerbots.h"

namespace
{
    // Six talent tiers, one every 15 levels
    uint32 const TALENT_TIERS = 6;
    uint32 const TALENT_TIER_LEVELS = 15;

    std::string ToLower(std::string text)
    {
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        return text;
    }

    TalentEntry const* LearnedTalentInTier(Player* bot, uint32 tier)
    {
        for (TalentEntry const* talent : sTalentStore)
            if (talent->PlayerClass == bot->GetClass() && talent->TierID == tier && bot->HasSpell(talent->SpellID))
                return talent;

        return nullptr;
    }
}

bool TalentsAction::Execute(Event event)
{
    std::string const text = event.getParam();
    if (text.empty())
    {
        ListTalents();
        return true;
    }

    if (event.getOwner() && !botAI->CanManageFor(event.getOwner()))
    {
        bot->Whisper("Only my master can do that.", LANG_UNIVERSAL, event.getOwner());
        return false;
    }

    if (text.rfind("spec ", 0) == 0)
        return SwitchSpec(ToLower(text.substr(5)));

    return ChooseTalent(text);
}

void TalentsAction::ListTalents()
{
    std::ostringstream spec;
    ChrSpecializationEntry const* entry = sChrSpecializationStore.LookupEntry(bot->GetSpecialization());
    spec << "Specialization: " << (entry && entry->Name ? entry->Name : "none");
    botAI->TellMaster(spec);

    uint32 const unlockedTiers = bot->CalculateTalentsPoints();
    for (uint32 tier = 0; tier < unlockedTiers && tier < TALENT_TIERS; ++tier)
    {
        std::ostringstream out;
        out << "Level " << (tier + 1) * TALENT_TIER_LEVELS << ": ";
        TalentEntry const* talent = LearnedTalentInTier(bot, tier);
        SpellInfo const* spellInfo = talent ? sSpellMgr->GetSpellInfo(talent->SpellID) : nullptr;
        out << (spellInfo ? ChatHelper::FormatSpell(spellInfo) : "-");
        botAI->TellMaster(out);
    }
}

bool TalentsAction::SwitchSpec(std::string const& name)
{
    dbc::TalentTabs const specs = dbc::GetClassSpecializations(bot->GetClass());
    for (uint32 tab = 0; tab < specs.size(); ++tab)
    {
        ChrSpecializationEntry const* entry = specs[tab] ? sChrSpecializationStore.LookupEntry(specs[tab]) : nullptr;
        if (!entry || !entry->Name || ToLower(entry->Name).find(name) != 0)
            continue;

        if (bot->GetLevel() < 10)
        {
            botAI->TellMaster("I need to be level 10 to choose a specialization");
            return false;
        }

        if (bot->GetSpecialization() == Specializations(specs[tab]))
        {
            botAI->TellMaster(std::string("I'm already ") + entry->Name);
            return false;
        }

        // As the .playerbots setspec command does, keeping the gear
        bot->ResetTalents(true, true, true);
        WorldPacket packet(CMSG_SET_PRIMARY_TALENT_TREE, 4);
        packet << uint32(tab);
        bot->GetSession()->HandeSetTalentSpecialization(packet);
        bot->ActivateSpec(0);
        BotFactory factory(bot, bot->GetLevel());
        factory.InitTalentsTree(false);
        botAI->ResetStrategies();

        botAI->TellMaster(std::string("Switched to ") + entry->Name);
        ListTalents();
        return true;
    }

    std::ostringstream out;
    out << "Specializations:";
    for (uint32 spec : specs)
        if (ChrSpecializationEntry const* entry = spec ? sChrSpecializationStore.LookupEntry(spec) : nullptr)
            out << " " << (entry->Name ? entry->Name : "?");
    botAI->TellMaster(out);
    return false;
}

TalentEntry const* TalentsAction::FindTalent(std::string const& text)
{
    std::set<uint32> const linkedSpells = ChatHelper::parseSpells(text);
    std::string const name = ToLower(text);

    TalentEntry const* partial = nullptr;
    for (TalentEntry const* talent : sTalentStore)
    {
        if (talent->PlayerClass != bot->GetClass())
            continue;

        if (linkedSpells.count(talent->SpellID))
            return talent;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(talent->SpellID);
        if (!spellInfo || !spellInfo->SpellName || !linkedSpells.empty())
            continue;

        std::string const spellName = ToLower(spellInfo->SpellName);
        if (spellName == name)
            return talent;

        if (!partial && spellName.find(name) != std::string::npos)
            partial = talent;
    }

    return partial;
}

bool TalentsAction::ChooseTalent(std::string const& text)
{
    TalentEntry const* talent = FindTalent(text);
    SpellInfo const* spellInfo = talent ? sSpellMgr->GetSpellInfo(talent->SpellID) : nullptr;
    if (!spellInfo)
    {
        botAI->TellMaster("I have no talent " + text + ". Usage: talents [spec <name> | <talent>]");
        return false;
    }

    std::string const link = ChatHelper::FormatSpell(spellInfo);
    if (talent->TierID >= bot->CalculateTalentsPoints())
    {
        std::ostringstream out;
        out << link << " needs level " << (talent->TierID + 1) * TALENT_TIER_LEVELS;
        botAI->TellMaster(out);
        return false;
    }

    TalentEntry const* current = LearnedTalentInTier(bot, talent->TierID);
    if (current == talent)
    {
        botAI->TellMaster("I already have " + link);
        return false;
    }

    // Swapping a talent is free for bots. RemoveTalent doesn't give the talent point back
    if (current && bot->RemoveTalent(current->ID) && bot->GetUsedTalentCount())
        bot->SetUsedTalentCount(bot->GetUsedTalentCount() - 1);

    WorldPacket packet(CMSG_LEARN_TALENT, 3 + 2);
    packet.WriteBits(1, 23);
    packet.FlushBits();
    packet << uint16(talent->ID);
    bot->GetSession()->HandleLearnTalentOpcode(packet);

    if (!bot->HasSpell(talent->SpellID))
    {
        botAI->TellMaster("I couldn't learn " + link);
        return false;
    }

    botAI->TellMaster("Learned " + link);
    return true;
}
