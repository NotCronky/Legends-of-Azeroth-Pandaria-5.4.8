/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "MageStrategies.h"

#include "Playerbots.h"

namespace
{
    // What every mage does: Arcane Brilliance, interrupt, Frost Nova when caught in melee, Mirror Image,
    // Evocation for mana, Ice Block
    void AddMageTriggers(std::vector<TriggerNode*>& triggers)
    {
        triggers.push_back(new TriggerNode("arcane brilliance", NextAction::array(0, new NextAction("arcane brilliance", ACTION_HIGH + 4), nullptr)));
        triggers.push_back(new TriggerNode("counterspell", NextAction::array(0, new NextAction("counterspell", ACTION_INTERRUPT + 2), nullptr)));
        triggers.push_back(new TriggerNode("counterspell on enemy healer", NextAction::array(0, new NextAction("counterspell on enemy healer", ACTION_INTERRUPT + 1), nullptr)));
        triggers.push_back(new TriggerNode("enemy too close for spell", NextAction::array(0, new NextAction("frost nova", ACTION_HIGH + 8), nullptr)));
        triggers.push_back(new TriggerNode("mirror image", NextAction::array(0, new NextAction("mirror image", ACTION_HIGH + 6), nullptr)));
        triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("evocation", ACTION_HIGH + 5), nullptr)));
        triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("ice block", ACTION_INTERRUPT), nullptr)));
    }
}

NextAction** ArcaneMageStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("arcane blast", ACTION_NORMAL), nullptr);
}

void ArcaneMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddMageTriggers(triggers);

    triggers.push_back(new TriggerNode("mage armor", NextAction::array(0, new NextAction("mage armor", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("arcane power", NextAction::array(0, new NextAction("arcane power", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("nether tempest", NextAction::array(0, new NextAction("nether tempest", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("arcane missiles!", NextAction::array(0, new NextAction("arcane missiles", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("arcane charges", NextAction::array(0, new NextAction("arcane barrage", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("arcane explosion", ACTION_HIGH + 2), nullptr)));
}

NextAction** FireMageStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("fireball", ACTION_NORMAL), nullptr);
}

void FireMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddMageTriggers(triggers);

    triggers.push_back(new TriggerNode("molten armor", NextAction::array(0, new NextAction("molten armor", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("combustion", NextAction::array(0, new NextAction("combustion", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("living bomb", NextAction::array(0, new NextAction("living bomb", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("pyroblast!", NextAction::array(0, new NextAction("pyroblast", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("heating up", NextAction::array(0, new NextAction("inferno blast", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0,
        new NextAction("dragon's breath", ACTION_HIGH + 3),
        new NextAction("flamestrike", ACTION_HIGH + 2),
        nullptr)));
}

NextAction** FrostMageStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("frostbolt", ACTION_NORMAL), nullptr);
}

void FrostMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    RangedCombatStrategy::InitTriggers(triggers);
    AddMageTriggers(triggers);

    triggers.push_back(new TriggerNode("frost armor", NextAction::array(0, new NextAction("frost armor", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("water elemental", NextAction::array(0, new NextAction("summon water elemental", ACTION_HIGH + 8), nullptr)));
    triggers.push_back(new TriggerNode("icy veins", NextAction::array(0, new NextAction("icy veins", ACTION_HIGH + 7), nullptr)));
    triggers.push_back(new TriggerNode("frozen orb", NextAction::array(0, new NextAction("frozen orb", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("frost bomb", NextAction::array(0, new NextAction("frost bomb", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("brain freeze", NextAction::array(0, new NextAction("frostfire bolt", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("fingers of frost", NextAction::array(0, new NextAction("ice lance", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("blizzard", ACTION_HIGH + 2), nullptr)));
}

void MageCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("remove curse on party", NextAction::array(0, new NextAction("remove curse on party", ACTION_DISPEL), nullptr)));
}
