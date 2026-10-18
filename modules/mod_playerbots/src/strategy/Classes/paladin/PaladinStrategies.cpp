/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PaladinStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class PaladinActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        PaladinActionNodeFactory()
        {
            creators["seal of truth"] = &seal_of_truth;
            creators["seal of insight"] = &seal_of_insight;
            creators["blessing of kings"] = &blessing_of_kings;
            creators["eternal flame"] = &eternal_flame;
            creators["eternal flame on party"] = &eternal_flame_on_party;
            creators["divine light on party"] = &divine_light_on_party;
            creators["divine shield"] = &divine_shield;
        }

    private:
        ACTION_NODE_A(seal_of_truth, "seal of truth", "seal of righteousness");
        ACTION_NODE_A(seal_of_insight, "seal of insight", "seal of righteousness");
        ACTION_NODE_A(blessing_of_kings, "blessing of kings", "blessing of might");
        // Eternal Flame (level 45 talent) replaces Word of Glory
        ACTION_NODE_A(eternal_flame, "eternal flame", "word of glory");
        ACTION_NODE_A(eternal_flame_on_party, "eternal flame on party", "word of glory on party");
        ACTION_NODE_A(divine_light_on_party, "divine light on party", "holy light on party");
        ACTION_NODE_A(divine_shield, "divine shield", "lay on hands");
    };

    // What every paladin does: keep a blessing up, interrupt, and survive
    void AddPaladinTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("blessing of kings", NextAction::array(0, new NextAction("blessing of kings", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("blessing of might", NextAction::array(0, new NextAction("blessing of might", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("rebuke", NextAction::array(0, new NextAction("rebuke", ACTION_INTERRUPT + 2), nullptr)));
        triggers.push_back(new TriggerNode("rebuke on enemy healer", NextAction::array(0, new NextAction("rebuke on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
        triggers.push_back(new TriggerNode("sacred shield", NextAction::array(0, new NextAction("sacred shield", ACTION_HIGH + 5), nullptr)));
    }
}

RetributionPaladinStrategy::RetributionPaladinStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PaladinActionNodeFactory());
}

NextAction** RetributionPaladinStrategy::getDefaultActions()
{
    // Templar's Verdict needs three holy power and Hammer of Wrath a target below 20%: until then the builders run
    return NextAction::array(0,
        new NextAction("templar's verdict", ACTION_NORMAL + 5),
        new NextAction("hammer of wrath", ACTION_NORMAL + 4),
        new NextAction("exorcism", ACTION_NORMAL + 3),
        new NextAction("crusader strike", ACTION_NORMAL + 2),
        new NextAction("judgment", ACTION_NORMAL + 1),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void RetributionPaladinStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddPaladinTriggers(triggers);

    triggers.push_back(new TriggerNode("seal of truth", NextAction::array(0, new NextAction("seal of truth", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("avenging wrath", NextAction::array(0, new NextAction("avenging wrath", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("guardian of ancient kings", NextAction::array(0, new NextAction("guardian of ancient kings", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("inquisition", NextAction::array(0, new NextAction("inquisition", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("execution sentence", NextAction::array(0, new NextAction("execution sentence", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("divine purpose", NextAction::array(0, new NextAction("templar's verdict", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("divine storm", ACTION_HIGH + 2),
        new NextAction("hammer of the righteous", ACTION_HIGH + 1),
        nullptr)));

    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("eternal flame", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("divine shield", ACTION_INTERRUPT), nullptr)));
}

ProtectionPaladinStrategy::ProtectionPaladinStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PaladinActionNodeFactory());
}

NextAction** ProtectionPaladinStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("shield of the righteous", ACTION_NORMAL + 6),
        new NextAction("avenger's shield", ACTION_NORMAL + 5),
        new NextAction("crusader strike", ACTION_NORMAL + 4),
        new NextAction("judgment", ACTION_NORMAL + 3),
        new NextAction("hammer of wrath", ACTION_NORMAL + 2),
        new NextAction("consecration", ACTION_NORMAL + 1),
        new NextAction("holy wrath", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void ProtectionPaladinStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddPaladinTriggers(triggers);

    triggers.push_back(new TriggerNode("righteous fury", NextAction::array(0, new NextAction("righteous fury", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("lose aggro", NextAction::array(0, new NextAction("reckoning", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("seal of insight", NextAction::array(0, new NextAction("seal of insight", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("grand crusader", NextAction::array(0, new NextAction("avenger's shield", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("execution sentence", NextAction::array(0, new NextAction("execution sentence", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("hammer of the righteous", ACTION_HIGH + 2),
        new NextAction("consecration", ACTION_HIGH + 1),
        nullptr)));

    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("divine protection", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0,
        new NextAction("eternal flame", ACTION_HIGH + 8),
        new NextAction("guardian of ancient kings", ACTION_HIGH + 7),
        nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0,
        new NextAction("ardent defender", ACTION_INTERRUPT + 1),
        new NextAction("lay on hands", ACTION_INTERRUPT),
        nullptr)));
}

HolyPaladinStrategy::HolyPaladinStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PaladinActionNodeFactory());
}

void HolyPaladinStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddPaladinTriggers(triggers);

    triggers.push_back(new TriggerNode("seal of insight", NextAction::array(0, new NextAction("seal of insight", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_EMERGENCY + 3), nullptr)));
    triggers.push_back(new TriggerNode("beacon of light on main tank", NextAction::array(0, new NextAction("beacon of light on main tank", ACTION_CRITICAL_HEAL + 7), nullptr)));

    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("lay on hands on party", ACTION_CRITICAL_HEAL + 6),
        new NextAction("holy shock on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("eternal flame on party", ACTION_CRITICAL_HEAL + 4),
        new NextAction("flash of light on party", ACTION_CRITICAL_HEAL + 3),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("holy shock on party", ACTION_MEDIUM_HEAL + 6),
        new NextAction("eternal flame on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("divine light on party", ACTION_MEDIUM_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("holy shock on party", ACTION_LIGHT_HEAL + 9),
        new NextAction("holy light on party", ACTION_LIGHT_HEAL + 8),
        nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0,
        new NextAction("light of dawn", ACTION_MEDIUM_HEAL + 8),
        new NextAction("holy prism", ACTION_MEDIUM_HEAL + 7),
        new NextAction("holy radiance on party", ACTION_MEDIUM_HEAL + 7),
        nullptr)));

    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("divine plea", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("divine shield", ACTION_INTERRUPT), nullptr)));
}

void PaladinCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("cleanse poison on party", NextAction::array(0, new NextAction("cleanse poison on party", ACTION_DISPEL + 2), nullptr)));
    triggers.push_back(new TriggerNode("cleanse disease on party", NextAction::array(0, new NextAction("cleanse disease on party", ACTION_DISPEL + 1), nullptr)));
    triggers.push_back(new TriggerNode("cleanse magic on party", NextAction::array(0, new NextAction("cleanse magic on party", ACTION_DISPEL), nullptr)));
}
