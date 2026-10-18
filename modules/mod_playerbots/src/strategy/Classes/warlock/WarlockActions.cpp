/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "WarlockActions.h"

#include "Playerbots.h"

bool CastLifeTapAction::isUseful() { return AI_VALUE2(uint8, "health", "self target") > sPlayerbotAIConfig->mediumHealth; }

bool CastSummonDemonAction::isUseful() { return !bot->GetPet() && !bot->GetGuardianPet() && !bot->IsMounted(); }

bool CastSummonDemonAction::Execute(Event /*event*/)
{
    std::vector<std::string> demons = { "summon voidwalker", "summon imp" };
    switch (bot->GetSpecialization())
    {
        case Specializations::SPEC_WARLOCK_AFFLICTION:
            demons.insert(demons.begin(), "summon felhunter");
            break;
        case Specializations::SPEC_WARLOCK_DEMONOLOGY:
            demons.insert(demons.begin(), { "summon felguard", "summon felhunter" });
            break;
        default:
            demons = { "summon imp" };
            break;
    }

    for (std::string const& demon : demons)
        if (botAI->CanCastSpell(demon, bot))
            return botAI->CastSpell(demon, bot);

    return false;
}
