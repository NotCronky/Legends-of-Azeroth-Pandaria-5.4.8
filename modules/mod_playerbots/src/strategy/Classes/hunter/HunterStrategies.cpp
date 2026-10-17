/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "HunterStrategies.h"

#include "Playerbots.h"

namespace
{
    // Spells that may not be known yet fall back to one that is
    class HunterActionNodeFactory : public NamedObjectFactory<ActionNode>
    {
    public:
        HunterActionNodeFactory()
        {
            creators["aspect of the iron hawk"] = &aspect_of_the_iron_hawk;
            creators["cobra shot"] = &cobra_shot;
            creators["deterrence"] = &deterrence;
        }

    private:
        ACTION_NODE_A(aspect_of_the_iron_hawk, "aspect of the iron hawk", "aspect of the hawk");
        // Cobra Shot from level 81
        ACTION_NODE_A(cobra_shot, "cobra shot", "steady shot");
        ACTION_NODE_A(deterrence, "deterrence", "feign death");
    };
}

GenericHunterStrategy::GenericHunterStrategy(PlayerbotAI* botAI) : RangedCombatStrategy(botAI)
{
    actionNodeFactories.Add(new HunterActionNodeFactory());
}

NextAction** GenericHunterStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("auto shot", ACTION_DEFAULT), nullptr);
}

void GenericHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("auto shot dead target", NextAction::array(0, new NextAction("stop auto shot", ACTION_INTERRUPT + 2), nullptr)));

    // Get in range before the shots below, which outrank the generic "reach spell"
    triggers.push_back(new TriggerNode("enemy out of spell", NextAction::array(0, new NextAction("reach spell", ACTION_MOVE + 9), nullptr)));
    triggers.push_back(new TriggerNode("no aspect", NextAction::array(0, new NextAction("aspect of the iron hawk", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("hunter no pet", NextAction::array(0, new NextAction("call pet 1", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("hunter pet low health", NextAction::array(0, new NextAction("mend pet", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("misdirection on main tank", NextAction::array(0, new NextAction("misdirection on main tank", ACTION_HIGH + 8), nullptr)));

    triggers.push_back(new TriggerNode("target critical health", NextAction::array(0, new NextAction("kill shot", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("hunter's mark", NextAction::array(0, new NextAction("hunter's mark", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("serpent sting", NextAction::array(0, new NextAction("serpent sting", ACTION_HIGH + 3), nullptr)));

    triggers.push_back(new TriggerNode("counter shot", NextAction::array(0, new NextAction("counter shot", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("deterrence", ACTION_INTERRUPT + 1), nullptr)));

    // Level 60 and 90 talents: whichever the bot has
    triggers.push_back(new TriggerNode("a murder of crows", NextAction::array(0, new NextAction("a murder of crows", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("lynx rush", NextAction::array(0, new NextAction("lynx rush", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("glaive toss", NextAction::array(0, new NextAction("glaive toss", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("powershot", NextAction::array(0, new NextAction("powershot", ACTION_HIGH + 4), nullptr)));
}

NextAction** BeastMasteryHunterStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("cobra shot", ACTION_NORMAL), new NextAction("auto shot", ACTION_DEFAULT), nullptr);
}

void BeastMasteryHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericHunterStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("frenzy five stacks", NextAction::array(0, new NextAction("focus fire", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("high focus", NextAction::array(0,
        new NextAction("bestial wrath", ACTION_HIGH + 7),
        new NextAction("arcane shot", ACTION_HIGH + 1),
        nullptr)));
    triggers.push_back(new TriggerNode("kill command", NextAction::array(0, new NextAction("kill command", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("dire beast", NextAction::array(0, new NextAction("dire beast", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("thrill of the hunt", NextAction::array(0, new NextAction("arcane shot", ACTION_HIGH + 2), nullptr)));
}

NextAction** MarksmanshipHunterStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("steady shot", ACTION_NORMAL), new NextAction("auto shot", ACTION_DEFAULT), nullptr);
}

void MarksmanshipHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericHunterStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("chimera shot", NextAction::array(0, new NextAction("chimera shot", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("fire!", NextAction::array(0, new NextAction("aimed shot", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("thrill of the hunt", NextAction::array(0, new NextAction("arcane shot", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high focus", NextAction::array(0, new NextAction("arcane shot", ACTION_HIGH + 1), nullptr)));
}

NextAction** SurvivalHunterStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("cobra shot", ACTION_NORMAL), new NextAction("auto shot", ACTION_DEFAULT), nullptr);
}

void SurvivalHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericHunterStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("lock and load", NextAction::array(0, new NextAction("explosive shot", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("explosive shot", NextAction::array(0, new NextAction("explosive shot", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("black arrow", NextAction::array(0, new NextAction("black arrow", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("thrill of the hunt", NextAction::array(0, new NextAction("arcane shot", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("high focus", NextAction::array(0, new NextAction("arcane shot", ACTION_HIGH + 1), nullptr)));
}

void HunterBoostStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("rapid fire", NextAction::array(0, new NextAction("rapid fire", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("stampede", NextAction::array(0, new NextAction("stampede", ACTION_HIGH + 7), nullptr)));
}

void HunterAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("barrage", ACTION_HIGH + 6),
        new NextAction("multi-shot", ACTION_HIGH + 5),
        nullptr)));
}
