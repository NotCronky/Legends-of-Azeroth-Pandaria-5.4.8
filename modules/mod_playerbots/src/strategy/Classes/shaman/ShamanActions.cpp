/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ShamanActions.h"

#include "Playerbots.h"

bool CastTotemAction::isUseful()
{
    if (needLifeTime > 0.1f && AI_VALUE(uint8, "attacker count") < 3)
    {
        Unit* target = AI_VALUE(Unit*, "current target");
        if (!target)
            return false;

        float dps = AI_VALUE(float, "estimated group dps");
        if (target->GetHealth() / dps < needLifeTime)
            return false;
    }

    return CastBuffSpellAction::isUseful() && !AI_VALUE2(bool, "has totem", name) && !botAI->HasAura(buff, bot);
}
