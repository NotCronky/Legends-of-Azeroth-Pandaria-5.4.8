/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DKActions.h"

#include "Playerbots.h"

bool CastIcyTouchAction::isUseful()
{
    Unit* target = GetTarget();
    return target && !botAI->HasAura("frost fever", target, false, true) && CastSpellAction::isUseful();
}

bool CastPlagueStrikeAction::isUseful()
{
    Unit* target = GetTarget();
    return target && !botAI->HasAura("blood plague", target, false, true) && CastMeleeSpellAction::isUseful();
}

bool CastRaiseDeadAction::isUseful() { return !bot->GetPet() && !bot->GetGuardianPet() && CastBuffSpellAction::isUseful(); }
