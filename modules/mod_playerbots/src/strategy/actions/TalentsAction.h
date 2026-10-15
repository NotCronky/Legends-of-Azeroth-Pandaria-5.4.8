/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_TALENTSACTION_H
#define _PLAYERBOT_TALENTSACTION_H

#include "Action.h"

class PlayerbotAI;

struct TalentEntry;

// "talents": the bot's specialization and its talent in each unlocked tier.
// "talents spec <name>" switches specialization (and picks talents for it); "talents <talent name or link>"
// takes that talent, replacing the one in its tier
class TalentsAction : public Action
{
public:
    TalentsAction(PlayerbotAI* botAI) : Action(botAI, "talents") {}

    bool Execute(Event event) override;

private:
    void ListTalents();
    bool SwitchSpec(std::string const& name);
    bool ChooseTalent(std::string const& name);
    TalentEntry const* FindTalent(std::string const& text);
};

#endif
