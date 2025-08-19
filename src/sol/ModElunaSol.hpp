#ifndef MOD_ELUNA_SOL_HPP
#define MOD_ELUNA_SOL_HPP

// Configuration avant d'inclure Sol
#include "config.hpp"

// Inclure nos headers Lua existants AVANT Sol
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// Maintenant on peut inclure Sol qui va utiliser nos headers
#include "sol.hpp"

namespace ModEluna {
    
// Wrapper pour utiliser Sol avec notre lua_State existant
class SolWrapper {
private:
    sol::state_view lua_view;
    
public:
    explicit SolWrapper(lua_State* L) : lua_view(L) {}
    
    // Accès direct au sol::state_view
    sol::state_view& get() { return lua_view; }
    const sol::state_view& get() const { return lua_view; }
    
    // Operators pour faciliter l'usage
    sol::state_view* operator->() { return &lua_view; }
    const sol::state_view* operator->() const { return &lua_view; }
    
    sol::state_view& operator*() { return lua_view; }
    const sol::state_view& operator*() const { return lua_view; }
    
    // Helper pour compatibility avec notre code existant
    lua_State* lua_state() const { return lua_view.lua_state(); }
};

} // namespace ModEluna

#endif // MOD_ELUNA_SOL_HPP