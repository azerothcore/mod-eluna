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
    if (!IsEnabled())\
        return;\
    auto key = EventKey<TicketEvents>(EVENT);\
    if (!TicketEventBindings->HasBindingsFor(key))\
        return;\
    LOCK_ELUNA

#define START_HOOK(EVENT) \
    if (!IsEnabled())\
        return;\
    auto key = EventKey<TicketEvents>(EVENT);\
    if (!TicketEventBindings->HasBindingsFor(key))\
        return;\
    LOCK_ELUNA

void Eluna::OnTicketCreate(Player* player, GmTicket* ticket)
{
    START_HOOK(TICKET_EVENT_ON_CREATE);
    Push(player);
    Push(ticket);
    CallAllFunctions(TicketEventBindings, key);
}

void Eluna::OnTicketUpdate(Player* player, GmTicket* ticket)
{
    START_HOOK(TICKET_EVENT_ON_UPDATE);
    Push(player);
    Push(ticket);
    CallAllFunctions(TicketEventBindings, key);
}

void Eluna::OnTicketClose(Player* player, GmTicket* ticket)
{
    START_HOOK(TICKET_EVENT_ON_CLOSE);
    Push(player);
    Push(ticket);
    CallAllFunctions(TicketEventBindings, key);
}

void Eluna::OnTicketStatusUpdate(Player* player, GmTicket* ticket)
{
    START_HOOK(TICKET_EVENT_STATUS_UPDATE);
    Push(player);
    Push(ticket);
    CallAllFunctions(TicketEventBindings, key);
}

void Eluna::OnTicketResolve(Player* player, GmTicket* ticket)
{
    START_HOOK(TICKET_EVENT_ON_RESOLVE);
    Push(player);
    Push(ticket);
    CallAllFunctions(TicketEventBindings, key);
}

