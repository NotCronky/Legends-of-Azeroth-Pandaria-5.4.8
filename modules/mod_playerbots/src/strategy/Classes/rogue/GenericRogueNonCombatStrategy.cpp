/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "GenericRogueNonCombatStrategy.h"

#include "Playerbots.h"

namespace
{
    class GenericRogueNonCombatStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        GenericRogueNonCombatStrategyActionNodeFactory()
        {
            creators["deadly poison"] = &deadly_poison;
            creators["crippling poison"] = &crippling_poison;
        }

    private:
        ACTION_NODE_A(deadly_poison, "deadly poison", "wound poison");
        ACTION_NODE_A(crippling_poison, "crippling poison", "mind-numbing poison");
    };
}

GenericRogueNonCombatStrategy::GenericRogueNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericRogueNonCombatStrategyActionNodeFactory());
}

void GenericRogueNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("player has flag", NextAction::array(0, new NextAction("sprint", ACTION_EMERGENCY + 1), nullptr)));
    triggers.push_back(new TriggerNode("enemy flagcarrier near", NextAction::array(0, new NextAction("sprint", ACTION_EMERGENCY + 2), nullptr)));

    // MoP poisons are self buffs that last an hour
    triggers.push_back(new TriggerNode("lethal poison", NextAction::array(0, new NextAction("deadly poison", 20.0f), nullptr)));
    triggers.push_back(new TriggerNode("non-lethal poison", NextAction::array(0, new NextAction("crippling poison", 19.0f), nullptr)));

    // Stealth (after Vanish) slows the bot down out of combat
    triggers.push_back(new TriggerNode("often", NextAction::array(0, new NextAction("unstealth", 30.0f), nullptr)));
}
