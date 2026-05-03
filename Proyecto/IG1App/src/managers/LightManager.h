#pragma once
#include <vector>
#include <core/lighting/PointLight.h>

namespace cme {

    class Shader;

    /// @brief Singleton que almacena todas las luces puntuales de la escena
    ///        y las sube al shader activo en cada frame.
    class LightManager {
    public:
        static constexpr int MAX_POINT_LIGHTS = 8;

        static LightManager& get();

        // --- Gestion de luces ---

        /// @brief Anade una luz puntual. Devuelve el indice asignado, o -1 si se alcanzo el maximo.
        int  addLight(const PointLight& light);
        /// @brief Elimina la luz en el indice dado.
        void removeLight(int index);
        /// @brief Elimina todas las luces puntuales.
        void clearLights();
        /// @brief Modifica una luz existente.
        void setLight(int index, const PointLight& light);

        const PointLight&              getLight(int index) const { return _lights[index]; }
        const std::vector<PointLight>& lights()            const { return _lights; }
        int                            count()             const { return (int)_lights.size(); }

        // --- Upload ---

        /// @brief Sube todas las luces puntuales al shader activo.
        ///        El shader debe estar activado (use()) antes de llamar a esto.
        void uploadToShader(Shader* shader) const;

    private:
        LightManager() = default;
        std::vector<PointLight> _lights;
    };

    /// @brief Acceso rapido al LightManager (convencion del proyecto)
    inline LightManager& lightM() { return LightManager::get(); }

} // namespace cme
