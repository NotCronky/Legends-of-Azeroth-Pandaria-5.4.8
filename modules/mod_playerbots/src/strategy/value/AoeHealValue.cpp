/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "AoeHealValue.h"

#include "Group.h"
#include "Playerbots.h"

uint8 AoeHealValue::Calculate()
{
    Group* group = bot->GetGroup();
    if (!group)
        return 0;

    float threshold = sPlayerbotAIConfig->mediumHealth;
    if (qualifier == "critical")
        threshold = sPlayerbotAIConfig->criticalHealth;
    else if (qualifier == "low")
        threshold = sPlayerbotAIConfig->lowHealth;
    else if (qualifier == "almost full")
        threshold = sPlayerbotAIConfig->almostFullHealth;

    uint8 count = 0;
    for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
    {
        Player* member = ref->GetSource();
        if (!member || !member->IsAlive() || !member->IsInWorld() || member->GetMap() != bot->GetMap())
            continue;

        if (member->GetDistance2d(bot) > sPlayerbotAIConfig->healDistance)
            continue;

        if (member->GetHealthPct() < threshold)
            ++count;
    }

    return count;
}
