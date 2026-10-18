/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MonkStrategies.h"

#include "Playerbots.h"

namespace
{
    // What every monk does: Legacy of the Emperor, Chi Wave, Touch of Death, interrupt and survive
    void AddMonkTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("legacy of the emperor", NextAction::array(0, new NextAction("legacy of the emperor", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("touch of death", NextAction::array(0, new NextAction("touch of death", ACTION_HIGH + 8), nullptr)));
        triggers.push_back(new TriggerNode("chi wave", NextAction::array(0, new NextAction("chi wave", ACTION_HIGH + 1), nullptr)));
        triggers.push_back(new TriggerNode("spear hand strike", NextAction::array(0, new NextAction("spear hand strike", ACTION_INTERRUPT + 2), nullptr)));
        triggers.push_back(new TriggerNode("spear hand strike on enemy healer", NextAction::array(0, new NextAction("spear hand strike on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
        triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("fortifying brew", ACTION_INTERRUPT), nullptr)));
    }
}

BrewmasterMonkStrategy::BrewmasterMonkStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI) {}

NextAction** BrewmasterMonkStrategy::getDefaultActions()
{
    // Chi spenders first; they are only possible with the chi for them
    return NextAction::array(0,
        new NextAction("keg smash", ACTION_NORMAL + 4),
        new NextAction("blackout kick", ACTION_NORMAL + 3),
        new NextAction("tiger palm", ACTION_NORMAL + 2),
        new NextAction("expel harm", ACTION_NORMAL + 1),
        new NextAction("jab", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void BrewmasterMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddMonkTriggers(triggers);

    triggers.push_back(new TriggerNode("stance of the sturdy ox", NextAction::array(0, new NextAction("stance of the sturdy ox", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("lose aggro", NextAction::array(0, new NextAction("provoke", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("heavy stagger", NextAction::array(0, new NextAction("purifying brew", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("shuffle", NextAction::array(0, new NextAction("blackout kick", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("guard", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("tiger power", NextAction::array(0, new NextAction("tiger palm", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("breath of fire", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("fortifying brew", ACTION_HIGH + 8), nullptr)));
}

WindwalkerMonkStrategy::WindwalkerMonkStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI) {}

NextAction** WindwalkerMonkStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("rising sun kick", ACTION_NORMAL + 4),
        new NextAction("fists of fury", ACTION_NORMAL + 3),
        new NextAction("blackout kick", ACTION_NORMAL + 2),
        new NextAction("expel harm", ACTION_NORMAL + 1),
        new NextAction("jab", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void WindwalkerMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddMonkTriggers(triggers);

    triggers.push_back(new TriggerNode("stance of the fierce tiger", NextAction::array(0, new NextAction("stance of the fierce tiger", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("invoke xuen", NextAction::array(0, new NextAction("invoke xuen, the white tiger", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("energizing brew", NextAction::array(0, new NextAction("energizing brew", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("tiger power", NextAction::array(0, new NextAction("tiger palm", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("rising sun kick", NextAction::array(0, new NextAction("rising sun kick", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("combo breaker: blackout kick", NextAction::array(0, new NextAction("blackout kick", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("combo breaker: tiger palm", NextAction::array(0, new NextAction("tiger palm", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("spinning crane kick", ACTION_HIGH + 2), nullptr)));
}

MistweaverMonkStrategy::MistweaverMonkStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI) {}

NextAction** MistweaverMonkStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("tiger palm", ACTION_NORMAL + 1),
        new NextAction("jab", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void MistweaverMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);
    AddMonkTriggers(triggers);

    triggers.push_back(new TriggerNode("stance of the wise serpent", NextAction::array(0, new NextAction("stance of the wise serpent", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_EMERGENCY + 3), nullptr)));

    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("life cocoon on party", ACTION_CRITICAL_HEAL + 6),
        new NextAction("enveloping mist on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("surging mist on party", ACTION_CRITICAL_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("renewing mist on party", ACTION_MEDIUM_HEAL + 6),
        new NextAction("enveloping mist on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("surging mist on party", ACTION_MEDIUM_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0, new NextAction("renewing mist on party", ACTION_LIGHT_HEAL + 9), nullptr)));
    triggers.push_back(new TriggerNode("low aoe heal", NextAction::array(0, new NextAction("revival", ACTION_CRITICAL_HEAL + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0, new NextAction("uplift", ACTION_MEDIUM_HEAL + 7), nullptr)));
}

void MonkCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("detox magic on party", NextAction::array(0, new NextAction("detox magic on party", ACTION_DISPEL + 2), nullptr)));
    triggers.push_back(new TriggerNode("detox poison on party", NextAction::array(0, new NextAction("detox poison on party", ACTION_DISPEL + 1), nullptr)));
    triggers.push_back(new TriggerNode("detox disease on party", NextAction::array(0, new NextAction("detox disease on party", ACTION_DISPEL), nullptr)));
}
