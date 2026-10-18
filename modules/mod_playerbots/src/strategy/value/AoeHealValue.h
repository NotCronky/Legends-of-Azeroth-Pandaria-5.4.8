/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_AOEHEALVALUE_H
#define _PLAYERBOT_AOEHEALVALUE_H

#include "NamedObjectContext.h"
#include "Value.h"

class PlayerbotAI;

// Group members in heal range whose health is below a level: the qualifier is "critical", "low", "medium" or
// "almost full" (the configured health thresholds). Read by the "... aoe heal" triggers
class AoeHealValue : public Uint8CalculatedValue, public Qualified
{
public:
    AoeHealValue(PlayerbotAI* botAI, std::string const name = "aoe heal") : Uint8CalculatedValue(botAI, name) {}

    uint8 Calculate() override;
};

#endif
