/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "RogueActions.h"

#include "Event.h"
#include "Playerbots.h"

namespace
{
    // Warsong Gulch flags and the Eye of the Storm flag
    bool CarriesFlag(PlayerbotAI* botAI, Player* bot)
    {
        return botAI->HasAura(23333, bot) || botAI->HasAura(23335, bot) || botAI->HasAura(34976, bot);
    }
}

bool CastStealthAction::isPossible() { return !CarriesFlag(botAI, bot) && CastBuffSpellAction::isPossible(); }

bool CastVanishAction::isUseful() { return !CarriesFlag(botAI, bot) && CastBuffSpellAction::isUseful(); }

bool UnstealthAction::Execute(Event /*event*/)
{
    botAI->RemoveAura("stealth");
    return true;
}

bool CastTricksOfTheTradeOnMainTankAction::isUseful()
{
    return CastSpellAction::isUseful() && AI_VALUE2(float, "distance", GetTargetName()) < 20.0f;
}
