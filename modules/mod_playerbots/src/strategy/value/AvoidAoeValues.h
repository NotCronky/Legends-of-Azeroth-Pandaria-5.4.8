/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_AVOIDAOEVALUES_H
#define _PLAYERBOT_AVOIDAOEVALUES_H

#include "Value.h"

class Aura;
class PlayerbotAI;

// A harmful aura on the bot that comes from a ground effect (a dynamic object such as Rain of Fire), for
// "avoid aoe" to step out of
class AreaDebuffValue : public CalculatedValue<Aura*>
{
public:
    AreaDebuffValue(PlayerbotAI* botAI) : CalculatedValue<Aura*>(botAI, "area debuff", 1) {}

    Aura* Calculate() override;
};

// Hostile traps nearby whose spell does damage (lava bombs and the like)
class NearestTrapWithDamageValue : public ObjectGuidListCalculatedValue
{
public:
    NearestTrapWithDamageValue(PlayerbotAI* botAI, float range = 15.0f)
        : ObjectGuidListCalculatedValue(botAI, "nearest trap with damage", 1 * 1000), range(range)
    {
    }

protected:
    GuidVector Calculate() override;

private:
    float range;
};

#endif
