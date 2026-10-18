/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DKTriggers.h"

#include "Playerbots.h"

bool HighRunicPowerTrigger::IsActive()
{
    uint32 const max = bot->GetMaxPower(POWER_RUNIC_POWER);
    return max && bot->GetPower(POWER_RUNIC_POWER) * 100 >= max * 80;
}

bool NoRunesTrigger::IsActive()
{
    if (bot->GetClass() != CLASS_DEATH_KNIGHT)
        return false;

    for (uint8 i = 0; i < MAX_RUNES; ++i)
        if (bot->GetRune(i).State == RuneState::Ready)
            return false;

    return true;
}

Value<Unit*>* ShadowInfusionTrigger::GetTargetValue() { return context->GetValue<Unit*>("pet target"); }

bool SoulReaperTrigger::IsActive()
{
    Unit* target = GetTarget();
    return target && target->GetHealthPct() < 35.0f && SpellCanBeCastTrigger::IsActive();
}

bool DKNoGhoulTrigger::IsActive()
{
    return bot->GetSpecialization() == Specializations::SPEC_DEATH_KNIGHT_UNHOLY && !bot->GetPet() &&
           !bot->GetGuardianPet() && !bot->IsMounted() && bot->HasSpell(46584) /*Raise Dead*/;
}
