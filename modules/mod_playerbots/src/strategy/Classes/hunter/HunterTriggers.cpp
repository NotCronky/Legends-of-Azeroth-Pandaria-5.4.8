/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "HunterTriggers.h"

#include "Playerbots.h"
#include "Pet.h"

bool HunterFocusTrigger::IsActive() { return bot->GetPower(POWER_FOCUS) >= int32(amount); }

bool HunterNoAspectTrigger::IsActive()
{
    return !botAI->HasAnyAuraOf(bot, "aspect of the hawk", "aspect of the iron hawk", nullptr) &&
           (bot->HasSpell(13165) /*Aspect of the Hawk*/ || bot->HasSpell(109260) /*Aspect of the Iron Hawk*/);
}

bool HuntersPetDeadTrigger::IsActive()
{
    Pet* pet = bot->GetPet();
    return pet && !pet->IsAlive();
}

bool HuntersPetLowHealthTrigger::IsActive()
{
    Pet* pet = bot->GetPet();
    return pet && pet->IsAlive() && pet->GetHealthPct() < 50.0f && !botAI->HasAura("mend pet", pet);
}

bool HunterNoPetTrigger::IsActive()
{
    // A dead pet is revived instead; stabled or dismissed ones are called
    return !bot->GetPet() && !bot->GetGuardianPet() && !bot->IsMounted() && bot->HasSpell(883) /*Call Pet 1*/ &&
           bot->GetPetIdBySlot(PET_SLOT_ACTIVE_FIRST);
}

bool AutoShotDeadTargetTrigger::IsActive()
{
    Spell* autoRepeat = bot->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL);
    if (!autoRepeat)
        return false;

    Unit* target = autoRepeat->m_targets.GetUnitTarget();
    return !target || !target->IsAlive() || !bot->IsValidAttackTarget(target);
}

Value<Unit*>* FrenzyFiveStacksTrigger::GetTargetValue() { return context->GetValue<Unit*>("pet target"); }

bool FrenzyFiveStacksTrigger::IsActive()
{
    // Not during Bestial Wrath, which Focus Fire would waste
    return HasAuraStackTrigger::IsActive() && !botAI->HasAura("bestial wrath", bot);
}
