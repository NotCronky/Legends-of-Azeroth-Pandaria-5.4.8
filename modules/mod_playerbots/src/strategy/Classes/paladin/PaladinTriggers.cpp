/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PaladinTriggers.h"

#include "Playerbots.h"

bool BlessingOfKingsTrigger::IsActive()
{
    return bot->HasSpell(20217) /*Blessing of Kings*/ &&
           !botAI->HasAnyAuraOf(bot, "blessing of kings", "mark of the wild", "legacy of the emperor",
                                "embrace of the shale spider", nullptr);
}

bool BlessingOfMightTrigger::IsActive()
{
    // Kings is up but not ours: another paladin, druid or monk has the stats buff covered
    return bot->HasSpell(19740) /*Blessing of Might*/ &&
           botAI->HasAnyAuraOf(bot, "blessing of kings", "mark of the wild", "legacy of the emperor",
                               "embrace of the shale spider", nullptr) &&
           !botAI->HasAura("blessing of kings", bot, false, true) &&
           !botAI->HasAnyAuraOf(bot, "blessing of might", "grace of air", "roar of courage", "spirit beast blessing",
                                nullptr);
}
