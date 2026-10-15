/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_AIOBJECTCONTEXT_H
#define _PLAYERBOT_AIOBJECTCONTEXT_H

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "Common.h"
#include "NamedObjectContext.h"
#include "PlayerbotAIAware.h"
#include "Strategy.h"
#include "Trigger.h"
#include "Value.h"

class PlayerbotAI;

class AiObjectContext : public PlayerbotAIAware
{
public:
    AiObjectContext(PlayerbotAI* botAI);
    virtual ~AiObjectContext() {}

    virtual Strategy* GetStrategy(std::string const name);
    virtual std::set<std::string> GetSiblingStrategy(std::string const name);
    virtual Trigger* GetTrigger(std::string const name);
    virtual Action* GetAction(std::string const name);
    virtual UntypedValue* GetUntypedValue(std::string const name);

    // Never returns nullptr for a default-constructible T: callers dereference the result directly
    // (AI_VALUE), so an unregistered or mistyped name falls back to a per-context default value.
    template <class T>
    Value<T>* GetValue(std::string const name)
    {
        UntypedValue* untyped = GetUntypedValue(name);
        if (Value<T>* value = dynamic_cast<Value<T>*>(untyped))
            return value;

        if (untyped)
            LogMissing("value (type mismatch)", name);

        return GetFallbackValue<T>(name);
    }

    template <class T>
    Value<T>* GetValue(std::string const name, std::string const param)
    {
        return GetValue<T>((std::string(name) + "::" + param));
    }

    template <class T>
    Value<T>* GetValue(std::string const name, int32 param)
    {
        std::ostringstream out;
        out << param;
        return GetValue<T>(name, out.str());
    }

    std::set<std::string> GetValues();
    std::set<std::string> GetSupportedStrategies();
    std::set<std::string> GetSupportedActions();
    std::string const FormatValues();

    virtual void Update();
    virtual void Reset();
    virtual void AddShared(NamedObjectContext<UntypedValue>* sharedValues);

    std::vector<std::string> Save();
    void Load(std::vector<std::string> data);

    std::vector<std::string> performanceStack;

protected:
    // Logs each missing (type, name, class) combination once per server run.
    void LogMissing(char const* type, std::string const& name) const;

    template <class T>
    Value<T>* GetFallbackValue(std::string const& name)
    {
        if constexpr (std::is_default_constructible_v<T>)
        {
            std::unique_ptr<UntypedValue>& value = fallbackValues[name + "#" + typeid(T).name()];
            if (!value)
                value = std::make_unique<ManualSetValue<T>>(botAI, T(), name);

            return dynamic_cast<Value<T>*>(value.get());
        }
        else
            return nullptr;
    }

    NamedObjectContextList<Strategy> strategyContexts;
    NamedObjectContextList<Action> actionContexts;
    NamedObjectContextList<Trigger> triggerContexts;
    NamedObjectContextList<UntypedValue> valueContexts;
    std::map<std::string, std::unique_ptr<UntypedValue>> fallbackValues;
};

#endif
