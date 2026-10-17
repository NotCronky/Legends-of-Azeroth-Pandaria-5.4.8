/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "HunterActions.h"

#include "Event.h"
#include "Playerbots.h"

bool CastAutoShotAction::isUseful()
{
    if (botAI->IsInVehicle() && !botAI->IsInVehicle(false, false, true))
        return false;

    // Already shooting at the current target
    Unit* target = AI_VALUE(Unit*, "current target");
    if (!target)
        return false;

    Spell* autoRepeat = bot->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL);
    if (autoRepeat && autoRepeat->m_targets.GetUnitTargetGUID() == target->GetGUID())
        return false;

    return AI_VALUE(uint32, "active spell") != AI_VALUE2(uint32, "spell id", getName());
}

bool StopAutoShotAction::Execute(Event /*event*/)
{
    bot->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
    return true;
}

bool CastCallPetAction::isUseful()
{
    // Calling takes a moment; don't recast it while the pet appears
    return getMSTimeDiff(lastCall, getMSTime()) > 5 * IN_MILLISECONDS && bot->GetPetIdBySlot(PET_SLOT_ACTIVE_FIRST);
}

bool CastCallPetAction::Execute(Event event)
{
    bool const calledRecently = lastCall && getMSTimeDiff(lastCall, getMSTime()) < 15 * IN_MILLISECONDS;
    lastCall = getMSTime();
    if (calledRecently && botAI->CastSpell("revive pet", bot))
        return true;

    return CastBuffSpellAction::Execute(event);
}

bool CastBestialWrathAction::isUseful()
{
    Pet* pet = bot->GetPet();
    return pet && pet->IsAlive() && bot->IsInCombat() && CastBuffSpellAction::isUseful();
}

bool CastKillCommandAction::isUseful()
{
    // The spell fails (without saying why) unless the pet is within 25 yards of the target
    Pet* pet = bot->GetPet();
    Unit* target = GetTarget();
    return pet && pet->IsAlive() && target && pet->IsWithinDist(target, 25.0f, true) && CastSpellAction::isUseful();
}

bool CastAspectOfTheHawkAction::isUseful()
{
    // The talent replaces it
    return !bot->HasSpell(109260) /*Aspect of the Iron Hawk*/ && CastBuffSpellAction::isUseful();
}
