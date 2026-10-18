/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "RogueTriggers.h"

#include "Playerbots.h"
#include "SpellAuras.h"

uint8 RogueComboPoints(PlayerbotAI* botAI, Player* bot)
{
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("current target")->Get();
    if (!target || bot->GetComboTarget() != target)
        return 0;

    return bot->GetComboPoints();
}

bool RogueComboPointsTrigger::IsActive() { return RogueComboPoints(botAI, bot) >= amount; }

bool RogueTargetDyingTrigger::IsActive()
{
    Unit* target = AI_VALUE(Unit*, "current target");
    return target && target->GetHealthPct() < 15.0f && RogueComboPointsTrigger::IsActive();
}

bool SliceAndDiceTrigger::IsActive()
{
    if (!bot->HasSpell(5171) /*Slice and Dice*/ || !RogueComboPoints(botAI, bot))
        return false;

    Aura* aura = botAI->GetAura("slice and dice", bot);
    return !aura || aura->GetDuration() < 3 * IN_MILLISECONDS;
}

bool RuptureTrigger::IsActive() { return RogueComboPoints(botAI, bot) >= 5 && DebuffTrigger::IsActive(); }

bool RogueStealthedTrigger::IsActive()
{
    return botAI->HasAnyAuraOf(bot, "stealth", "vanish", "shadow dance", "subterfuge", nullptr);
}

bool ShadowDanceTrigger::IsActive()
{
    return bot->GetPower(POWER_ENERGY) >= 75 && SpellCanBeCastTrigger::IsActive();
}

bool KillingSpreeTrigger::IsActive()
{
    return bot->GetPower(POWER_ENERGY) < 40 && !botAI->HasAura("adrenaline rush", bot) &&
           SpellCanBeCastTrigger::IsActive();
}

bool RoguePoisonTrigger::IsActive()
{
    if (lethal)
        return (bot->HasSpell(2823) /*Deadly Poison*/ || bot->HasSpell(8679) /*Wound Poison*/) &&
               !botAI->HasAnyAuraOf(bot, "deadly poison", "wound poison", nullptr);

    return (bot->HasSpell(3408) /*Crippling Poison*/ || bot->HasSpell(5761) /*Mind-numbing Poison*/) &&
           !botAI->HasAnyAuraOf(bot, "crippling poison", "mind-numbing poison", "leeching poison",
                                "paralytic poison", nullptr);
}
