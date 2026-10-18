/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ShamanTriggers.h"

#include "Playerbots.h"

bool ShamanShieldTrigger::IsActive()
{
    bool const isRestoration = bot->GetSpecialization() == Specializations::SPEC_SHAMAN_RESTORATION;
    return isRestoration == restoration && BuffTrigger::IsActive();
}

bool ShamanWeaponImbueTrigger::IsActive()
{
    if (bot->GetSpecialization() != spec || !bot->HasSpell(spellId))
        return false;

    Item* weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
    return weapon && !weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
}

bool NoFireTotemTrigger::IsActive()
{
    return bot->HasSpell(3599) /*Searing Totem*/ && !AI_VALUE2(bool, "has totem", "searing totem") &&
           !AI_VALUE2(bool, "has totem", "magma totem") && !AI_VALUE2(bool, "has totem", "fire elemental totem");
}
