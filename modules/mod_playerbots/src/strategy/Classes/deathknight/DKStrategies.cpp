/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "DKStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known, fall back to one that is
    class DKActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        DKActionNodeFactory()
        {
            creators["rune strike"] = &rune_strike;
            creators["icebound fortitude"] = &icebound_fortitude;
        }

    private:
        // Rune Strike left in 5.4 for Death Coil
        ACTION_NODE_A(rune_strike, "rune strike", "death coil");
        ACTION_NODE_A(icebound_fortitude, "icebound fortitude", "anti-magic shell");
    };
}

GenericDKStrategy::GenericDKStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new DKActionNodeFactory());
}

void GenericDKStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);

    // Outbreak puts up both; Icy Touch and Plague Strike only what is still missing
    triggers.push_back(new TriggerNode("frost fever", NextAction::array(0,
        new NextAction("outbreak", ACTION_HIGH + 5),
        new NextAction("icy touch", ACTION_HIGH + 4),
        nullptr)));
    triggers.push_back(new TriggerNode("blood plague", NextAction::array(0,
        new NextAction("outbreak", ACTION_HIGH + 5),
        new NextAction("plague strike", ACTION_HIGH + 4),
        nullptr)));

    triggers.push_back(new TriggerNode("soul reaper", NextAction::array(0, new NextAction("soul reaper", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("no runes", NextAction::array(0, new NextAction("empower rune weapon", ACTION_HIGH + 1), nullptr)));
    triggers.push_back(new TriggerNode("blood charge", NextAction::array(0, new NextAction("blood tap", ACTION_HIGH + 1), nullptr)));
    triggers.push_back(new TriggerNode("horn of winter", NextAction::array(0, new NextAction("horn of winter", ACTION_NORMAL + 3), nullptr)));

    triggers.push_back(new TriggerNode("mind freeze", NextAction::array(0, new NextAction("mind freeze", ACTION_INTERRUPT + 2), nullptr)));
    triggers.push_back(new TriggerNode("mind freeze on enemy healer", NextAction::array(0, new NextAction("mind freeze on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("icebound fortitude", ACTION_INTERRUPT), nullptr)));
}

NextAction** FrostDKStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("obliterate", ACTION_NORMAL + 2),
        new NextAction("howling blast", ACTION_NORMAL + 1),
        new NextAction("frost strike", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void FrostDKStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericDKStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("frost presence", NextAction::array(0, new NextAction("frost presence", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("pillar of frost", NextAction::array(0, new NextAction("pillar of frost", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("raise dead", NextAction::array(0, new NextAction("raise dead", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("killing machine", NextAction::array(0, new NextAction("obliterate", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("freezing fog", NextAction::array(0, new NextAction("howling blast", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high runic power", NextAction::array(0, new NextAction("frost strike", ACTION_HIGH + 1), nullptr)));
}

NextAction** UnholyDKStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("scourge strike", ACTION_NORMAL + 2),
        new NextAction("festering strike", ACTION_NORMAL + 1),
        new NextAction("death coil", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void UnholyDKStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericDKStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("unholy presence", NextAction::array(0, new NextAction("unholy presence", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("dk no ghoul", NextAction::array(0, new NextAction("raise dead", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("summon gargoyle", NextAction::array(0, new NextAction("summon gargoyle", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("unholy frenzy", NextAction::array(0, new NextAction("unholy frenzy", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("shadow infusion", NextAction::array(0, new NextAction("dark transformation", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("sudden doom", NextAction::array(0, new NextAction("death coil", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high runic power", NextAction::array(0, new NextAction("death coil", ACTION_HIGH + 1), nullptr)));
}

NextAction** BloodDKStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("death strike", ACTION_NORMAL + 2),
        new NextAction("heart strike", ACTION_NORMAL + 1),
        new NextAction("rune strike", ACTION_NORMAL),
        new NextAction("melee", ACTION_DEFAULT),
        nullptr);
}

void BloodDKStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericDKStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("blood presence", NextAction::array(0, new NextAction("blood presence", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("lose aggro", NextAction::array(0, new NextAction("dark command", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("bone shield", NextAction::array(0, new NextAction("bone shield", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("vampiric blood", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("rune tap", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("dancing rune weapon", NextAction::array(0, new NextAction("dancing rune weapon", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("raise dead", NextAction::array(0, new NextAction("raise dead", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("crimson scourge", NextAction::array(0, new NextAction("blood boil", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high runic power", NextAction::array(0, new NextAction("rune strike", ACTION_HIGH + 1), nullptr)));
}

void DKAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("death and decay", ACTION_HIGH + 3),
        new NextAction("blood boil", ACTION_HIGH + 2),
        nullptr)));
}
