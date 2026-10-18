/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PriestActions.h"

#include "Playerbots.h"

bool CastPowerWordShieldOnPartyAction::isUseful()
{
    Unit* target = GetTarget();
    return target && !botAI->HasAura("weakened soul", target) && HealPartyMemberAction::isUseful();
}

bool CastPowerWordShieldAction::isUseful() { return !botAI->HasAura("weakened soul", bot) && CastBuffSpellAction::isUseful(); }
