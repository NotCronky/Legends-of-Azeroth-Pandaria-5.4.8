/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ListQuestsAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"

bool ListQuestsAction::Execute(Event event)
{
    std::string const filter = event.getParam();
    bool const all = filter == "all";
    bool const showCompleted = all || filter == "co" || filter == "completed";
    bool const showIncomplete = all || filter == "in" || filter == "incompleted" || filter == "incomplete";

    if (showIncomplete)
        botAI->TellMaster("--- Incomplete quests ---");

    uint32 const incomplete = ListQuests(false, !showIncomplete);

    if (showCompleted)
        botAI->TellMaster("--- Completed quests ---");

    uint32 const completed = ListQuests(true, !showCompleted);

    std::ostringstream out;
    out << "Quests: " << (completed + incomplete) << " / " << MAX_QUEST_LOG_SIZE << " (incomplete: " << incomplete
        << ", completed: " << completed << ")";
    botAI->TellMaster(out);
    return true;
}

uint32 ListQuestsAction::ListQuests(bool completed, bool silent)
{
    uint32 count = 0;
    for (uint16 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
    {
        uint32 const questId = bot->GetQuestSlotQuestId(slot);
        if (!questId)
            continue;

        Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
        if (!quest || (bot->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE) != completed)
            continue;

        ++count;
        if (!silent)
            botAI->TellMaster(ChatHelper::FormatQuest(quest));
    }

    return count;
}
