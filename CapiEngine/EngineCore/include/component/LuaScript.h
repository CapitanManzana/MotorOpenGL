#pragma once
#include <ec/component.h>
#include <sol/sol.hpp>
#include <string>

namespace cme {

    class LuaScript : public ec::Component, public ec::UpdateComponent
    {
    private:
        std::string filepath;       // La ruta al archivo .lua asignado (ej: "assets/scripts/player.lua")

        sol::table scriptInstance;  // El estado individual. Si 5 enemigos usan el mismo script, cada uno necesita su propia tabla para no compartir la variable 'vida'.
        sol::function startFunc;    // Guardamos la función Start de Lua
        sol::function updateFunc;
    public:
        __CMPID_DECL__(ec::comp::LUA_SCRIPT)

        LuaScript() = default;
        ~LuaScript() override;

        void initComponent() override {}

        void start() override;
        void update() override;

        void serialize(JsonSerializer& s) const override;
        void deserialize(JsonSerializer& s)       override;

        std::string serializeID() const override { return "LuaScript"; }

        void loadScript(const std::string& path, sol::state& luaState);
    };
}
