/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "AvoidAoeValues.h"

#include "Playerbots.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"

namespace
{
    struct TrapInRangeCheck
    {
        TrapInRangeCheck(WorldObject const* source, float range) : source(source), range(range) {}

        bool operator()(GameObject* go) const
        {
            return go->GetGoType() == GAMEOBJECT_TYPE_TRAP && go->isSpawned() && source->IsWithinDistInMap(go, range);
        }

        WorldObject const* source;
        float range;
    };
}

Aura* AreaDebuffValue::Calculate()
{
    for (AuraType type : { SPELL_AURA_PERIODIC_DAMAGE, SPELL_AURA_PERIODIC_DAMAGE_PERCENT, SPELL_AURA_PERIODIC_TRIGGER_SPELL,
                           SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, SPELL_AURA_DUMMY })
    {
        for (AuraEffect* aurEff : bot->GetAuraEffectsByType(type))
        {
            Aura* aura = aurEff ? aurEff->GetBase() : nullptr;
            if (!aura || aura->GetType() != DYNOBJ_AURA_TYPE || aura->GetSpellInfo()->IsPositive())
                continue;

            if (aura->GetDynobjOwner())
                return aura;
        }
    }

    return nullptr;
}

GuidVector NearestTrapWithDamageValue::Calculate()
{
    std::list<GameObject*> traps;
    TrapInRangeCheck check(bot, range);
    Trinity::GameObjectListSearcher<TrapInRangeCheck> searcher(bot, traps, check);
    Cell::VisitAllObjects(bot, searcher, range);

    GuidVector result;
    for (GameObject* go : traps)
    {
        Unit* owner = go->GetOwner();
        if (owner && owner->IsFriendlyTo(bot))
            continue;

        GameObjectTemplate const* goInfo = go->GetGOInfo();
        if (!goInfo || !goInfo->trap.spellId)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(goInfo->trap.spellId);
        if (!spellInfo || spellInfo->IsPositive())
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if ((spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA &&
                 spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_PERIODIC_DAMAGE) ||
                spellInfo->Effects[i].Effect == SPELL_EFFECT_SCHOOL_DAMAGE)
            {
                result.push_back(go->GetGUID());
                break;
            }
        }
    }

    return result;
}
