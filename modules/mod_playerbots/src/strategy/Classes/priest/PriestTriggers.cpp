/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PriestTriggers.h"

#include "Playerbots.h"

bool PowerWordFortitudeTrigger::IsActive()
{
    return bot->HasSpell(21562) /*Power Word: Fortitude*/ &&
           !botAI->HasAnyAuraOf(bot, "power word: fortitude", "commanding shout", "blood pact", "qiraji fortitude",
                                nullptr);
}

bool ShadowfiendTrigger::IsActive()
{
    Unit* target = AI_VALUE(Unit*, "current target");
    return target && (botAI->CanCastSpell("mindbender", target) || botAI->CanCastSpell("shadowfiend", target));
}
