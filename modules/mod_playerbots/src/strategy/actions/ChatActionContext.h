/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CHATACTIONCONTEXT_H
#define _PLAYERBOT_CHATACTIONCONTEXT_H

#include "AttackActions.h"
#include "ChangeStrategyAction.h"
#include "ChatShortcutActions.h"
#include "EquipAction.h"
#include "HelpAction.h"
#include "LeaveGroupAction.h"
#include "ListQuestsAction.h"
#include "ListSpellsAction.h"
#include "NamedObjectContext.h"
#include "RtiAction.h"
#include "SellAction.h"
#include "StatsAction.h"
#include "SummonAction.h"
#include "TalentsAction.h"
#include "TellItemCountAction.h"
#include "TradeAction.h"
#include "UseItemAction.h"

// Actions run by chat commands
class ChatActionContext : public NamedObjectContext<Action>
{
public:
    ChatActionContext()
    {
        creators["co"] = &ChatActionContext::co;
        creators["nc"] = &ChatActionContext::nc;
        creators["de"] = &ChatActionContext::de;
        creators["follow chat shortcut"] = &ChatActionContext::follow_chat_shortcut;
        creators["stay chat shortcut"] = &ChatActionContext::stay_chat_shortcut;
        creators["flee chat shortcut"] = &ChatActionContext::flee_chat_shortcut;
        creators["runaway chat shortcut"] = &ChatActionContext::runaway_chat_shortcut;
        creators["grind chat shortcut"] = &ChatActionContext::grind_chat_shortcut;
        creators["move from group chat shortcut"] = &ChatActionContext::move_from_group_chat_shortcut;
        creators["tank attack chat shortcut"] = &ChatActionContext::tank_attack_chat_shortcut;
        creators["max dps chat shortcut"] = &ChatActionContext::max_dps_chat_shortcut;
        creators["attack my target"] = &ChatActionContext::attack_my_target;
        creators["summon"] = &ChatActionContext::summon;
        creators["leave"] = &ChatActionContext::leave;
        creators["rti"] = &ChatActionContext::rti;
        creators["stats"] = &ChatActionContext::stats;
        creators["help"] = &ChatActionContext::help;
        creators["c"] = &ChatActionContext::item_count;
        creators["equip"] = &ChatActionContext::equip;
        creators["unequip"] = &ChatActionContext::unequip;
        creators["sell"] = &ChatActionContext::sell;
        creators["use"] = &ChatActionContext::use;
        creators["trade"] = &ChatActionContext::trade;
        creators["quests"] = &ChatActionContext::quests;
        creators["spells"] = &ChatActionContext::spells;
        creators["talents"] = &ChatActionContext::talents;
    }

private:
    static Action* co(PlayerbotAI* botAI) { return new ChangeCombatStrategyAction(botAI); }
    static Action* nc(PlayerbotAI* botAI) { return new ChangeNonCombatStrategyAction(botAI); }
    static Action* de(PlayerbotAI* botAI) { return new ChangeDeadStrategyAction(botAI); }
    static Action* follow_chat_shortcut(PlayerbotAI* botAI) { return new FollowChatShortcutAction(botAI); }
    static Action* stay_chat_shortcut(PlayerbotAI* botAI) { return new StayChatShortcutAction(botAI); }
    static Action* flee_chat_shortcut(PlayerbotAI* botAI) { return new FleeChatShortcutAction(botAI); }
    static Action* runaway_chat_shortcut(PlayerbotAI* botAI) { return new GoawayChatShortcutAction(botAI); }
    static Action* grind_chat_shortcut(PlayerbotAI* botAI) { return new GrindChatShortcutAction(botAI); }
    static Action* move_from_group_chat_shortcut(PlayerbotAI* botAI) { return new MoveFromGroupChatShortcutAction(botAI); }
    static Action* tank_attack_chat_shortcut(PlayerbotAI* botAI) { return new TankAttackChatShortcutAction(botAI); }
    static Action* max_dps_chat_shortcut(PlayerbotAI* botAI) { return new MaxDpsChatShortcutAction(botAI); }
    static Action* attack_my_target(PlayerbotAI* botAI) { return new AttackMyTargetAction(botAI); }
    static Action* summon(PlayerbotAI* botAI) { return new SummonAction(botAI); }
    static Action* leave(PlayerbotAI* botAI) { return new LeaveGroupAction(botAI); }
    static Action* rti(PlayerbotAI* botAI) { return new RtiAction(botAI); }
    static Action* stats(PlayerbotAI* botAI) { return new StatsAction(botAI); }
    static Action* help(PlayerbotAI* botAI) { return new HelpAction(botAI); }
    static Action* item_count(PlayerbotAI* botAI) { return new TellItemCountAction(botAI); }
    static Action* equip(PlayerbotAI* botAI) { return new EquipAction(botAI); }
    static Action* unequip(PlayerbotAI* botAI) { return new UnequipAction(botAI); }
    static Action* sell(PlayerbotAI* botAI) { return new SellAction(botAI); }
    static Action* use(PlayerbotAI* botAI) { return new UseItemAction(botAI); }
    static Action* trade(PlayerbotAI* botAI) { return new TradeAction(botAI); }
    static Action* quests(PlayerbotAI* botAI) { return new ListQuestsAction(botAI); }
    static Action* spells(PlayerbotAI* botAI) { return new ListSpellsAction(botAI); }
    static Action* talents(PlayerbotAI* botAI) { return new TalentsAction(botAI); }
};

#endif
