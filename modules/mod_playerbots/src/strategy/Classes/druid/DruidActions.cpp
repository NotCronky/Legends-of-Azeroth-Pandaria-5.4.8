/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DruidActions.h"

#include "Event.h"
#include "Playerbots.h"

bool CastCasterFormAction::isUseful()
{
    return botAI->HasAnyAuraOf(bot, "bear form", "cat form", "travel form", "aquatic form", "flight form",
                               "swift flight form", nullptr);
}

bool CastCasterFormAction::Execute(Event /*event*/)
{
    botAI->RemoveShapeshift();
    return true;
}
