/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DruidStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class DruidActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        DruidActionNodeFactory()
        {
            creators["survival instincts"] = &survival_instincts;
            creators["swiftmend on party"] = &swiftmend_on_party;
            creators["nourish on party"] = &nourish_on_party;
        }

    private:
        ACTION_NODE_A(survival_instincts, "survival instincts", "barkskin");
        ACTION_NODE_A(swiftmend_on_party, "swiftmend on party", "regrowth on party");
        ACTION_NODE_A(nourish_on_party, "nourish on party", "healing touch on party");
    };

    // What every druid does: Mark of the Wild and Barkskin
    void AddDruidTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("mark of the wild", NextAction::array(0, new NextAction("mark of the wild", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("barkskin", ACTION_HIGH + 8), nullptr)));
    }
}

BalanceDruidStrategy::BalanceDruidStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new DruidActionNodeFactory());
}

NextAction** BalanceDruidStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("starsurge", ACTION_NORMAL + 2),
        new NextAction("wrath", ACTION_NORMAL),
        nullptr);
}

void BalanceDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddDruidTriggers(triggers);

    triggers.push_back(new TriggerNode("in animal form", NextAction::array(0, new NextAction("caster form", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("moonkin form", NextAction::array(0, new NextAction("moonkin form", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("celestial alignment", NextAction::array(0, new NextAction("celestial alignment", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("starfall", NextAction::array(0, new NextAction("starfall", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("moonfire", NextAction::array(0, new NextAction("moonfire", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("sunfire", NextAction::array(0, new NextAction("sunfire", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("shooting stars", NextAction::array(0, new NextAction("starsurge", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("starfire phase", NextAction::array(0, new NextAction("starfire", ACTION_NORMAL + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("hurricane", ACTION_HIGH + 2), nullptr)));
}

FeralDruidStrategy::FeralDruidStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new DruidActionNodeFactory());
}

NextAction** FeralDruidStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("mangle", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void FeralDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddDruidTriggers(triggers);

    triggers.push_back(new TriggerNode("cat form", NextAction::array(0, new NextAction("cat form", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("berserk", NextAction::array(0, new NextAction("berserk", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("tiger's fury", NextAction::array(0, new NextAction("tiger's fury", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("savage roar", NextAction::array(0, new NextAction("savage roar", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("rip", NextAction::array(0, new NextAction("rip", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("druid finisher", NextAction::array(0, new NextAction("ferocious bite", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("rake", NextAction::array(0, new NextAction("rake", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("faerie fire", NextAction::array(0, new NextAction("faerie fire", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("behind target", NextAction::array(0, new NextAction("shred", ACTION_NORMAL + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("thrash", ACTION_HIGH + 2),
        new NextAction("swipe", ACTION_HIGH + 1),
        nullptr)));

    triggers.push_back(new TriggerNode("skull bash", NextAction::array(0, new NextAction("skull bash", ACTION_INTERRUPT + 2), nullptr)));
    triggers.push_back(new TriggerNode("skull bash on enemy healer", NextAction::array(0, new NextAction("skull bash on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("survival instincts", ACTION_INTERRUPT), nullptr)));
}

GuardianDruidStrategy::GuardianDruidStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new DruidActionNodeFactory());
}

NextAction** GuardianDruidStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("mangle", ACTION_NORMAL + 3),
        new NextAction("lacerate", ACTION_NORMAL + 1),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void GuardianDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddDruidTriggers(triggers);

    triggers.push_back(new TriggerNode("bear form", NextAction::array(0, new NextAction("bear form", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("lose aggro", NextAction::array(0, new NextAction("growl", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("savage defense", NextAction::array(0, new NextAction("savage defense", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("thrash", NextAction::array(0, new NextAction("thrash", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("faerie fire", NextAction::array(0, new NextAction("faerie fire", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high rage", NextAction::array(0, new NextAction("maul", ACTION_HIGH + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("thrash", ACTION_HIGH + 3),
        new NextAction("swipe", ACTION_HIGH + 2),
        nullptr)));

    triggers.push_back(new TriggerNode("skull bash", NextAction::array(0, new NextAction("skull bash", ACTION_INTERRUPT + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("frenzied regeneration", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("might of ursoc", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("survival instincts", ACTION_INTERRUPT), nullptr)));
}

RestorationDruidStrategy::RestorationDruidStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new DruidActionNodeFactory());
}

void RestorationDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddDruidTriggers(triggers);

    triggers.push_back(new TriggerNode("in animal form", NextAction::array(0, new NextAction("caster form", ACTION_EMERGENCY + 4), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_EMERGENCY + 3), nullptr)));
    triggers.push_back(new TriggerNode("lifebloom on main tank", NextAction::array(0, new NextAction("lifebloom on main tank", ACTION_CRITICAL_HEAL + 7), nullptr)));

    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("ironbark on party", ACTION_CRITICAL_HEAL + 6),
        new NextAction("swiftmend on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("regrowth on party", ACTION_CRITICAL_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("rejuvenation on party", ACTION_MEDIUM_HEAL + 6),
        new NextAction("swiftmend on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("healing touch on party", ACTION_MEDIUM_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("rejuvenation on party", ACTION_LIGHT_HEAL + 9),
        new NextAction("nourish on party", ACTION_LIGHT_HEAL + 8),
        nullptr)));
    triggers.push_back(new TriggerNode("low aoe heal", NextAction::array(0, new NextAction("tranquility", ACTION_CRITICAL_HEAL + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0, new NextAction("wild growth on party", ACTION_MEDIUM_HEAL + 7), nullptr)));
    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("innervate", ACTION_HIGH + 5), nullptr)));
}

void DruidCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("nature's cure magic on party", NextAction::array(0, new NextAction("nature's cure magic on party", ACTION_DISPEL + 3), nullptr)));
    triggers.push_back(new TriggerNode("nature's cure curse on party", NextAction::array(0, new NextAction("nature's cure curse on party", ACTION_DISPEL + 2), nullptr)));
    triggers.push_back(new TriggerNode("nature's cure poison on party", NextAction::array(0, new NextAction("nature's cure poison on party", ACTION_DISPEL + 2), nullptr)));
    triggers.push_back(new TriggerNode("remove corruption curse on party", NextAction::array(0, new NextAction("remove corruption curse on party", ACTION_DISPEL + 1), nullptr)));
    triggers.push_back(new TriggerNode("remove corruption poison on party", NextAction::array(0, new NextAction("remove corruption poison on party", ACTION_DISPEL), nullptr)));
}
