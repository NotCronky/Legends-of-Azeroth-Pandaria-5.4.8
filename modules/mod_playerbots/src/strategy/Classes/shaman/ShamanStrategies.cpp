/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "ShamanStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class ShamanActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        ShamanActionNodeFactory()
        {
            creators["lava burst"] = &lava_burst;
            creators["stormstrike"] = &stormstrike;
            creators["greater healing wave on party"] = &greater_healing_wave_on_party;
            creators["astral shift"] = &astral_shift;
        }

    private:
        ACTION_NODE_A(lava_burst, "lava burst", "lightning bolt");
        // Primal Strike before level 26
        ACTION_NODE_A(stormstrike, "stormstrike", "primal strike");
        ACTION_NODE_A(greater_healing_wave_on_party, "greater healing wave on party", "healing wave on party");
        ACTION_NODE_A(astral_shift, "astral shift", "shamanistic rage");
    };

    // What every shaman does: interrupt and survive
    void AddShamanTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("wind shear", NextAction::array(0, new NextAction("wind shear", ACTION_INTERRUPT + 2), nullptr)));
        triggers.push_back(new TriggerNode("wind shear on enemy healer", NextAction::array(0, new NextAction("wind shear on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
        triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("astral shift", ACTION_HIGH + 8), nullptr)));
        triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("healing surge", ACTION_INTERRUPT), nullptr)));
    }
}

ElementalShamanStrategy::ElementalShamanStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new ShamanActionNodeFactory());
}

NextAction** ElementalShamanStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("lava burst", ACTION_NORMAL + 2),
        new NextAction("lightning bolt", ACTION_NORMAL),
        nullptr);
}

void ElementalShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddShamanTriggers(triggers);

    triggers.push_back(new TriggerNode("lightning shield", NextAction::array(0, new NextAction("lightning shield", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("flametongue weapon", NextAction::array(0, new NextAction("flametongue weapon", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("elemental mastery", NextAction::array(0, new NextAction("elemental mastery", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("ascendance", NextAction::array(0, new NextAction("ascendance", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("fire elemental totem", NextAction::array(0, new NextAction("fire elemental totem", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("no fire totem", NextAction::array(0, new NextAction("searing totem", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("flame shock", NextAction::array(0, new NextAction("flame shock", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("lava surge", NextAction::array(0, new NextAction("lava burst", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("fulmination", NextAction::array(0, new NextAction("earth shock", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("earthquake", ACTION_HIGH + 3),
        new NextAction("chain lightning", ACTION_HIGH + 1),
        nullptr)));
}

EnhancementShamanStrategy::EnhancementShamanStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new ShamanActionNodeFactory());
}

NextAction** EnhancementShamanStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("stormstrike", ACTION_NORMAL + 5),
        new NextAction("lava lash", ACTION_NORMAL + 4),
        new NextAction("unleash elements", ACTION_NORMAL + 3),
        new NextAction("earth shock", ACTION_NORMAL + 1),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void EnhancementShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddShamanTriggers(triggers);

    triggers.push_back(new TriggerNode("lightning shield", NextAction::array(0, new NextAction("lightning shield", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("windfury weapon", NextAction::array(0, new NextAction("windfury weapon", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("feral spirit", NextAction::array(0, new NextAction("feral spirit", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("ascendance", NextAction::array(0, new NextAction("ascendance", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("fire elemental totem", NextAction::array(0, new NextAction("fire elemental totem", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("no fire totem", NextAction::array(0, new NextAction("searing totem", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("maelstrom weapon", NextAction::array(0, new NextAction("lightning bolt", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("flame shock", NextAction::array(0, new NextAction("flame shock", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("magma totem", ACTION_HIGH + 4),
        new NextAction("fire nova", ACTION_HIGH + 3),
        nullptr)));
}

RestorationShamanStrategy::RestorationShamanStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new ShamanActionNodeFactory());
}

void RestorationShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddShamanTriggers(triggers);

    triggers.push_back(new TriggerNode("water shield", NextAction::array(0, new NextAction("water shield", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("earthliving weapon", NextAction::array(0, new NextAction("earthliving weapon", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_EMERGENCY + 3), nullptr)));
    triggers.push_back(new TriggerNode("earth shield on main tank", NextAction::array(0, new NextAction("earth shield on main tank", ACTION_CRITICAL_HEAL + 7), nullptr)));

    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("riptide on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("healing surge on party", ACTION_CRITICAL_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("riptide on party", ACTION_MEDIUM_HEAL + 6),
        new NextAction("greater healing wave on party", ACTION_MEDIUM_HEAL + 5),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("healing stream totem", ACTION_LIGHT_HEAL + 9),
        new NextAction("riptide on party", ACTION_LIGHT_HEAL + 8),
        new NextAction("healing wave on party", ACTION_LIGHT_HEAL + 7),
        nullptr)));
    triggers.push_back(new TriggerNode("low aoe heal", NextAction::array(0, new NextAction("healing tide totem", ACTION_CRITICAL_HEAL + 6), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0,
        new NextAction("healing rain on party", ACTION_MEDIUM_HEAL + 8),
        new NextAction("chain heal on party", ACTION_MEDIUM_HEAL + 7),
        nullptr)));
    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("mana tide totem", ACTION_HIGH + 5), nullptr)));
}

void ShamanCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("purify spirit magic on party", NextAction::array(0, new NextAction("purify spirit magic on party", ACTION_DISPEL + 2), nullptr)));
    triggers.push_back(new TriggerNode("purify spirit curse on party", NextAction::array(0, new NextAction("purify spirit curse on party", ACTION_DISPEL + 1), nullptr)));
    triggers.push_back(new TriggerNode("cleanse spirit on party", NextAction::array(0, new NextAction("cleanse spirit on party", ACTION_DISPEL), nullptr)));
}
