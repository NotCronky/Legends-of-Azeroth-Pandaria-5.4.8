/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SPELLNAMEAUDIT_H
#define _PLAYERBOT_SPELLNAMEAUDIT_H

#include "Define.h"

class AiObjectContext;

namespace SpellNameAudit
{
    // Logs, once per class, the spells named by that class's cast actions that the class can't learn in this
    // client (e.g. abilities removed after WotLK). Bots look spells up by name, so those actions never fire.
    void CheckClass(uint8 cls, AiObjectContext* context);
}

#endif
