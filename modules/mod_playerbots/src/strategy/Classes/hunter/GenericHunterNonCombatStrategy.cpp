/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericHunterNonCombatStrategy.h"

#include "Playerbots.h"

namespace
{
    class GenericHunterNonCombatStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        GenericHunterNonCombatStrategyActionNodeFactory()
        {
            creators["aspect of the iron hawk"] = &aspect_of_the_iron_hawk;
        }

    private:
        ACTION_NODE_A(aspect_of_the_iron_hawk, "aspect of the iron hawk", "aspect of the hawk");
    };
}

GenericHunterNonCombatStrategy::GenericHunterNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericHunterNonCombatStrategyActionNodeFactory());
}

void GenericHunterNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("no aspect", NextAction::array(0, new NextAction("aspect of the iron hawk", 2.0f), nullptr)));
    triggers.push_back(new TriggerNode("auto shot dead target", NextAction::array(0, new NextAction("stop auto shot", 50.0f), nullptr)));
}

void HunterPetStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("hunter no pet", NextAction::array(0, new NextAction("call pet 1", 60.0f), nullptr)));
    triggers.push_back(new TriggerNode("hunter pet dead", NextAction::array(0, new NextAction("revive pet", 60.0f), nullptr)));
    triggers.push_back(new TriggerNode("hunter pet low health", NextAction::array(0, new NextAction("mend pet", 60.0f), nullptr)));
    triggers.push_back(new TriggerNode("has pet", NextAction::array(0, new NextAction("toggle pet spell", 60.0f), nullptr)));
}
