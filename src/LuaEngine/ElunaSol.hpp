#ifndef ELUNA_SOL_HPP
#define ELUNA_SOL_HPP

#include "config.hpp"

#include "sol.hpp"

namespace ElunaSol
{
class SolWrapper {
private:
    sol::state_view lua_view;
    
public:
    explicit SolWrapper(lua_State* L) : lua_view(L) {}
    
    sol::state_view& get() { return lua_view; }
    const sol::state_view& get() const { return lua_view; }
    
    sol::state_view* operator->() { return &lua_view; }
    const sol::state_view* operator->() const { return &lua_view; }
    
    sol::state_view& operator*() { return lua_view; }
    const sol::state_view& operator*() const { return lua_view; }
    
    lua_State* lua_state() const { return lua_view.lua_state(); }
};
} // namespace ElunaSol



#endif