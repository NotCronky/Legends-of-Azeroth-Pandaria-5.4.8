#ifndef _PLAYERBOT_ACTIONCONTEXT_H
#define _PLAYERBOT_ACTIONCONTEXT_H

#include "AttackActions.h"
#include "ActionSayHello.h"
#include "AutoMaintenanceOnLevelupAction.h"
#include "ChooseTargetActions.h"
#include "FollowActions.h"
#include "ConsumableActions.h"
#include "EquipAction.h"
#include "GenericActions.h"
#include "LootActions.h"
#include "MountActions.h"
#include "PositionAction.h"
#include "RtiAction.h"
#include "MovementActions.h"
#include "NewRpgAction.h"
#include "RandomBotUpdateAction.h"
#include "ReachTargetActions.h"
#include "StayActions.h"
#include "VendorActions.h"

class PlayerbotAI;
class ActionContext : public NamedObjectContext<Action>
{
public:
    ActionContext()
    {
        creators["say hello"] = &ActionContext::hello;
        creators["move random"] = &ActionContext::move_random;

        creators["follow"] = &ActionContext::follow;
        creators["flee"] = &ActionContext::flee;
        creators["move from group"] = &ActionContext::move_from_group;
        creators["flee to master"] = &ActionContext::flee_to_master;
        creators["guard"] = &ActionContext::guard;
        creators["runaway"] = &ActionContext::runaway;
        creators["stay"] = &ActionContext::stay;
        creators["sit"] = &ActionContext::sit;

        creators["random bot update"] = &ActionContext::random_bot_update;
        creators["rpg status"] = &ActionContext::rpg_status;
        creators["new rpg status update"] = &ActionContext::new_rpg_status_update;
        creators["new rpg go grind"] = &ActionContext::new_rpg_go_grind;
        creators["new rpg go innkeeper"] = &ActionContext::new_rpg_go_innkeeper;
        creators["new rpg move random"] = &ActionContext::new_rpg_move_random;
        creators["new rpg move npc"] = &ActionContext::new_rpg_move_npc;
        creators["mark rti"] = &ActionContext::mark_rti;
        creators["attack rti target"] = &ActionContext::attack_rti_target;
        creators["auto maintenance on levelup"] = &ActionContext::auto_maintenance_on_levelup;

        creators["loot"] = &ActionContext::loot;
        creators["move to loot"] = &ActionContext::move_to_loot;
        creators["open loot"] = &ActionContext::open_loot;
        creators["store loot"] = &ActionContext::store_loot;
        creators["add all loot"] = &ActionContext::add_all_loot;
        creators["add gathering loot"] = &ActionContext::add_gathering_loot;
        creators["loot roll"] = &ActionContext::loot_roll;
        creators["equip upgrades"] = &ActionContext::equip_upgrades;

        creators["food"] = &ActionContext::food;
        creators["drink"] = &ActionContext::drink;
        creators["healing potion"] = &ActionContext::healing_potion;
        creators["mana potion"] = &ActionContext::mana_potion;
        creators["healthstone"] = &ActionContext::healthstone;
        creators["sell junk"] = &ActionContext::sell_junk;
        creators["check mount state"] = &ActionContext::check_mount_state;
        creators["dismount"] = &ActionContext::dismount;

        creators["tank face"] = &ActionContext::tank_face;
        creators["tank assist"] = &ActionContext::tank_assist;
        creators["set facing"] = &ActionContext::set_facing;
        creators["drop target"] = &ActionContext::drop_target;
        creators["dps assist"] = &ActionContext::dps_assist;
        creators["dps aoe"] = &ActionContext::dps_aoe;
        creators["attack"] = &ActionContext::melee;
        creators["melee"] = &ActionContext::melee;
        creators["attack anything"] = &ActionContext::attack_anything;

        creators["toggle pet spell"] = &ActionContext::toggle_pet_spell;
        creators["pet attack"] = &ActionContext::pet_attack;

        creators["reach spell"] = &ActionContext::ReachSpell;
        creators["reach melee"] = &ActionContext::ReachMelee;
        creators["reach party member to heal"] = &ActionContext::reach_party_member_to_heal;
        creators["reach party member to resurrect"] = &ActionContext::reach_party_member_to_resurrect;

        //creators["invite nearby"] = &ActionContext::invite_nearby;
        //creators["invite guild"] = &ActionContext::invite_guild;
    }

private:
    static Action* hello(PlayerbotAI* botAI) { return new SayHelloAction(botAI); }
    static Action* move_random(PlayerbotAI* botAI) { return new MoveRandomAction(botAI); }

    static Action* follow(PlayerbotAI* botAI) { return new FollowAction(botAI); }
    static Action* flee(PlayerbotAI* botAI) { return new FleeAction(botAI); }
    static Action* move_from_group(PlayerbotAI* botAI) { return new MoveFromGroupAction(botAI); }
    static Action* flee_to_master(PlayerbotAI* botAI) { return new FleeToMasterAction(botAI); }
    static Action* guard(PlayerbotAI* botAI) { return new GuardAction(botAI); }
    static Action* stay(PlayerbotAI* botAI) { return new StayAction(botAI); }
    static Action* sit(PlayerbotAI* botAI) { return new SitAction(botAI); }
    static Action* runaway(PlayerbotAI* botAI) { return new RunAwayAction(botAI); }

    static Action* random_bot_update(PlayerbotAI* botAI) { return new RandomBotUpdateAction(botAI); }
    static Action* rpg_status(PlayerbotAI* botAI) { return new TellRpgStatusAction(botAI); }
    static Action* new_rpg_status_update(PlayerbotAI* botAI) { return new NewRpgStatusUpdateAction(botAI); }
    static Action* new_rpg_go_grind(PlayerbotAI* botAI) { return new NewRpgGoGrindAction(botAI); }
    static Action* new_rpg_go_innkeeper(PlayerbotAI* botAI) { return new NewRpgGoInnKeeperAction(botAI); }
    static Action* new_rpg_move_random(PlayerbotAI* botAI) { return new NewRpgMoveRandomAction(botAI); }
    static Action* new_rpg_move_npc(PlayerbotAI* botAI) { return new NewRpgMoveNpcAction(botAI); }
    static Action* mark_rti(PlayerbotAI* botAI) { return new MarkRtiAction(botAI); }
    static Action* attack_rti_target(PlayerbotAI* botAI) { return new AttackRtiTargetAction(botAI); }
    static Action* auto_maintenance_on_levelup(PlayerbotAI* botAI) { return new AutoMaintenanceOnLevelupAction(botAI); }

    static Action* loot(PlayerbotAI* botAI) { return new LootAction(botAI); }
    static Action* move_to_loot(PlayerbotAI* botAI) { return new MoveToLootAction(botAI); }
    static Action* open_loot(PlayerbotAI* botAI) { return new OpenLootAction(botAI); }
    static Action* store_loot(PlayerbotAI* botAI) { return new StoreLootAction(botAI); }
    static Action* add_all_loot(PlayerbotAI* botAI) { return new AddAllLootAction(botAI); }
    static Action* add_gathering_loot(PlayerbotAI* botAI) { return new AddGatheringLootAction(botAI); }
    static Action* loot_roll(PlayerbotAI* botAI) { return new LootRollAction(botAI); }
    static Action* equip_upgrades(PlayerbotAI* botAI) { return new EquipUpgradesAction(botAI); }

    static Action* food(PlayerbotAI* botAI) { return new EatAction(botAI); }
    static Action* drink(PlayerbotAI* botAI) { return new DrinkAction(botAI); }
    static Action* healing_potion(PlayerbotAI* botAI) { return new UseHealingPotionAction(botAI); }
    static Action* mana_potion(PlayerbotAI* botAI) { return new UseManaPotionAction(botAI); }
    static Action* healthstone(PlayerbotAI* botAI) { return new UseHealthstoneAction(botAI); }
    static Action* sell_junk(PlayerbotAI* botAI) { return new SellJunkAction(botAI); }
    static Action* check_mount_state(PlayerbotAI* botAI) { return new CheckMountStateAction(botAI); }
    static Action* dismount(PlayerbotAI* botAI) { return new DismountAction(botAI); }

    static Action* tank_face(PlayerbotAI* botAI) { return new TankFaceAction(botAI); }
    static Action* tank_assist(PlayerbotAI* botAI) { return new TankAssistAction(botAI); }
    static Action* set_facing(PlayerbotAI* botAI) { return new SetFacingTargetAction(botAI); }
    static Action* drop_target(PlayerbotAI* botAI) { return new DropTargetAction(botAI); }
    static Action* dps_assist(PlayerbotAI* botAI) { return new DpsAssistAction(botAI); }
    static Action* dps_aoe(PlayerbotAI* botAI) { return new DpsAoeAction(botAI); }
    static Action* melee(PlayerbotAI* botAI) { return new MeleeAction(botAI); }
    static Action* attack_anything(PlayerbotAI* botAI) { return new AttackAnythingAction(botAI); }

    static Action* toggle_pet_spell(PlayerbotAI* ai) { return new TogglePetSpellAutoCastAction(ai); }
    static Action* pet_attack(PlayerbotAI* ai) { return new PetAttackAction(ai); }

    static Action* ReachSpell(PlayerbotAI* botAI) { return new ReachSpellAction(botAI); }
    static Action* ReachMelee(PlayerbotAI* botAI) { return new ReachMeleeAction(botAI); }
    static Action* reach_party_member_to_heal(PlayerbotAI* botAI) { return new ReachPartyMemberToHealAction(botAI); }
    static Action* reach_party_member_to_resurrect(PlayerbotAI* botAI) { return new ReachPartyMemberToResurrectAction(botAI); }
};

#endif
