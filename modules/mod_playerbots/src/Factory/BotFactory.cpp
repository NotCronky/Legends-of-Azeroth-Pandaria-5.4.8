#include "BotFactory.h"

#include "StatsWeightCalculator.h"

#include <random>
#include <utility>
 
#include "AccountMgr.h"
#include "AiFactory.h"
#include "ArenaTeam.h"
#include "Bag.h"
#include "DBCStores.h"
#include "DBCStructure.h"
#include "GuildMgr.h"
#include "Helper.h"
#include "Item.h"
#include "ItemVisitors.h"
#include "Log.h"
#include "LogCommon.h"
#include "LootMgr.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "PetDefines.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "PlayerbotSpec.h"
#include "Playerbots.h"
#include "RandomPlayerbotFactory.h"
#include "RandomItemManager.h"
#include "SharedDefines.h"
#include "SpellAuraDefines.h"
#include "SpellMgr.h"
#include "StatsWeightCalculator.h"
#include "World.h"
  
BotFactory::BotFactory(Player* bot, uint32 level, uint32 itemQuality, uint32 gearScoreLimit)
    : level(level), bot(bot)
{
    botAI = GET_PLAYERBOT_AI(bot);
}

void BotFactory::CancelAuras() { bot->RemoveAllAuras(); }
 
void BotFactory::Init()
{
    /*for (std::vector<uint32>::iterator i = sPlayerbotAIConfig->randomBotQuestIds.begin();
        i != sPlayerbotAIConfig->randomBotQuestIds.end(); ++i)
    {
        uint32 questId = *i;
        AddPrevQuests(questId, specialQuestIds);
        specialQuestIds.remove(questId);
        specialQuestIds.push_back(questId);
    }
    uint32 maxStoreSize = sSpellMgr->GetSpellInfoStoreSize();
    for (uint32 id = 1; id < maxStoreSize; ++id)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(id);
        if (!spellInfo)
            continue;
 
        if (id == 47181 || id == 50358 || id == 47242 || id == 52639 || id == 47147 || id == 7218)  // Test Enchant
            continue;
 
        uint32 requiredLevel = spellInfo->BaseLevel;
 
        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].Effect != SPELL_EFFECT_ENCHANT_ITEM)
                continue;
 
            uint32 enchant_id = spellInfo->Effects[j].MiscValue;
            if (!enchant_id)
                continue;
 
            SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
            if (!enchant || (enchant->Flags != PERM_ENCHANTMENT_SLOT && enchant->slot != TEMP_ENCHANTMENT_SLOT))
                continue;
 
            // SpellInfo const* enchantSpell = sSpellMgr->GetSpellInfo(enchant->spellid[0]);
            // if (!enchantSpell)
            //     continue;
            if (strstr(spellInfo->SpellName[0], "Test"))
                break;
 
            enchantSpellIdCache.push_back(id);
            break;
            // TC_LOG_INFO("playerbots", "Add {} to enchantment spells", id);
        }
    }
    TC_LOG_INFO("playerbots", "Loading {} enchantment spells", enchantSpellIdCache.size());
    for (auto iter = sSpellItemEnchantmentStore.begin(); iter != sSpellItemEnchantmentStore.end(); iter++)
    {
        uint32 gemId = iter->GemID;
        if (gemId == 0)
        {
            continue;
        }
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(gemId);
 
        if (proto->ItemLevel < 60)
            continue;
 
        if (proto->Flags & ITEM_FLAG_UNIQUE_EQUIPPABLE)
        {
            continue;
        }
        if (sRandomItemMgr->IsTestItem(gemId))
            continue;
 
        if (!proto || !sGemPropertiesStore.LookupEntry(proto->GemProperties))
        {
            continue;
        }
        // TC_LOG_INFO("playerbots", "Add {} to enchantment gems", gemId);
        enchantGemIdCache.push_back(gemId);
    }
    TC_LOG_INFO("playerbots", "Loading {} enchantment gems", enchantGemIdCache.size());*/
}
 
void BotFactory::Prepare()
{
    if (bot->isDead())
        bot->ResurrectPlayer(1.0f, false);
 
    bot->CombatStop(true);

    int32 newlevel = level;
    if (newlevel < 1)
        newlevel = 1;
    if (newlevel > 90)
        newlevel = 90;

    bot->GiveLevel(newlevel);
    bot->InitTalentForLevel();
    bot->SetUInt32Value(PLAYER_FIELD_XP, 0);
    bot->RemoveAllSpellCooldown();
    bot->InitStatsForLevel();
    CancelAuras();
}
 
void BotFactory::Randomize(bool incremental)
{
    TC_LOG_INFO("playerbots", "{} randomizing {} (level {} class = {})...", (incremental ? "Incremental" : "Full"),
            bot->GetName().c_str(), level, ClassToString((Classes)bot->GetClass()).c_str());

    Prepare();
    if (!incremental)
    {
        // -- Unlearn talents and spec, remove glyphs
        bot->ResetTalents(true, true, true);
        for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
            bot->SetGlyph(slot, 0);

        // -- release pet
        bot->RemovePet(PetRemoveMode::PET_REMOVE_ABANDON, PetRemoveFlag::PET_REMOVE_FLAG_NONE);

        // Destroy equipped items.
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                std::string itemName = item->GetTemplate()->Name1;
                bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            }
        }
    }

    InitTalentsTree(false);
    InitEquipment(incremental);
    InitGlyphs();
    InitPet();
    InitMounts();
    InitPotions();
 
    bot->SetMoney(urand(level * 100000, level * 5 * 100000));
    bot->SetHealth(bot->GetMaxHealth());
    bot->SetPower(POWER_MANA, bot->GetMaxPower(POWER_MANA));

    bot->SaveToDB(false);
}
 
void BotFactory::Refresh()
{
    InitPet();
    InitPotions();
    bot->DurabilityRepairAll(false, 1.0f, false);
    if (bot->isDead())
        bot->ResurrectPlayer(1.0f, false);
    uint32 money = urand(level * 1000, level * 5 * 1000);
    if (bot->GetMoney() < money)
        bot->SetMoney(money);
}
 
void BotFactory::InitPet()
{
    Pet* pet = bot->GetPet();

    /*if (bot->GetClass() == CLASS_HUNTER)
    {
        if (pet)
        {
            bot->RemovePet(PET_REMOVE_ABANDON);
            pet = nullptr;
        }

        // -- Delete all pet slots if possible
        for (uint8 pet_slot_active = 0; pet_slot_active < PetSlot::PET_SLOT_ACTIVE_LAST; ++pet_slot_active)
        {
            uint32 pet_id = bot->GetPetIdBySlot(pet_slot_active);
            if (!pet_id) continue;

            bot->SummonPet(pet_id, bot->GetWorldLocation().GetPositionX(), bot->GetWorldLocation().GetPositionY(),
                           bot->GetWorldLocation().GetPositionZ(), 0.0f, 0);
            bot->RemovePet(PetRemoveMode::PET_REMOVE_ABANDON);
            pet = nullptr;
        }
    }*/

    if (!pet)
    {
        if (bot->GetClass() != CLASS_HUNTER)
            return;
 
        Map* map = bot->GetMap();
        if (!map)
            return;

        std::vector<uint32> ids;
        CreatureTemplateContainer const* creatures = sObjectMgr->GetCreatureTemplates();
        for (CreatureTemplateContainer::const_iterator itr = creatures->begin(); itr != creatures->end(); ++itr)
        {
            if (!itr->second.IsTameable(bot->CanTameExoticPets()))
                continue;
 
            if (itr->second.minlevel > bot->GetLevel())
                continue;
 
            ids.push_back(itr->first);
        }
 
        if (ids.empty())
        {
            TC_LOG_ERROR("playerbots", "No pets available for bot {} ({} level)", bot->GetName().c_str(), bot->GetLevel());
            return;
        }

        for (uint32 i = 0; i < 10; i++)
        {
            uint32 index = urand(0, ids.size() - 1);
            CreatureTemplate const* co = sObjectMgr->GetCreatureTemplate(ids[index]);
            if (!co)
                continue;
            if (co->Name.size() > 21)
                continue;

            int8 newPetSlot = bot->GetSlotForNewPet();
            if (newPetSlot == -1)
                continue;

            // Everything looks OK, create new pet
            pet = bot->CreateTamedPetFrom(co->Entry, 0);
            if (!pet)
                continue;

            // prepare visual effect for levelup
            pet->SetUInt32Value(UNIT_FIELD_LEVEL, bot->GetLevel() - 1);
 
            // add to world
            pet->GetMap()->AddToMap(pet->ToCreature());
 
            // visual effect for levelup
            pet->SetUInt32Value(UNIT_FIELD_LEVEL, bot->GetLevel());
 
            // caster have pet now
            bot->SetMinion(pet, true);
 
            pet->InitTalentForLevel();
 
            pet->SavePetToDB();
            bot->PetSpellInitialize();
            break;
        }
    }
 
    if (pet)
    {
        pet->InitStatsForLevel(bot->GetLevel());
        pet->SetLevel(bot->GetLevel());
        pet->SetHealth(pet->GetMaxHealth());
    }
    else
    {
        TC_LOG_ERROR("playerbots", "Cannot create pet for bot {}", bot->GetName().c_str());
        return;
    }
 
    // TC_LOG_INFO("playerbots", "Start make spell auto cast for {} spells. {} already auto casted.", pet->m_spells.size(),
    // pet->GetPetAutoSpellSize());
    for (PetSpellMap::const_iterator itr = pet->m_spells.begin(); itr != pet->m_spells.end(); ++itr)
    {
        if (itr->second.state == PETSPELL_REMOVED)
            continue;
 
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
        if (!spellInfo)
            continue;
 
        if (spellInfo->IsPassive())
        {
            continue;
        }
        pet->ToggleAutocast(spellInfo, true);
    }
}
#include <fstream>
namespace
{
    enum GlyphType
    {
        GLYPH_TYPE_MAJOR = 0,
        GLYPH_TYPE_MINOR = 1
    };

    // Talent column per tier, by class and specialization tab ("." or a missing tab: the class's first)
    std::string DefaultTalentLink(uint32 cls, uint8 tab)
    {
        static std::map<uint32, std::vector<std::string>> const links = {
            { CLASS_WARRIOR, { "011211", "011211", "011110" } },
            { CLASS_PALADIN, { "201120", "201122", "200122" } },
            { CLASS_HUNTER, { "022210" } },
            { CLASS_ROGUE, { "110002" } },
            { CLASS_PRIEST, { "001222", "000222", "001202" } },
            { CLASS_DEATH_KNIGHT, { "021011" } },
            { CLASS_SHAMAN, { "001221", "001220", "001201" } },
            { CLASS_MAGE, { "112100", "112110", "112120" } },
            { CLASS_WARLOCK, { "110111" } },
            { CLASS_MONK, { "001201" } },
            { CLASS_DRUID, { "002021" } },
        };

        auto itr = links.find(cls);
        if (itr == links.end())
            return "";

        return tab < itr->second.size() ? itr->second[tab] : itr->second.front();
    }

    // Major glyphs, without "Glyph of", by class and specialization tab: the PvE picks of Method's MoP Classic
    // guides, as in playerbots.conf.dist
    std::vector<std::string> DefaultGlyphs(uint32 cls, uint8 tab)
    {
        static std::map<uint32, std::vector<std::vector<std::string>>> const glyphs = {
            { CLASS_WARRIOR, {
                { "Unending Rage", "Colossus Smash", "Bull Rush" },
                { "Unending Rage", "Colossus Smash", "Bull Rush" },
                { "Heavy Repercussions", "Hold the Line", "Unending Rage" },
            } },
            { CLASS_PALADIN, {
                { "Hand of Sacrifice", "Protector of the Innocent", "Divinity" },
                { "Focused Shield", "the Alabaster Shield", "Final Wrath" },
                { "Hand of Sacrifice", "Templar's Verdict", "Inquisition" },
            } },
            { CLASS_HUNTER, {
                { "Deterrence", "Animal Bond", "Disengage" },
                { "Aimed Shot", "Deterrence", "Animal Bond" },
                { "Deterrence", "Animal Bond", "Disengage" },
            } },
            { CLASS_ROGUE, {
                { "Vendetta", "Feint", "Redirect" },
                { "Feint", "Redirect", "Cloak of Shadows" },
                { "Feint", "Redirect", "Cloak of Shadows" },
            } },
            { CLASS_PRIEST, {
                { "Penance", "Fade", "Smite" },
                { "Fade", "Renew", "Circle of Healing" },
                { "Vampiric Embrace", "Fade", "Dispersion" },
            } },
            { CLASS_DEATH_KNIGHT, {
                { "Outbreak", "Anti-Magic Shell", "Regenerative Magic" },
                { "Pestilence", "Anti-Magic Shell", "Loud Horn" },
                { "Loud Horn", "Anti-Magic Shell", "Regenerative Magic" },
            } },
            { CLASS_SHAMAN, {
                { "Lightning Shield", "Flame Shock", "Healing Stream Totem" },
                { "Lightning Shield", "Fire Nova", "Feral Spirit" },
                { "Telluric Currents", "Healing Stream Totem", "Chaining" },
            } },
            { CLASS_MAGE, {
                { "Arcane Power", "Evocation", "Rapid Displacement" },
                { "Rapid Displacement", "Combustion", "Inferno Blast" },
                { "Icy Veins", "Splitting Ice", "Water Elemental" },
            } },
            { CLASS_WARLOCK, {
                { "Unstable Affliction", "Siphon Life", "Healthstone" },
                { "Imp Swarm", "Siphon Life", "Demon Training" },
                { "Siphon Life", "Healthstone", "Eternal Resolve" },
            } },
            { CLASS_MONK, {
                { "Fortifying Brew", "Touch of Death", "Spinning Crane Kick" },
                { "Spinning Crane Kick", "Touch of Death", "Touch of Karma" },
                { "Renewing Mists", "Zen Meditation", "Touch of Death" },
            } },
            { CLASS_DRUID, {
                { "Stampede", "Stampeding Roar", "Rebirth" },
                { "Stampeding Roar", "Savagery", "Cat Form" },
                { "Stampeding Roar", "Rebirth", "Maul" },
                { "Efflorescence", "Wild Growth", "Rejuvenation" },
            } },
        };

        auto itr = glyphs.find(cls);
        if (itr == glyphs.end() || tab >= itr->second.size())
            return {};

        return itr->second[tab];
    }
}

// A class's glyphs: GlyphProperties.dbc has no class, but the class's glyph items teach them (an apply glyph
// effect on the item's spell, with the glyph id). Majors are keyed by the item's name without "Glyph of"
BotFactory::GlyphsForClass const& BotFactory::GetGlyphsForClass(uint32 cls)
{
    // Built once, read from every map thread
    static std::map<uint32, GlyphsForClass> const glyphs = []()
    {
        std::map<uint32, GlyphsForClass> result;
        for (auto const& [itemId, proto] : *sObjectMgr->GetItemTemplateStore())
        {
            if (proto.Class != ITEM_CLASS_GLYPH || proto.Name1.size() <= 9)
                continue;

            for (_Spell const& itemSpell : proto.Spells)
            {
                SpellInfo const* spellInfo = itemSpell.SpellId > 0 ? sSpellMgr->GetSpellInfo(itemSpell.SpellId) : nullptr;
                if (!spellInfo)
                    continue;

                for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                {
                    if (spellInfo->Effects[i].Effect != SPELL_EFFECT_APPLY_GLYPH)
                        continue;

                    GlyphPropertiesEntry const* glyph = sGlyphPropertiesStore.LookupEntry(spellInfo->Effects[i].MiscValue);
                    if (!glyph)
                        continue;

                    GlyphsForClass& classGlyphs = result[proto.SubClass];
                    if (glyph->TypeFlags == GLYPH_TYPE_MAJOR)
                        classGlyphs.majors.emplace(proto.Name1.substr(9), glyph->Id);  // after "Glyph of "
                    else if (glyph->TypeFlags == GLYPH_TYPE_MINOR &&
                             std::find(classGlyphs.minors.begin(), classGlyphs.minors.end(), glyph->Id) == classGlyphs.minors.end())
                        classGlyphs.minors.push_back(glyph->Id);
                }
            }
        }

        return result;
    }();

    static GlyphsForClass const none;
    auto itr = glyphs.find(cls);
    return itr != glyphs.end() ? itr->second : none;
}

void BotFactory::InitTalentsTree(bool reset)
{
    /*std::map<uint32, std::list<const TalentEntry*>> talents_dbc;
    for (auto entry = sTalentStore.begin(); entry != sTalentStore.end(); ++entry)
    {
        if (talents_dbc.find(entry->PlayerClass) == talents_dbc.end())
            talents_dbc[entry->PlayerClass] = std::list<const TalentEntry*>();
        talents_dbc[entry->PlayerClass].push_back(*entry);
    }

    for (auto& ref : talents_dbc)
    {
        ref.second.sort([](const TalentEntry* a, const TalentEntry* b)
        {
            return (a->Row < b->Row) || (a->Row == b->Row && a->Col < b->Col);
        });
    }

    std::ofstream os("./talent_export.txt", std::ios::app);
    for (const auto& ref : talents_dbc)
    {
        auto classe = ClassToString((Classes)ref.first);
        os << classe << ":\n";
        uint32 currentRow = 0;
        for (const auto& tal : ref.second)
        {
            if (tal->Row != currentRow)
            {
                currentRow = tal->Row;
                os << "\n";
            }
            os << tal->TalentID << "\t";
        }
        os << "\n";
    }
    os.close();*/

    // -- reset spec in case we down level
    if (reset)
    {
        bot->ResetTalents(true, true, true);
    }
    
    // if no spec then pick one random (need to change that to balance)
    if (bot->GetSpecialization() == Specializations::SPEC_NONE)
    {
        // -- Select spec
        if (bot->GetLevel() >= 10)
        {
            // Druids have four specializations, every other class three
            dbc::TalentTabs const specs = dbc::GetClassSpecializations(bot->GetClass());
            std::vector<uint32> tabs;
            for (uint32 i = 0; i < specs.size(); ++i)
                if (specs[i])
                    tabs.push_back(i);

            if (tabs.empty())
                return;

            uint32 tab = tabs[urand(0, tabs.size() - 1)];
            WorldPacket p(CMSG_SET_PRIMARY_TALENT_TREE);
            p << tab;
            bot->GetSession()->HandeSetTalentSpecialization(p);
            bot->ActivateSpec(0);
        }
    }

    uint8 const specTab = PlayerBotSpec::GetSpectab(bot);
    if (specTab >= MAX_SPECIALIZATIONS)
        return;

    std::string link = sPlayerbotAIConfig->premadeSpecLink[bot->GetClass()][specTab];
    if (link.empty())
        link = DefaultTalentLink(bot->GetClass(), specTab);

    // One talent per tier, a tier every 15 levels; learned talents are stored by spell, as in Player::LearnTalent
    uint32 const unlockedTiers = bot->CalculateTalentsPoints();
    std::map<uint32, std::vector<TalentEntry const*>> talentsByTier;
    for (TalentEntry const* talent : sTalentStore)
        if (talent->PlayerClass == bot->GetClass() && talent->TierID < unlockedTiers)
            talentsByTier[talent->TierID].push_back(talent);

    std::vector<uint16> toLearn;
    for (auto const& [tier, tierTalents] : talentsByTier)
    {
        if (tierTalents.empty() || std::any_of(tierTalents.begin(), tierTalents.end(),
                                               [&](TalentEntry const* talent) { return bot->HasSpell(talent->SpellID); }))
            continue;

        // The template's column, or a random talent
        TalentEntry const* choice = nullptr;
        if (tier < link.size() && link[tier] >= '0' && link[tier] <= '2')
            for (TalentEntry const* talent : tierTalents)
                if (talent->ColumnIndex == uint32(link[tier] - '0'))
                    choice = talent;

        if (!choice)
            choice = tierTalents[urand(0, tierTalents.size() - 1)];

        toLearn.push_back(choice->ID);
    }

    if (toLearn.empty())
        return;

    WorldPacket p(CMSG_LEARN_TALENT);
    p.WriteBits(toLearn.size(), 23);
    for (uint16 talentId : toLearn)
        p << talentId;
    bot->GetSession()->HandleLearnTalentOpcode(p);
}

void BotFactory::InitGlyphs()
{
    uint32 const enabledSlots = bot->GetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS_ENABLED);
    if (!enabledSlots)
        return;

    GlyphsForClass const& glyphs = GetGlyphsForClass(bot->GetClass());

    uint8 const specTab = PlayerBotSpec::GetSpectab(bot);
    std::vector<std::string> names;
    if (specTab < MAX_SPECIALIZATIONS)
        names = sPlayerbotAIConfig->premadeSpecGlyphs[bot->GetClass()][specTab];
    if (names.empty())
        names = DefaultGlyphs(bot->GetClass(), specTab);

    std::vector<uint32> majors;
    for (std::string const& name : names)
    {
        auto itr = glyphs.majors.find(name);
        if (itr != glyphs.majors.end())
            majors.push_back(itr->second);
    }

    std::set<uint32> used;
    for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
        if (uint32 glyph = bot->GetGlyph(bot->GetActiveSpec(), slot))
            used.insert(glyph);

    for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
    {
        if (!(enabledSlots & (1 << slot)) || bot->GetGlyph(bot->GetActiveSpec(), slot))
            continue;

        GlyphSlotEntry const* glyphSlot = sGlyphSlotStore.LookupEntry(bot->GetGlyphSlot(slot));
        if (!glyphSlot)
            continue;

        // Majors from the template in order; minors (cosmetic in MoP) at random
        uint32 glyph = 0;
        if (glyphSlot->TypeFlags == GLYPH_TYPE_MAJOR)
        {
            for (uint32 candidate : majors)
                if (!used.count(candidate))
                {
                    glyph = candidate;
                    break;
                }
        }
        else if (glyphSlot->TypeFlags == GLYPH_TYPE_MINOR)
        {
            std::vector<uint32> free;
            for (uint32 candidate : glyphs.minors)
                if (!used.count(candidate))
                    free.push_back(candidate);
            if (!free.empty())
                glyph = free[urand(0, free.size() - 1)];
        }

        if (!glyph)
            continue;

        bot->SetGlyph(slot, glyph);
        used.insert(glyph);
    }
}

void BotFactory::ClearEverything()
{
    bot->GiveLevel(bot->GetClass() == CLASS_DEATH_KNIGHT ? sWorld->getIntConfig(CONFIG_START_HEROIC_PLAYER_LEVEL)
                                                        : sWorld->getIntConfig(CONFIG_START_PLAYER_LEVEL));
    bot->SetUInt32Value(PLAYER_FIELD_XP, 0);
    TC_LOG_INFO("playerbots", "Resetting player...");
    bot->ResetTalents(true);
}
  
ObjectGuid BotFactory::GetRandomBot()
{
    GuidVector guids;
    for (std::vector<uint32>::iterator i = sPlayerbotAIConfig->randomBotAccounts.begin();
        i != sPlayerbotAIConfig->randomBotAccounts.end(); i++)
    {
        uint32 accountId = *i;
        if (!AccountMgr::GetCharactersCount(accountId))
            continue;
 
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARS_BY_ACCOUNT_ID);
        stmt->setUInt32(0, accountId);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);
        if (!result)
            continue;
 
        do
        {
            Field* fields = result->Fetch();
            ObjectGuid guid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt32());
            if (!ObjectAccessor::FindPlayer(guid))
                guids.push_back(guid);
        } while (result->NextRow());
    }
 
    if (guids.empty())
        return ObjectGuid::Empty;
 
    uint32 index = urand(0, guids.size() - 1);
    return guids[index];
}
 
std::vector<InventoryType> BotFactory::GetPossibleInventoryTypeListBySlot(EquipmentSlots slot)
{
    std::vector<InventoryType> ret;
    switch (slot)
    {
    case EQUIPMENT_SLOT_HEAD:
        ret.push_back(INVTYPE_HEAD);
        break;
    case EQUIPMENT_SLOT_NECK:
        ret.push_back(INVTYPE_NECK);
        break;
    case EQUIPMENT_SLOT_SHOULDERS:
        ret.push_back(INVTYPE_SHOULDERS);
        break;
    case EQUIPMENT_SLOT_BODY:
        ret.push_back(INVTYPE_BODY);
        break;
    case EQUIPMENT_SLOT_CHEST:
        ret.push_back(INVTYPE_CHEST);
        ret.push_back(INVTYPE_ROBE);
        break;
    case EQUIPMENT_SLOT_WAIST:
        ret.push_back(INVTYPE_WAIST);
        break;
    case EQUIPMENT_SLOT_LEGS:
        ret.push_back(INVTYPE_LEGS);
        break;
    case EQUIPMENT_SLOT_FEET:
        ret.push_back(INVTYPE_FEET);
        break;
    case EQUIPMENT_SLOT_WRISTS:
        ret.push_back(INVTYPE_WRISTS);
        break;
    case EQUIPMENT_SLOT_HANDS:
        ret.push_back(INVTYPE_HANDS);
        break;
    case EQUIPMENT_SLOT_FINGER1:
    case EQUIPMENT_SLOT_FINGER2:
        ret.push_back(INVTYPE_FINGER);
        break;
    case EQUIPMENT_SLOT_TRINKET1:
    case EQUIPMENT_SLOT_TRINKET2:
        ret.push_back(INVTYPE_TRINKET);
        break;
    case EQUIPMENT_SLOT_BACK:
        ret.push_back(INVTYPE_CLOAK);
        break;
    case EQUIPMENT_SLOT_MAINHAND:
        ret.push_back(INVTYPE_WEAPON);
        ret.push_back(INVTYPE_2HWEAPON);
        ret.push_back(INVTYPE_WEAPONMAINHAND);
        ret.push_back(INVTYPE_RANGED);
        break;
    case EQUIPMENT_SLOT_OFFHAND:
        ret.push_back(INVTYPE_WEAPON);
        ret.push_back(INVTYPE_2HWEAPON);
        ret.push_back(INVTYPE_WEAPONOFFHAND);
        ret.push_back(INVTYPE_SHIELD);
        ret.push_back(INVTYPE_HOLDABLE);
        break;
    case EQUIPMENT_SLOT_RANGED:
        ret.push_back(INVTYPE_RANGED);
        break;
    default:
        break;
    }
    return ret;
}

bool BotFactory::CanEquipUnseenItem(uint8 slot, uint16& dest, uint32 item)
{
    dest = 0;

    if (Item* pItem = Item::CreateItem(item, 1, bot, true))
    {
        InventoryResult result = botAI ? botAI->CanEquipItem(slot, dest, pItem, true, true)
            : bot->CanEquipItem(slot, dest, pItem, true, true);
        pItem->RemoveFromUpdateQueueOf(bot);
        delete pItem;
        return result == EQUIP_ERR_OK;
    }

    return false;
}

bool BotFactory::CanEquipItem(ItemTemplate const* proto)
{
    if (proto->Duration != 0)
        return false;

    if (proto->Bonding == BIND_QUEST /*|| proto->Bonding == BIND_WHEN_USE*/)
        return false;

    if (proto->Class == ITEM_CLASS_CONTAINER)
        return true;

    uint32 requiredLevel = proto->RequiredLevel;
    bool hasItem = bot->HasItemCount(proto->ItemId, 1, false);
    if (!requiredLevel && hasItem)
        return false;

    uint32 level = bot->GetLevel();

    if (requiredLevel > level)
        return false;

    return true;
}

namespace
{
    bool IsMountSpell(uint32 spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        return spellInfo && spellInfo->HasAura(SPELL_AURA_MOUNTED);
    }
}

namespace
{
    struct PotionEntry
    {
        uint32 requiredLevel;
        uint32 itemId;
    };

    // Healing or mana potions by required level
    std::vector<PotionEntry> BuildPotions(uint32 effect)
    {
        std::vector<PotionEntry> potions;
        for (auto const& [itemId, proto] : *sObjectMgr->GetItemTemplateStore())
        {
            if (proto.Class != ITEM_CLASS_CONSUMABLE || proto.SubClass != ITEM_SUBCLASS_POTION ||
                proto.Quality > ITEM_QUALITY_UNCOMMON || !proto.SellPrice || proto.Bonding == BIND_QUEST ||
                proto.Spells[0].SpellId <= 0 || proto.Spells[1].SpellId > 0 || proto.Area || proto.Map ||
                (proto.Flags & ITEM_PROTO_FLAG_DEPRECATED))
                continue;

            // Real potions: not fish, brews and placeholder items that happen to heal
            std::string const& name = proto.Name1;
            if ((name.find("Potion") == std::string::npos && name.find("Injector") == std::string::npos) ||
                name.find("DEP") != std::string::npos || name.find("Deprecated") != std::string::npos ||
                name.find("Test") != std::string::npos || name.find("OLD") != std::string::npos)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto.Spells[0].SpellId);
            if (!spellInfo)
                continue;

            // Only the one effect: no rejuvenation potions, no potions with drawbacks
            bool matches = false;
            uint32 effects = 0;
            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (!spellInfo->Effects[i].Effect)
                    continue;

                ++effects;
                if (spellInfo->Effects[i].Effect == effect &&
                    (effect != SPELL_EFFECT_ENERGIZE || spellInfo->Effects[i].MiscValue == POWER_MANA))
                    matches = true;
            }

            if (matches && effects == 1)
                potions.push_back({ proto.RequiredLevel, itemId });
        }

        std::sort(potions.begin(), potions.end(),
                  [](PotionEntry const& a, PotionEntry const& b) { return a.requiredLevel < b.requiredLevel; });
        return potions;
    }

    uint32 FindPotion(uint32 level, uint32 effect)
    {
        // Built once, read from every map thread
        static std::vector<PotionEntry> const healing = BuildPotions(SPELL_EFFECT_HEAL);
        static std::vector<PotionEntry> const mana = BuildPotions(SPELL_EFFECT_ENERGIZE);

        // Among the highest level ones the bot can use, a random one
        std::vector<uint32> best;
        uint32 bestLevel = 0;
        for (PotionEntry const& potion : effect == SPELL_EFFECT_HEAL ? healing : mana)
        {
            if (potion.requiredLevel > level)
                break;

            if (potion.requiredLevel != bestLevel)
            {
                best.clear();
                bestLevel = potion.requiredLevel;
            }
            best.push_back(potion.itemId);
        }

        return best.empty() ? 0 : best[urand(0, best.size() - 1)];
    }
}

void BotFactory::InitPotions()
{
    for (uint32 effect : { uint32(SPELL_EFFECT_HEAL), uint32(SPELL_EFFECT_ENERGIZE) })
    {
        if (effect == SPELL_EFFECT_ENERGIZE && !bot->GetMaxPower(POWER_MANA))
            continue;

        // Keep what it has until it runs out
        FindPotionVisitor visitor(bot, effect);
        for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
            if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                visitor.Visit(item);
        for (uint8 bagSlot = INVENTORY_SLOT_BAG_START; bagSlot < INVENTORY_SLOT_BAG_END; ++bagSlot)
            if (Bag* bag = bot->GetBagByPos(bagSlot))
                for (uint32 slot = 0; slot < bag->GetBagSize(); ++slot)
                    if (Item* item = bag->GetItemByPos(slot))
                        visitor.Visit(item);

        if (!visitor.GetResult().empty())
            continue;

        uint32 const itemId = FindPotion(level, effect);
        ItemTemplate const* proto = itemId ? sObjectMgr->GetItemTemplate(itemId) : nullptr;
        if (!proto)
            continue;

        uint32 const maxCount = proto->GetMaxStackSize();
        bot->StoreNewItemInBestSlots(itemId, urand(std::max(1u, maxCount / 2), maxCount));
    }
}

void BotFactory::InitMounts()
{
    // Riding: apprentice 20, journeyman 40, expert 60, artisan 70, master 80, and the flying licenses
    struct RidingSpell
    {
        uint32 level;
        uint32 spellId;
    };
    static RidingSpell const riding[] = {
        { 20, 33388 },   // Apprentice Riding
        { 40, 33391 },   // Journeyman Riding
        { 60, 34090 },   // Expert Riding
        { 60, 90267 },   // Flight Master's License
        { 68, 54197 },   // Cold Weather Flying
        { 70, 34091 },   // Artisan Riding
        { 80, 90265 },   // Master Riding
        { 90, 115913 },  // Wisdom of the Four Winds
    };

    for (RidingSpell const& spell : riding)
        if (level >= spell.level && !bot->HasSpell(spell.spellId) && sSpellMgr->GetSpellInfo(spell.spellId))
            bot->LearnSpell(spell.spellId, false);

    if (level < 20)
        return;

    bool const horde = bot->GetTeam() == HORDE;
    std::vector<uint32> slow, fast;
    switch (bot->GetRace())
    {
        case RACE_HUMAN: slow = { 470, 6648, 458, 472 }; fast = { 23228, 23227, 23229 }; break;
        case RACE_ORC: slow = { 6654, 6653, 580 }; fast = { 23250, 23252, 23251 }; break;
        case RACE_DWARF: slow = { 6899, 6777, 6898 }; fast = { 23238, 23239, 23240 }; break;
        case RACE_NIGHTELF: slow = { 10789, 8394, 10793 }; fast = { 23219, 23220, 63637 }; break;
        case RACE_UNDEAD_PLAYER: slow = { 17463, 17464, 17462 }; fast = { 17465, 23246, 66846 }; break;
        case RACE_TAUREN: slow = { 18990, 18989, 64657 }; fast = { 23249, 23248, 23247 }; break;
        case RACE_GNOME: slow = { 10969, 17453, 10873, 17454 }; fast = { 23225, 23223, 23222 }; break;
        case RACE_TROLL: slow = { 10796, 10799, 8395 }; fast = { 23241, 23242, 23243 }; break;
        case RACE_DRAENEI: slow = { 34406, 35711, 35710 }; fast = { 35713, 35712, 35714 }; break;
        case RACE_BLOODELF: slow = { 33660, 35020, 35022, 35018 }; fast = { 35025, 35027 }; break;
        case RACE_GOBLIN: slow = { 87090 }; fast = { 87091 }; break;  // Goblin Trike, Goblin Turbo-Trike
        case RACE_WORGEN: slow = { 103195 }; fast = { 103196 }; break;  // Mountain Horse, Swift Mountain Horse
        default:
            // Pandaren: the faction's horses or wolves
            if (horde)
            {
                slow = { 6654, 6653, 580 };
                fast = { 23250, 23252, 23251 };
            }
            else
            {
                slow = { 470, 6648, 458, 472 };
                fast = { 23228, 23227, 23229 };
            }
            break;
    }

    std::vector<uint32> const flying = horde ? std::vector<uint32>{ 32244, 32245, 32243 } : std::vector<uint32>{ 32235, 32239, 32240 };
    std::vector<uint32> const fastFlying =
        horde ? std::vector<uint32>{ 32295, 32297, 32246, 32296 } : std::vector<uint32>{ 32242, 32289, 32290, 32292 };

    // One of each kind the bot can ride, unless it knows one already
    auto learnOne = [&](std::vector<uint32> const& spells)
    {
        std::vector<uint32> valid;
        for (uint32 spellId : spells)
        {
            if (bot->HasSpell(spellId))
                return;

            if (IsMountSpell(spellId))
                valid.push_back(spellId);
        }

        if (!valid.empty())
            bot->LearnSpell(valid[urand(0, valid.size() - 1)], false);
    };

    learnOne(level >= 40 ? fast : slow);
    if (level >= 70)
        learnOne(fastFlying);
    else if (level >= 60)
        learnOne(flying);
}

void BotFactory::InitEquipment(bool incremental, bool second_chance)
{
    // Candidates for a slot are random items of the bot's level; keep the one its spec values most
    StatsWeightCalculator calculator(bot);
    std::unordered_map<uint8, std::vector<uint32>> items;
    uint32 blevel = bot->GetLevel();
    int32 delta = std::min(blevel, 10u);

    for (int32 slot = (int32)EQUIPMENT_SLOT_TABARD; slot >= (int32)EQUIPMENT_SLOT_START; slot--)
    {
        if (slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_BODY || slot == EQUIPMENT_SLOT_RANGED)
            continue;

        if (level < 50 && (slot == EQUIPMENT_SLOT_TRINKET1 || slot == EQUIPMENT_SLOT_TRINKET2))
            continue;

        if (level < 30 && slot == EQUIPMENT_SLOT_NECK)
            continue;

        if (level < 25 && slot == EQUIPMENT_SLOT_HEAD)
            continue;

        if (level < 20 && (slot == EQUIPMENT_SLOT_FINGER1 || slot == EQUIPMENT_SLOT_FINGER2))
            continue;

        Item* oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (oldItem && second_chance)
        {
            bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
        }

        oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        bool isforcedbreak = false;
        int maxiRetry = 10;
        do
        {
            for (InventoryType inventoryType : GetPossibleInventoryTypeListBySlot((EquipmentSlots)slot))
            {
                uint32 itemid = sRandomItemMgr->FindBestItemForLevelAndEquip(bot, inventoryType);
                if (itemid)
                {
                    uint32 skipProb = 25;
                    if (urand(1, 100) <= skipProb)
                        continue;

                    items[slot].push_back(itemid);
                }
                else
                {
                    maxiRetry--;
                    if (maxiRetry <= 0)
                    {
                        isforcedbreak = true;
                        break;
                    }
                }
            }
        } while (items[slot].size() < 25 && !isforcedbreak);

        std::vector<uint32>& ids = items[slot];
        if (ids.empty())
        {
            continue;
        }

        uint32 bestItemForSlot = 0;
        float bestScoreForSlot = -1.0f;
        for (uint32 itemId : ids)
        {
            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);

            // delay heavy check to here
            if (!CanEquipItem(proto))
                continue;
            uint16 dest;
            if (!CanEquipUnseenItem(slot, dest, proto->ItemId))
                continue;

            float const score = calculator.CalculateItem(proto);
            if (score > bestScoreForSlot)
            {
                bestScoreForSlot = score;
                bestItemForSlot = proto->ItemId;
            }
        }

        if (bestItemForSlot == 0)
        {
            continue;
        }
        uint16 dest;
        if (!CanEquipUnseenItem(slot, dest, bestItemForSlot))
        {
            continue;
        }

        oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        // fail to store in bag
        if (oldItem)
        {
            bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            //continue;
        }
        Item* newItem = bot->EquipNewItem(dest, bestItemForSlot, true);
        bot->AutoUnequipOffhandIfNeed();
    }

    // Secondary init for better equips
    /// @todo: clean up duplicate code
    if (second_chance)
    {
        for (int32 slot = (int32)EQUIPMENT_SLOT_TABARD; slot >= (int32)EQUIPMENT_SLOT_START; slot--)
        {
            if (slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_BODY)
                continue;

            if (level < 50 && (slot == EQUIPMENT_SLOT_TRINKET1 || slot == EQUIPMENT_SLOT_TRINKET2))
                continue;

            if (level < 30 && slot == EQUIPMENT_SLOT_NECK)
                continue;

            if (level < 25 && slot == EQUIPMENT_SLOT_HEAD)
                continue;

            if (level < 20 && (slot == EQUIPMENT_SLOT_FINGER1 || slot == EQUIPMENT_SLOT_FINGER2))
                continue;

            if (Item* oldItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

            std::vector<uint32>& ids = items[slot];
            if (ids.empty())
                continue;

            float bestScoreForSlot = -1.0f;
            uint32 bestItemForSlot = 0;
            for (uint32 itemId : ids)
            {
                ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
                // delay heavy check to here
                if (!CanEquipItem(proto))
                    continue;
                uint16 dest;
                if (!CanEquipUnseenItem(slot, dest, proto->ItemId))
                    continue;

                float const score = calculator.CalculateItem(proto);
                if (score > bestScoreForSlot)
                {
                    bestScoreForSlot = score;
                    bestItemForSlot = proto->ItemId;
                }
            }

            if (bestItemForSlot == 0)
            {
                continue;
            }
            uint16 dest;
            if (!CanEquipUnseenItem(slot, dest, bestItemForSlot))
            {
                continue;
            }
            Item* newItem = bot->EquipNewItem(dest, bestItemForSlot, true);
            bot->AutoUnequipOffhandIfNeed();
        }
    }
}