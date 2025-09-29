#ifndef ELUNA_CONFIG_HPP
#define ELUNA_CONFIG_HPP

#include "ConfigValueCache.h"

enum class ElunaConfigValues : uint32
{
    // Boolean
    ENABLED = 0,
    TRACEBACK_ENABLED,
    AUTORELOAD_ENABLED,
    BYTECODE_CACHE_ENABLED,

    // String
    SCRIPT_PATH,
    REQUIRE_PATH,
    REQUIRE_CPATH,

    // Number
    AUTORELOAD_INTERVAL,

    CONFIG_VALUE_COUNT
};

class ElunaConfig final : public ConfigValueCache<ElunaConfigValues>
{
    public:
        static ElunaConfig& GetInstance();

        void Initialize(bool reload = false);

        bool IsElunaEnabled() const { return GetConfigValue<bool>(ElunaConfigValues::ENABLED); }
        bool IsTraceBackEnabled() const { return GetConfigValue<bool>(ElunaConfigValues::TRACEBACK_ENABLED); }
        bool IsAutoReloadEnabled() const { return GetConfigValue<bool>(ElunaConfigValues::AUTORELOAD_ENABLED); }
        bool IsByteCodeCacheEnabled() const { return GetConfigValue<bool>(ElunaConfigValues::BYTECODE_CACHE_ENABLED); }

        std::string_view GetScriptPath() const { return GetConfigValue(ElunaConfigValues::SCRIPT_PATH); }
        std::string_view GetRequirePath() const { return GetConfigValue(ElunaConfigValues::REQUIRE_PATH); }
        std::string_view GetRequireCPath() const { return GetConfigValue(ElunaConfigValues::REQUIRE_CPATH); }

        uint32 GetAutoReloadInterval() const { return GetConfigValue<uint32>(ElunaConfigValues::AUTORELOAD_INTERVAL); }

    protected:
        void BuildConfigCache() override;

    private:
        ElunaConfig();
        ~ElunaConfig() = default;
        ElunaConfig(const ElunaConfig&) = delete;
        ElunaConfig& operator=(const ElunaConfig&) = delete;
};

#endif // ELUNA_CONFIG_H