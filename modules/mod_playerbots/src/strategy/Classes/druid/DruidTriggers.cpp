/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DruidTriggers.h"

#include "Playerbots.h"
#include "SpellAuras.h"

bool MarkOfTheWildTrigger::IsActive()
{
    // Not castable in cat or bear form
    return bot->HasSpell(1126) /*Mark of the Wild*/ && !botAI->HasAnyAuraOf(bot, "cat form", "bear form", nullptr) &&
           !botAI->HasAnyAuraOf(bot, "mark of the wild", "blessing of kings", "legacy of the emperor",
                                "embrace of the shale spider", nullptr);
}

bool InAnimalFormTrigger::IsActive()
{
    return botAI->HasAnyAuraOf(bot, "cat form", "bear form", "travel form", "aquatic form", nullptr);
}

bool StarfirePhaseTrigger::IsActive()
{
    if (botAI->HasAura("eclipse (lunar)", bot))
        return true;

    if (botAI->HasAura("eclipse (solar)", bot))
        return false;

    return int32(bot->GetPower(POWER_ECLIPSE)) > 0;
}

bool DruidFinisherTrigger::IsActive() { return AI_VALUE2(uint8, "combo", "current target") >= 5; }

bool RipTrigger::IsActive()
{
    Unit* target = AI_VALUE(Unit*, "current target");
    return DruidFinisherTrigger::IsActive() && target && target->GetHealthPct() > 25.0f &&
           !botAI->HasAura("rip", target, false, true);
}

bool SavageRoarTrigger::IsActive()
{
    if (!bot->HasSpell(52610) /*Savage Roar*/ || !AI_VALUE2(uint8, "combo", "current target"))
        return false;

    Aura* aura = botAI->GetAura("savage roar", bot);
    return !aura || aura->GetDuration() < 3 * IN_MILLISECONDS;
}

bool TigersFuryTrigger::IsActive() { return bot->GetPower(POWER_ENERGY) < 35 && SpellCanBeCastTrigger::IsActive(); }

bool DruidRageTrigger::IsActive()
{
    // Rage is kept in tenths
    return bot->GetPower(POWER_RAGE) >= amount * 10;
}
