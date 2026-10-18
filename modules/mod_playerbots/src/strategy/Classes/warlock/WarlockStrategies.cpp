/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "WarlockStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class WarlockActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        WarlockActionNodeFactory()
        {
            creators["malefic grasp"] = &malefic_grasp;
            creators["incinerate"] = &incinerate;
            creators["unending resolve"] = &unending_resolve;
        }

    private:
        ACTION_NODE_A(malefic_grasp, "malefic grasp", "shadow bolt");
        ACTION_NODE_A(incinerate, "incinerate", "shadow bolt");
        ACTION_NODE_A(unending_resolve, "unending resolve", "mortal coil");
    };

    // What every warlock does: a demon, Dark Intent, Life Tap for mana, Drain Life and Unending Resolve when hurt
    void AddWarlockTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("no pet", NextAction::array(0, new NextAction("summon demon", ACTION_HIGH + 9), nullptr)));
        triggers.push_back(new TriggerNode("dark intent", NextAction::array(0, new NextAction("dark intent", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("life tap", ACTION_HIGH + 6), nullptr)));
        triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("drain life", ACTION_HIGH + 7), nullptr)));
        triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("unending resolve", ACTION_INTERRUPT), nullptr)));
    }
}

AfflictionWarlockStrategy::AfflictionWarlockStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new WarlockActionNodeFactory());
}

NextAction** AfflictionWarlockStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("malefic grasp", ACTION_NORMAL), nullptr);
}

void AfflictionWarlockStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddWarlockTriggers(triggers);

    triggers.push_back(new TriggerNode("dark soul: misery", NextAction::array(0, new NextAction("dark soul: misery", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("agony", NextAction::array(0, new NextAction("agony", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("corruption", NextAction::array(0, new NextAction("corruption", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("unstable affliction", NextAction::array(0, new NextAction("unstable affliction", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("haunt", NextAction::array(0, new NextAction("haunt", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("drain soul", NextAction::array(0, new NextAction("drain soul", ACTION_NORMAL + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("seed of corruption", ACTION_HIGH + 3), nullptr)));
}

DemonologyWarlockStrategy::DemonologyWarlockStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new WarlockActionNodeFactory());
}

NextAction** DemonologyWarlockStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("shadow bolt", ACTION_NORMAL), nullptr);
}

void DemonologyWarlockStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddWarlockTriggers(triggers);

    triggers.push_back(new TriggerNode("dark soul: knowledge", NextAction::array(0, new NextAction("dark soul: knowledge", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("corruption", NextAction::array(0, new NextAction("corruption", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("shadowflame", NextAction::array(0, new NextAction("hand of gul'dan", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("molten core", NextAction::array(0, new NextAction("soul fire", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("seed of corruption", ACTION_HIGH + 3), nullptr)));
}

DestructionWarlockStrategy::DestructionWarlockStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new WarlockActionNodeFactory());
}

NextAction** DestructionWarlockStrategy::getDefaultActions()
{
    // Shadowburn needs a target below 20% and an ember
    return NextAction::array(0,
        new NextAction("shadowburn", ACTION_NORMAL + 2),
        new NextAction("incinerate", ACTION_NORMAL),
        nullptr);
}

void DestructionWarlockStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddWarlockTriggers(triggers);

    triggers.push_back(new TriggerNode("dark soul: instability", NextAction::array(0, new NextAction("dark soul: instability", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("immolate", NextAction::array(0, new NextAction("immolate", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("conflagrate", NextAction::array(0, new NextAction("conflagrate", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("burning embers", NextAction::array(0, new NextAction("chaos bolt", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("rain of fire", ACTION_HIGH + 3), nullptr)));
}
