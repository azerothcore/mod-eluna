#include "ElunaConfig.h"

ElunaConfig& ElunaConfig::GetInstance()
{
    static ElunaConfig instance;
    return instance;
}

ElunaConfig::ElunaConfig() : ConfigValueCache<ElunaConfigValues>(ElunaConfigValues::CONFIG_VALUE_COUNT)
{
}

void ElunaConfig::Initialize(bool reload)
{
    ConfigValueCache<ElunaConfigValues>::Initialize(reload);
}

void ElunaConfig::BuildConfigCache()
{
    SetConfigValue<bool>(ElunaConfigValues::ENABLED,                    "Eluna.Enabled",            "false");
    SetConfigValue<bool>(ElunaConfigValues::TRACEBACK_ENABLED,          "Eluna.TraceBack",          "false");
    SetConfigValue<bool>(ElunaConfigValues::AUTORELOAD_ENABLED,         "Eluna.AutoReload",         "false");
    SetConfigValue<bool>(ElunaConfigValues::BYTECODE_CACHE_ENABLED,     "Eluna.BytecodeCache",      "false");

    SetConfigValue<std::string>(ElunaConfigValues::SCRIPT_PATH,         "Eluna.ScriptPath",         "lua_scripts");
    SetConfigValue<std::string>(ElunaConfigValues::REQUIRE_PATH,        "Eluna.RequirePaths",       "");
    SetConfigValue<std::string>(ElunaConfigValues::REQUIRE_CPATH,       "Eluna.RequireCPaths",      "");

    SetConfigValue<uint32>(ElunaConfigValues::AUTORELOAD_INTERVAL,      "Eluna.AutoReloadInterval", 1);
}
