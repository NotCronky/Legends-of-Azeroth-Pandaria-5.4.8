/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "RogueStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet, or not usable now, fall back to one that is
    class RogueActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        RogueActionNodeFactory()
        {
            creators["ambush"] = &ambush;
            creators["garrote"] = &garrote;
            creators["dispatch"] = &dispatch;
            creators["mutilate"] = &mutilate;
            creators["hemorrhage"] = &hemorrhage;
            creators["envenom"] = &envenom;
            creators["evasion"] = &evasion;
        }

    private:
        // Ambush from behind, else Garrote, else Cheap Shot
        ACTION_NODE_A(ambush, "ambush", "garrote");
        ACTION_NODE_A(garrote, "garrote", "cheap shot");
        // Dispatch below 35% health or on Blindside, else Mutilate (level 10 Assassination)
        ACTION_NODE_A(dispatch, "dispatch", "mutilate");
        ACTION_NODE_A(mutilate, "mutilate", "sinister strike");
        ACTION_NODE_A(hemorrhage, "hemorrhage", "sinister strike");
        ACTION_NODE_A(envenom, "envenom", "eviscerate");
        ACTION_NODE_A(evasion, "evasion", "cloak of shadows");
    };
}

GenericRogueStrategy::GenericRogueStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new RogueActionNodeFactory());
}

void GenericRogueStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("stealthed", NextAction::array(0,
        new NextAction("premeditation", ACTION_HIGH + 9),
        new NextAction("ambush", ACTION_HIGH + 8),
        nullptr)));
    triggers.push_back(new TriggerNode("slice and dice", NextAction::array(0, new NextAction("slice and dice", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("combo points target dying", NextAction::array(0, new NextAction("eviscerate", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("finisher", NextAction::array(0, new NextAction("eviscerate", ACTION_HIGH + 3), nullptr)));

    triggers.push_back(new TriggerNode("kick", NextAction::array(0, new NextAction("kick", ACTION_INTERRUPT + 2), nullptr)));
    triggers.push_back(new TriggerNode("kick on enemy healer", NextAction::array(0, new NextAction("kick on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("evasion", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("feint", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("tricks of the trade on main tank", NextAction::array(0, new NextAction("tricks of the trade on main tank", ACTION_HIGH + 2), nullptr)));
}

NextAction** AssassinationRogueStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("dispatch", ACTION_NORMAL), new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void AssassinationRogueStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("vendetta", NextAction::array(0, new NextAction("vendetta", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("rupture", NextAction::array(0, new NextAction("rupture", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("finisher", NextAction::array(0, new NextAction("envenom", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("blindside", NextAction::array(0, new NextAction("dispatch", ACTION_HIGH + 2), nullptr)));
}

NextAction** CombatRogueStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("sinister strike", ACTION_NORMAL), new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void CombatRogueStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("adrenaline rush", NextAction::array(0, new NextAction("adrenaline rush", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("killing spree", NextAction::array(0, new NextAction("killing spree", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("revealing strike", NextAction::array(0, new NextAction("revealing strike", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("light aoe", NextAction::array(0, new NextAction("blade flurry", ACTION_HIGH + 8), nullptr)));
}

NextAction** SubtletyRogueStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("hemorrhage", ACTION_NORMAL), new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void SubtletyRogueStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericRogueStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("shadow dance", NextAction::array(0, new NextAction("shadow dance", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("rupture", NextAction::array(0, new NextAction("rupture", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("hemorrhage", NextAction::array(0, new NextAction("hemorrhage", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("behind target", NextAction::array(0, new NextAction("backstab", ACTION_NORMAL + 1), nullptr)));
}

void RogueBoostStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("shadow blades", NextAction::array(0, new NextAction("shadow blades", ACTION_HIGH + 7), nullptr)));
}

void RogueAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("fan of knives", ACTION_HIGH + 1), nullptr)));
}
