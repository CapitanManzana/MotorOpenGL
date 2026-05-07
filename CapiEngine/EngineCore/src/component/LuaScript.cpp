#include <component/LuaScript.h>
#include <ec/entity.h>
#include <core/Scene.h>

namespace cme {
    void LuaScript::initComponent() {
        if (auto entity = _entity.lock()) {
            auto& lua = entity->getScene()->getLuaState();

            if (!filepath.empty()) loadScript(filepath, lua);
        }
    }


    void LuaScript::start() {
        if (startFunc.valid()) {
            startFunc(scriptInstance);
        }
    }

    void LuaScript::update() {
        if (updateFunc.valid()) {
            updateFunc(scriptInstance);
        }
    }

    void LuaScript::loadScript(const std::string& path, sol::state& lua) {
        filepath = path;

        // Ejecuta el archivo .lua — debe devolver una tabla con Start/Update
        sol::load_result file = lua.load_file(path);
        if (!file.valid()) {
            sol::error err = file;
            LOG_ERROR("Error cargando script Lua: " + std::string(err.what()));
            return;
        }

        // Obtenemos la "clase" que define el script
        sol::table scriptClass = file();
        if (!scriptClass.valid()) {
            LOG_ERROR("El script no devolvió una tabla válida: " + path);
            return;
        }

        // Creamos una instancia propia para este componente
        // (así dos entidades con el mismo script no comparten estado)
        scriptInstance = lua.create_table();
        scriptInstance[sol::metatable_key] = scriptClass;
        scriptClass["__index"] = scriptClass;

        // Guardamos las funciones si existen
        startFunc = scriptClass["Start"];
        updateFunc = scriptClass["Update"];
    }

    void LuaScript::serialize(JsonSerializer& s) const {
        s.write("script", filepath);
    }

    void LuaScript::deserialize(JsonSerializer& s) {
        filepath = s.readString("script");
        // loadScript se llama en initComponent(), cuando la Scene ya existe
    }
}