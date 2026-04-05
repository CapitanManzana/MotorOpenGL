#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <utils/logger.h>
#include <ec/component.h>

namespace cme {
    /// @brief Se encarga de registrar y crear los componentes para luego poder crearlos en tiempo de ejecución
    class ComponentRegistry {
    private:
        // El mapa: Relaciona String -> Función que devuelve un Component*
        using InstancerFunc = std::function<ec::Component* ()>;
        inline static std::unordered_map<std::string, InstancerFunc> _registry;

    public:
        /// @brief Registara un componente a su lista
        /// @tparam T El tipo de componente
        /// @param name El nombre de key para luego crearlo
        template<typename T>
        static void registerComponent(const std::string& name) {
            _registry[name] = []() -> ec::Component* {
                return new T();
            };
        }

        /// @brief Crea un componente dado su tipo
        /// @param name El tipo de componente
        /// @return Un puntero al componente
        static ec::Component* create(const std::string& name) {
            auto it = _registry.find(name);
            if (it != _registry.end()) {
                return it->second(); // Ejecuta la lambda y devuelve el 'new Componente()'
            }
            LOG_WARN("Componente no registrado: " + name);
            return nullptr;
        }
    };
}