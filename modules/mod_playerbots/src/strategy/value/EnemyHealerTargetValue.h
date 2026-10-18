/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_ENEMYHEALERTARGETVALUE_H
#define _PLAYERBOT_ENEMYHEALERTARGETVALUE_H

#include "NamedObjectContext.h"
#include "Value.h"

class PlayerbotAI;
class Unit;

// An attacker in spell range, other than the current target, casting a beneficial spell that the qualifier
// (an interrupt such as "kick") can stop: the "... on enemy healer" triggers and actions
class EnemyHealerTargetValue : public UnitCalculatedValue, public Qualified
{
public:
    EnemyHealerTargetValue(PlayerbotAI* botAI) : UnitCalculatedValue(botAI, "enemy healer target") {}

protected:
    Unit* Calculate() override;
};

#endif
