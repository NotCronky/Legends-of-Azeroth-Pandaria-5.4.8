/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MageTriggers.h"

#include "Playerbots.h"

bool MageArmorTrigger::IsActive() { return bot->GetSpecialization() == spec && BuffTrigger::IsActive(); }

bool ArcaneChargesTrigger::IsActive()
{
    return HasAuraStackTrigger::IsActive() && AI_VALUE2(uint8, "mana", "self target") < sPlayerbotAIConfig->mediumMana;
}

bool WaterElementalTrigger::IsActive()
{
    return bot->GetSpecialization() == Specializations::SPEC_MAGE_FROST && !bot->GetPet() && !bot->GetGuardianPet() &&
           !bot->IsMounted() && bot->HasSpell(31687) /*Summon Water Elemental*/;
}
