/*
 * Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT) \
    if (!ElunaConfig::GetInstance().IsElunaEnabled())\
        return;\
    auto key = EventKey<AllCreatureEvents>(EVENT);\
    if (!AllCreatureEventBindings->HasBindingsFor(key))\
        return;\
    LOCK_ELUNA

#define START_HOOK_WITH_RETVAL(EVENT, RETVAL) \
    if (!ElunaConfig::GetInstance().IsElunaEnabled())\
        return RETVAL;\
    auto key = EventKey<AllCreatureEvents>(EVENT);\
    if (!AllCreatureEventBindings->HasBindingsFor(key))\
        return RETVAL;\
    LOCK_ELUNA

void Eluna::OnAllCreatureAddToWorld(Creature* creature)
{
    START_HOOK(ALL_CREATURE_EVENT_ON_ADD);
    Push(creature);
    CallAllFunctions(AllCreatureEventBindings, key);
}

void Eluna::OnAllCreatureRemoveFromWorld(Creature* creature)
{
    START_HOOK(ALL_CREATURE_EVENT_ON_REMOVE);
    Push(creature);
    CallAllFunctions(AllCreatureEventBindings, key);
}

void Eluna::OnAllCreatureSelectLevel(const CreatureTemplate* cinfo, Creature* creature)
{
    START_HOOK(ALL_CREATURE_EVENT_ON_SELECT_LEVEL);
    Push(cinfo);
    Push(creature);
    CallAllFunctions(AllCreatureEventBindings, key);
}

void Eluna::OnAllCreatureBeforeSelectLevel(const CreatureTemplate* cinfo, Creature* creature, uint8& level)
{
    START_HOOK(ALL_CREATURE_EVENT_ON_BEFORE_SELECT_LEVEL);
    Push(cinfo);
    Push(creature);
    Push(level);
    int levelIndex = lua_gettop(L);
    int n = SetupStack(AllCreatureEventBindings, key, 3);

    while (n > 0)
    {
        int r = CallOneFunction(n--, 3, 1);

        if (lua_isnumber(L, r))
        {
            level = CHECKVAL<uint8>(L, r);
            // Update the stack for subsequent calls.
            ReplaceArgument(level, levelIndex);
        }

        lua_pop(L, 1);
    }

    CleanUpStack(3);
}