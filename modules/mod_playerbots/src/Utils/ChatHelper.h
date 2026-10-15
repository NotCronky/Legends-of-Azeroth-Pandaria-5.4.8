/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CHATHELPER_H
#define _PLAYERBOT_CHATHELPER_H

#include <string>

#include "Define.h"

// Formatting for bot chat replies (grows with the chat command port)
class ChatHelper
{
public:
    static std::string const formatMoney(uint32 copper);
};

#endif
