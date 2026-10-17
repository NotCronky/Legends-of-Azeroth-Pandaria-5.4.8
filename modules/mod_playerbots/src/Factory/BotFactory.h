#ifndef _PLAYERBOT_BOTFACTORY_H
#define _PLAYERBOT_BOTFACTORY_H

#include "Player.h"
#include "PlayerbotAI.h"

class BotFactory
{
public:
    BotFactory(Player* bot, uint32 level, uint32 itemQuality = 0, uint32 gearScoreLimit = 0);

    static ObjectGuid GetRandomBot();
    static void Init();
    void Refresh();
    void Randomize(bool incremental);
    void ClearEverything();

    void InitEquipment(bool incremental, bool second_chance = false);
    void InitPet();
    // The template's talent in each unlocked tier (AiPlayerbot.PremadeSpecLink, or the built-in one)
    void InitTalentsTree(bool reset);
    // Major glyphs from the template and random minor ones in the unlocked, empty glyph slots
    void InitGlyphs();
    // Riding skills and flying licenses for the bot's level, and a ground and a flying mount
    void InitMounts();
    // A stack of the best healing potions, and mana potions for mana users, if the bot has none
    void InitPotions();
private:
    // A class's major glyphs by name (without "Glyph of") and minor glyphs, as GlyphProperties ids
    struct GlyphsForClass
    {
        std::map<std::string, uint32> majors;
        std::vector<uint32> minors;
    };
    static GlyphsForClass const& GetGlyphsForClass(uint32 cls);

    void Prepare();
    void CancelAuras();

    // -- GEARING
    bool CanEquipItem(ItemTemplate const* proto);
    bool CanEquipUnseenItem(uint8 slot, uint16& dest, uint32 item);
    std::vector<InventoryType> GetPossibleInventoryTypeListBySlot(EquipmentSlots slot);

    uint32 level;
protected:
    Player* bot;
    PlayerbotAI* botAI;
};

#endif