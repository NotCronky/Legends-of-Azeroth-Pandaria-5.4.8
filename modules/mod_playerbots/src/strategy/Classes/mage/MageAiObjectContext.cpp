/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MageAiObjectContext.h"

#include "GenericMageNonCombatStrategy.h"
#include "MageActions.h"
#include "MageStrategies.h"
#include "MageTriggers.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"

namespace
{
    class MageStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        MageStrategyFactoryInternal()
        {
            creators["nc"] = &MageStrategyFactoryInternal::nc;
            creators["cure"] = &MageStrategyFactoryInternal::cure;
        }

    private:
        static Strategy* nc(PlayerbotAI* botAI) { return new GenericMageNonCombatStrategy(botAI); }
        static Strategy* cure(PlayerbotAI* botAI) { return new MageCureStrategy(botAI); }
    };

    // One specialization at a time
    class MageCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
    {
    public:
        MageCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
        {
            creators["arcane"] = &MageCombatStrategyFactoryInternal::arcane;
            creators["fire"] = &MageCombatStrategyFactoryInternal::fire;
            creators["frost"] = &MageCombatStrategyFactoryInternal::frost;
        }

    private:
        static Strategy* arcane(PlayerbotAI* botAI) { return new ArcaneMageStrategy(botAI); }
        static Strategy* fire(PlayerbotAI* botAI) { return new FireMageStrategy(botAI); }
        static Strategy* frost(PlayerbotAI* botAI) { return new FrostMageStrategy(botAI); }
    };

    class MageTriggerFactoryInternal : public NamedObjectContext<Trigger>
    {
    public:
        MageTriggerFactoryInternal()
        {
            creators["arcane brilliance"] = &MageTriggerFactoryInternal::arcane_brilliance;
            creators["mage armor"] = &MageTriggerFactoryInternal::mage_armor;
            creators["molten armor"] = &MageTriggerFactoryInternal::molten_armor;
            creators["frost armor"] = &MageTriggerFactoryInternal::frost_armor;
            creators["arcane missiles!"] = &MageTriggerFactoryInternal::arcane_missiles_proc;
            creators["nether tempest"] = &MageTriggerFactoryInternal::nether_tempest;
            creators["arcane power"] = &MageTriggerFactoryInternal::arcane_power;
            creators["arcane charges"] = &MageTriggerFactoryInternal::arcane_charges;
            creators["pyroblast!"] = &MageTriggerFactoryInternal::pyroblast_proc;
            creators["heating up"] = &MageTriggerFactoryInternal::heating_up;
            creators["living bomb"] = &MageTriggerFactoryInternal::living_bomb;
            creators["combustion"] = &MageTriggerFactoryInternal::combustion;
            creators["fingers of frost"] = &MageTriggerFactoryInternal::fingers_of_frost;
            creators["brain freeze"] = &MageTriggerFactoryInternal::brain_freeze;
            creators["frost bomb"] = &MageTriggerFactoryInternal::frost_bomb;
            creators["frozen orb"] = &MageTriggerFactoryInternal::frozen_orb;
            creators["icy veins"] = &MageTriggerFactoryInternal::icy_veins;
            creators["water elemental"] = &MageTriggerFactoryInternal::water_elemental;
            creators["mirror image"] = &MageTriggerFactoryInternal::mirror_image;
            creators["counterspell"] = &MageTriggerFactoryInternal::counterspell;
            creators["counterspell on enemy healer"] = &MageTriggerFactoryInternal::counterspell_on_enemy_healer;
            creators["remove curse on party"] = &MageTriggerFactoryInternal::remove_curse_on_party;
        }

    private:
        static Trigger* arcane_brilliance(PlayerbotAI* botAI) { return new ArcaneBrillianceTrigger(botAI); }
        static Trigger* mage_armor(PlayerbotAI* botAI) { return new MageArmorTrigger(botAI, "mage armor", Specializations::SPEC_MAGE_ARCANE); }
        static Trigger* molten_armor(PlayerbotAI* botAI) { return new MageArmorTrigger(botAI, "molten armor", Specializations::SPEC_MAGE_FIRE); }
        static Trigger* frost_armor(PlayerbotAI* botAI) { return new MageArmorTrigger(botAI, "frost armor", Specializations::SPEC_MAGE_FROST); }
        static Trigger* arcane_missiles_proc(PlayerbotAI* botAI) { return new ArcaneMissilesProcTrigger(botAI); }
        static Trigger* nether_tempest(PlayerbotAI* botAI) { return new NetherTempestTrigger(botAI); }
        static Trigger* arcane_power(PlayerbotAI* botAI) { return new ArcanePowerTrigger(botAI); }
        static Trigger* arcane_charges(PlayerbotAI* botAI) { return new ArcaneChargesTrigger(botAI); }
        static Trigger* pyroblast_proc(PlayerbotAI* botAI) { return new PyroblastProcTrigger(botAI); }
        static Trigger* heating_up(PlayerbotAI* botAI) { return new HeatingUpTrigger(botAI); }
        static Trigger* living_bomb(PlayerbotAI* botAI) { return new LivingBombTrigger(botAI); }
        static Trigger* combustion(PlayerbotAI* botAI) { return new CombustionTrigger(botAI); }
        static Trigger* fingers_of_frost(PlayerbotAI* botAI) { return new FingersOfFrostTrigger(botAI); }
        static Trigger* brain_freeze(PlayerbotAI* botAI) { return new BrainFreezeTrigger(botAI); }
        static Trigger* frost_bomb(PlayerbotAI* botAI) { return new FrostBombTrigger(botAI); }
        static Trigger* frozen_orb(PlayerbotAI* botAI) { return new FrozenOrbTrigger(botAI); }
        static Trigger* icy_veins(PlayerbotAI* botAI) { return new IcyVeinsTrigger(botAI); }
        static Trigger* water_elemental(PlayerbotAI* botAI) { return new WaterElementalTrigger(botAI); }
        static Trigger* mirror_image(PlayerbotAI* botAI) { return new MirrorImageTrigger(botAI); }
        static Trigger* counterspell(PlayerbotAI* botAI) { return new CounterspellInterruptSpellTrigger(botAI); }
        static Trigger* counterspell_on_enemy_healer(PlayerbotAI* botAI) { return new CounterspellInterruptEnemyHealerSpellTrigger(botAI); }
        static Trigger* remove_curse_on_party(PlayerbotAI* botAI) { return new RemoveCurseOnPartyTrigger(botAI); }
    };

    class MageActionFactoryInternal : public NamedObjectContext<Action>
    {
    public:
        MageActionFactoryInternal()
        {
            creators["arcane brilliance"] = &MageActionFactoryInternal::arcane_brilliance;
            creators["mage armor"] = &MageActionFactoryInternal::mage_armor;
            creators["molten armor"] = &MageActionFactoryInternal::molten_armor;
            creators["frost armor"] = &MageActionFactoryInternal::frost_armor;

            creators["arcane blast"] = &MageActionFactoryInternal::arcane_blast;
            creators["arcane missiles"] = &MageActionFactoryInternal::arcane_missiles;
            creators["arcane barrage"] = &MageActionFactoryInternal::arcane_barrage;
            creators["nether tempest"] = &MageActionFactoryInternal::nether_tempest;
            creators["arcane power"] = &MageActionFactoryInternal::arcane_power;
            creators["arcane explosion"] = &MageActionFactoryInternal::arcane_explosion;

            creators["fireball"] = &MageActionFactoryInternal::fireball;
            creators["pyroblast"] = &MageActionFactoryInternal::pyroblast;
            creators["inferno blast"] = &MageActionFactoryInternal::inferno_blast;
            creators["living bomb"] = &MageActionFactoryInternal::living_bomb;
            creators["combustion"] = &MageActionFactoryInternal::combustion;
            creators["scorch"] = &MageActionFactoryInternal::scorch;
            creators["flamestrike"] = &MageActionFactoryInternal::flamestrike;
            creators["dragon's breath"] = &MageActionFactoryInternal::dragons_breath;

            creators["frostbolt"] = &MageActionFactoryInternal::frostbolt;
            creators["ice lance"] = &MageActionFactoryInternal::ice_lance;
            creators["frostfire bolt"] = &MageActionFactoryInternal::frostfire_bolt;
            creators["frozen orb"] = &MageActionFactoryInternal::frozen_orb;
            creators["icy veins"] = &MageActionFactoryInternal::icy_veins;
            creators["frost bomb"] = &MageActionFactoryInternal::frost_bomb;
            creators["blizzard"] = &MageActionFactoryInternal::blizzard;
            creators["summon water elemental"] = &MageActionFactoryInternal::summon_water_elemental;

            creators["counterspell"] = &MageActionFactoryInternal::counterspell;
            creators["counterspell on enemy healer"] = &MageActionFactoryInternal::counterspell_on_enemy_healer;
            creators["ice block"] = &MageActionFactoryInternal::ice_block;
            creators["evocation"] = &MageActionFactoryInternal::evocation;
            creators["mirror image"] = &MageActionFactoryInternal::mirror_image;
            creators["frost nova"] = &MageActionFactoryInternal::frost_nova;
            creators["remove curse on party"] = &MageActionFactoryInternal::remove_curse_on_party;
        }

    private:
        static Action* arcane_brilliance(PlayerbotAI* botAI) { return new CastArcaneBrillianceAction(botAI); }
        static Action* mage_armor(PlayerbotAI* botAI) { return new CastMageArmorAction(botAI); }
        static Action* molten_armor(PlayerbotAI* botAI) { return new CastMoltenArmorAction(botAI); }
        static Action* frost_armor(PlayerbotAI* botAI) { return new CastFrostArmorAction(botAI); }

        static Action* arcane_blast(PlayerbotAI* botAI) { return new CastArcaneBlastAction(botAI); }
        static Action* arcane_missiles(PlayerbotAI* botAI) { return new CastArcaneMissilesAction(botAI); }
        static Action* arcane_barrage(PlayerbotAI* botAI) { return new CastArcaneBarrageAction(botAI); }
        static Action* nether_tempest(PlayerbotAI* botAI) { return new CastNetherTempestAction(botAI); }
        static Action* arcane_power(PlayerbotAI* botAI) { return new CastArcanePowerAction(botAI); }
        static Action* arcane_explosion(PlayerbotAI* botAI) { return new CastArcaneExplosionAction(botAI); }

        static Action* fireball(PlayerbotAI* botAI) { return new CastFireballAction(botAI); }
        static Action* pyroblast(PlayerbotAI* botAI) { return new CastPyroblastAction(botAI); }
        static Action* inferno_blast(PlayerbotAI* botAI) { return new CastInfernoBlastAction(botAI); }
        static Action* living_bomb(PlayerbotAI* botAI) { return new CastLivingBombAction(botAI); }
        static Action* combustion(PlayerbotAI* botAI) { return new CastCombustionAction(botAI); }
        static Action* scorch(PlayerbotAI* botAI) { return new CastScorchAction(botAI); }
        static Action* flamestrike(PlayerbotAI* botAI) { return new CastFlamestrikeAction(botAI); }
        static Action* dragons_breath(PlayerbotAI* botAI) { return new CastDragonsBreathAction(botAI); }

        static Action* frostbolt(PlayerbotAI* botAI) { return new CastFrostboltAction(botAI); }
        static Action* ice_lance(PlayerbotAI* botAI) { return new CastIceLanceAction(botAI); }
        static Action* frostfire_bolt(PlayerbotAI* botAI) { return new CastFrostfireBoltAction(botAI); }
        static Action* frozen_orb(PlayerbotAI* botAI) { return new CastFrozenOrbAction(botAI); }
        static Action* icy_veins(PlayerbotAI* botAI) { return new CastIcyVeinsAction(botAI); }
        static Action* frost_bomb(PlayerbotAI* botAI) { return new CastFrostBombAction(botAI); }
        static Action* blizzard(PlayerbotAI* botAI) { return new CastBlizzardAction(botAI); }
        static Action* summon_water_elemental(PlayerbotAI* botAI) { return new CastSummonWaterElementalAction(botAI); }

        static Action* counterspell(PlayerbotAI* botAI) { return new CastCounterspellAction(botAI); }
        static Action* counterspell_on_enemy_healer(PlayerbotAI* botAI) { return new CastCounterspellOnEnemyHealerAction(botAI); }
        static Action* ice_block(PlayerbotAI* botAI) { return new CastIceBlockAction(botAI); }
        static Action* evocation(PlayerbotAI* botAI) { return new CastEvocationAction(botAI); }
        static Action* mirror_image(PlayerbotAI* botAI) { return new CastMirrorImageAction(botAI); }
        static Action* frost_nova(PlayerbotAI* botAI) { return new CastFrostNovaAction(botAI); }
        static Action* remove_curse_on_party(PlayerbotAI* botAI) { return new CastRemoveCurseOnPartyAction(botAI); }
    };
}

MageAiObjectContext::MageAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new MageStrategyFactoryInternal());
    strategyContexts.Add(new MageCombatStrategyFactoryInternal());
    actionContexts.Add(new MageActionFactoryInternal());
    triggerContexts.Add(new MageTriggerFactoryInternal());
}
