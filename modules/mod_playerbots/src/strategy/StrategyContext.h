#ifndef _PLAYERBOT_STRATEGYCONTEXT_H
#define _PLAYERBOT_STRATEGYCONTEXT_H


#include "CustomStrategy.h"

#include "NamedObjectContext.h"

#include "ChatCommandHandlerStrategy.h"
#include "ConserveManaStrategy.h"
#include "DeadStrategy.h"
#include "DpsAssistStrategy.h"
#include "FleeStrategy.h"
#include "FollowMasterStrategy.h"
#include "GrindingStrategy.h"
#include "GuardStrategy.h"
#include "GroupStrategy.h"
#include "LootNonCombatStrategy.h"
#include "MeleeCombatStrategy.h"
#include "MoveFromGroupStrategy.h"
#include "PassiveStrategy.h"
#include "NewRpgStrategy.h"
#include "TankAssistStrategy.h"
#include "UpkeepStrategies.h"
#include "ThreatStrategy.h"
#include "HelloStrategy.h"
#include "RunawayStrategy.h"
#include "RangedCombatStrategy.h"
#include "StayStrategy.h"
#include "WorldPacketHandlerStrategy.h"

class StrategyContext : public NamedObjectContext<Strategy>
{
public:
    StrategyContext()
    {
        creators["default"] = &StrategyContext::world_packet;
        creators["custom"] = &StrategyContext::custom;

        creators["save mana"] = &StrategyContext::auto_save_mana;
        creators["close"] = &StrategyContext::close;
        creators["ranged"] = &StrategyContext::ranged;
        creators["dead"] = &StrategyContext::dead;
        creators["group"] = &StrategyContext::group;

        creators["formation"] = &StrategyContext::combat_formation;
        creators["move from group"] = &StrategyContext::move_from_group;

        creators["tank face"] = &StrategyContext::tank_face;
        creators["behind"] = &StrategyContext::behind;
        creators["avoid aoe"] = &StrategyContext::avoid_aoe;
        creators["threat"] = &StrategyContext::threat;
        // -- temp
        creators["say hello"] = &StrategyContext::say_hello;
        creators["new rpg"] = &StrategyContext::new_rpg;
        creators["chat"] = &StrategyContext::chat;
        creators["passive"] = &StrategyContext::passive;
        creators["grind"] = &StrategyContext::grind;
        creators["move random"] = &StrategyContext::move_random;
        creators["loot"] = &StrategyContext::loot;
        creators["gather"] = &StrategyContext::gather;
        creators["food"] = &StrategyContext::food;
        creators["potions"] = &StrategyContext::potions;
        creators["vendor"] = &StrategyContext::vendor;
        creators["mount"] = &StrategyContext::mount;
    }

private:
    // -- temp
    static Strategy* say_hello(PlayerbotAI* botAI) { return new HelloStrategy(botAI); }
    static Strategy* new_rpg(PlayerbotAI* botAI) { return new NewRpgStrategy(botAI); }
    static Strategy* chat(PlayerbotAI* botAI) { return new ChatCommandHandlerStrategy(botAI); }
    static Strategy* passive(PlayerbotAI* botAI) { return new PassiveStrategy(botAI); }
    static Strategy* grind(PlayerbotAI* botAI) { return new GrindingStrategy(botAI); }
    static Strategy* move_random(PlayerbotAI* botAI) { return new MoveRandomStrategy(botAI); }
    static Strategy* loot(PlayerbotAI* botAI) { return new LootNonCombatStrategy(botAI); }
    static Strategy* gather(PlayerbotAI* botAI) { return new GatherStrategy(botAI); }
    static Strategy* food(PlayerbotAI* botAI) { return new UseFoodStrategy(botAI); }
    static Strategy* potions(PlayerbotAI* botAI) { return new UsePotionsStrategy(botAI); }
    static Strategy* vendor(PlayerbotAI* botAI) { return new VendorStrategy(botAI); }
    static Strategy* mount(PlayerbotAI* botAI) { return new MountStrategy(botAI); }
    // --
    
    static Strategy* world_packet(PlayerbotAI* botAI) { return new WorldPacketHandlerStrategy(botAI); }
    static Strategy* custom(PlayerbotAI* botAI) { return new CustomStrategy(botAI); }

    static Strategy* follow_master(PlayerbotAI* botAI) { return new FollowMasterStrategy(botAI); }
    static Strategy* auto_save_mana(PlayerbotAI* botAI) { return new HealerAutoSaveManaStrategy(botAI); }
    static Strategy* close(PlayerbotAI* botAI) { return new MeleeCombatStrategy(botAI); }
    static Strategy* ranged(PlayerbotAI* botAI) { return new RangedCombatStrategy(botAI); }
    static Strategy* dead(PlayerbotAI* botAI) { return new DeadStrategy(botAI); }
    static Strategy* group(PlayerbotAI* botAI) { return new GroupStrategy(botAI); }
    static Strategy* combat_formation(PlayerbotAI* ai) { return new CombatFormationStrategy(ai); }
    static Strategy* move_from_group(PlayerbotAI* botAI) { return new MoveFromGroupStrategy(botAI); }

    static Strategy* tank_face(PlayerbotAI* botAI) { return new TankFaceStrategy(botAI); }
    static Strategy* behind(PlayerbotAI* botAI) { return new SetBehindCombatStrategy(botAI); }
    static Strategy* avoid_aoe(PlayerbotAI* botAI) { return new AvoidAoeStrategy(botAI); }
    static Strategy* threat(PlayerbotAI* botAI) { return new ThreatStrategy(botAI); }
};

class MovementStrategyContext : public NamedObjectContext<Strategy>
{
public:
    MovementStrategyContext() : NamedObjectContext<Strategy>(false, true)
    {
        creators["follow"] = &MovementStrategyContext::follow_master;
        creators["stay"] = &MovementStrategyContext::stay;
        creators["runaway"] = &MovementStrategyContext::runaway;
        creators["flee from adds"] = &MovementStrategyContext::flee_from_adds;
        creators["guard"] = &MovementStrategyContext::guard;
    }

private:
    static Strategy* guard(PlayerbotAI* botAI) { return new GuardStrategy(botAI); }
    static Strategy* follow_master(PlayerbotAI* botAI) { return new FollowMasterStrategy(botAI); }
    static Strategy* stay(PlayerbotAI* botAI) { return new StayStrategy(botAI); }
    static Strategy* runaway(PlayerbotAI* botAI) { return new RunawayStrategy(botAI); }
    static Strategy* flee_from_adds(PlayerbotAI* botAI) { return new FleeFromAddsStrategy(botAI); }
};

class AssistStrategyContext : public NamedObjectContext<Strategy>
{
public:
    AssistStrategyContext() : NamedObjectContext<Strategy>(false, true)
    {
        creators["dps assist"] = &AssistStrategyContext::dps_assist;
        creators["dps aoe"] = &AssistStrategyContext::dps_aoe;
        creators["tank assist"] = &AssistStrategyContext::tank_assist;
    }

private:
    static Strategy* dps_assist(PlayerbotAI* botAI) { return new DpsAssistStrategy(botAI); }
    static Strategy* dps_aoe(PlayerbotAI* botAI) { return new DpsAoeStrategy(botAI); }
    static Strategy* tank_assist(PlayerbotAI* botAI) { return new TankAssistStrategy(botAI); }
};

#endif
