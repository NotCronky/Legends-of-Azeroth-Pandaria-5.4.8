/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MonkTriggers.h"

#include "Playerbots.h"

bool LegacyOfTheEmperorTrigger::IsActive()
{
    return bot->HasSpell(115921) /*Legacy of the Emperor*/ &&
           !botAI->HasAnyAuraOf(bot, "legacy of the emperor", "mark of the wild", "blessing of kings",
                                "embrace of the shale spider", nullptr);
}

bool HeavyStaggerTrigger::IsActive() { return botAI->HasAnyAuraOf(bot, "moderate stagger", "heavy stagger", nullptr); }

bool MonkLowEnergyTrigger::IsActive() { return bot->GetPower(POWER_ENERGY) < 40 && SpellCanBeCastTrigger::IsActive(); }

bool TouchOfDeathTrigger::IsActive()
{
    Unit* target = GetTarget();
    return target && (target->GetHealth() < bot->GetHealth() || target->GetHealthPct() < 10.0f) &&
           SpellCanBeCastTrigger::IsActive();
}
