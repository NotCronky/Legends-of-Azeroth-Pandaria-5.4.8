/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "SummonAction.h"

#include <cmath>

#include "Event.h"
#include "Playerbots.h"

bool SummonAction::Execute(Event /*event*/)
{
    Player* master = GetMaster();
    if (!master || master == bot)
        return false;

    if (bot->GetVehicle())
    {
        botAI->TellError("You cannot summon me while I'm on a vehicle");
        return false;
    }

    if (master->IsInCombat())
    {
        botAI->TellError("You cannot summon me while you're in combat");
        return false;
    }

    if (!master->IsAlive())
    {
        botAI->TellError("You cannot summon me while you're dead");
        return false;
    }

    if (master->IsBeingTeleported() || bot->IsBeingTeleported())
        return false;

    // A spot beside the master that the master can see, starting behind them
    float const baseAngle = master->GetOrientation() + M_PI;
    float x = master->GetPositionX(), y = master->GetPositionY(), z = master->GetPositionZ();
    for (float delta = 0.0f; delta < 2 * M_PI; delta += M_PI / 4)
    {
        float const tryX = master->GetPositionX() + cos(baseAngle + delta) * sPlayerbotAIConfig->followDistance;
        float const tryY = master->GetPositionY() + sin(baseAngle + delta) * sPlayerbotAIConfig->followDistance;
        if (master->IsWithinLOS(tryX, tryY, z))
        {
            x = tryX;
            y = tryY;
            break;
        }
    }

    if (bot->isDead())
    {
        bot->ResurrectPlayer(1.0f);
        bot->SpawnCorpseBones();
    }

    bot->CombatStop(true);
    botAI->Reset();
    if (!bot->TeleportTo(master->GetMapId(), x, y, z, master->GetOrientation()))
        return false;

    botAI->TellMasterNoFacing("Hello!");
    return true;
}
