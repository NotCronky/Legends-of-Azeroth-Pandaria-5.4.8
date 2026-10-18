/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "PriestStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class PriestActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        PriestActionNodeFactory()
        {
            creators["mindbender"] = &mindbender;
            creators["greater heal on party"] = &greater_heal_on_party;
            creators["penance on party"] = &penance_on_party;
        }

    private:
        // Mindbender (level 45 talent) replaces Shadowfiend
        ACTION_NODE_A(mindbender, "mindbender", "shadowfiend");
        ACTION_NODE_A(greater_heal_on_party, "greater heal on party", "heal on party");
        ACTION_NODE_A(penance_on_party, "penance on party", "flash heal on party");
    };

    // What every priest does: Inner Fire, Fade from aggro, Power Word: Shield on itself when hurt
    void AddPriestTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("power word: fortitude", NextAction::array(0, new NextAction("power word: fortitude", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("inner fire", NextAction::array(0, new NextAction("inner fire", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("has aggro", NextAction::array(0, new NextAction("fade", ACTION_HIGH + 6), nullptr)));
        triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("power word: shield", ACTION_HIGH + 8), nullptr)));
        triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("flash heal", ACTION_INTERRUPT), nullptr)));
    }

    // Healers: move to the one to heal, Shadowfiend and Hymn of Hope for mana
    void AddPriestHealerTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_EMERGENCY + 3), nullptr)));
        triggers.push_back(new TriggerNode("low mana", NextAction::array(0,
            new NextAction("mindbender", ACTION_HIGH + 5),
            new NextAction("hymn of hope", ACTION_HIGH + 4),
            nullptr)));
    }
}

ShadowPriestStrategy::ShadowPriestStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PriestActionNodeFactory());
}

NextAction** ShadowPriestStrategy::getDefaultActions()
{
    // Devouring Plague needs three Shadow Orbs and Shadow Word: Death a target below 20%
    return NextAction::array(0,
        new NextAction("devouring plague", ACTION_NORMAL + 4),
        new NextAction("mind blast", ACTION_NORMAL + 3),
        new NextAction("shadow word: death", ACTION_NORMAL + 2),
        new NextAction("mind flay", ACTION_NORMAL),
        nullptr);
}

void ShadowPriestStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddPriestTriggers(triggers);

    triggers.push_back(new TriggerNode("shadowform", NextAction::array(0, new NextAction("shadowform", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("shadowfiend", NextAction::array(0, new NextAction("mindbender", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("vampiric touch", NextAction::array(0, new NextAction("vampiric touch", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("shadow word: pain", NextAction::array(0, new NextAction("shadow word: pain", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("surge of darkness", NextAction::array(0, new NextAction("mind spike", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("halo", NextAction::array(0, new NextAction("halo", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("mind sear", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("dispersion", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("silence", NextAction::array(0, new NextAction("silence", ACTION_INTERRUPT + 2), nullptr)));
    triggers.push_back(new TriggerNode("silence on enemy healer", NextAction::array(0, new NextAction("silence on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
}

DisciplinePriestStrategy::DisciplinePriestStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PriestActionNodeFactory());
}

void DisciplinePriestStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddPriestTriggers(triggers);
    AddPriestHealerTriggers(triggers);

    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("pain suppression on party", ACTION_CRITICAL_HEAL + 7),
        new NextAction("power word: shield on party", ACTION_CRITICAL_HEAL + 6),
        new NextAction("penance on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("flash heal on party", ACTION_CRITICAL_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("power word: shield on party", ACTION_MEDIUM_HEAL + 6),
        new NextAction("penance on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("greater heal on party", ACTION_MEDIUM_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("prayer of mending on party", ACTION_LIGHT_HEAL + 9),
        new NextAction("heal on party", ACTION_LIGHT_HEAL + 8),
        nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0,
        new NextAction("halo", ACTION_MEDIUM_HEAL + 8),
        new NextAction("prayer of healing on party", ACTION_MEDIUM_HEAL + 7),
        nullptr)));
}

HolyPriestStrategy::HolyPriestStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PriestActionNodeFactory());
}

void HolyPriestStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddPriestTriggers(triggers);
    AddPriestHealerTriggers(triggers);

    triggers.push_back(new TriggerNode("chakra: serenity", NextAction::array(0, new NextAction("chakra: serenity", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("guardian spirit on party", ACTION_CRITICAL_HEAL + 7),
        new NextAction("flash heal on party", ACTION_CRITICAL_HEAL + 5),
        nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("prayer of mending on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("greater heal on party", ACTION_MEDIUM_HEAL + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("renew on party", ACTION_LIGHT_HEAL + 9),
        new NextAction("heal on party", ACTION_LIGHT_HEAL + 8),
        nullptr)));
    triggers.push_back(new TriggerNode("low aoe heal", NextAction::array(0, new NextAction("divine hymn", ACTION_CRITICAL_HEAL + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe heal", NextAction::array(0,
        new NextAction("circle of healing on party", ACTION_MEDIUM_HEAL + 8),
        new NextAction("halo", ACTION_MEDIUM_HEAL + 7),
        new NextAction("prayer of healing on party", ACTION_MEDIUM_HEAL + 6),
        nullptr)));
}

HolyDpsPriestStrategy::HolyDpsPriestStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PriestActionNodeFactory());
}

NextAction** HolyDpsPriestStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("penance", ACTION_NORMAL + 3),
        new NextAction("holy fire", ACTION_NORMAL + 2),
        new NextAction("mind blast", ACTION_NORMAL + 1),
        new NextAction("smite", ACTION_NORMAL),
        nullptr);
}

void HolyDpsPriestStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddPriestTriggers(triggers);

    triggers.push_back(new TriggerNode("shadow word: pain", NextAction::array(0, new NextAction("shadow word: pain", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("flash heal", ACTION_HIGH + 7), nullptr)));
}

void PriestCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("purify magic on party", NextAction::array(0, new NextAction("purify magic on party", ACTION_DISPEL + 1), nullptr)));
    triggers.push_back(new TriggerNode("purify disease on party", NextAction::array(0, new NextAction("purify disease on party", ACTION_DISPEL), nullptr)));
}
