#pragma once
#include <glm/vec3.hpp>

namespace cme {

    /// @brief Datos de una luz puntual. Se registran en el LightManager para enviarse a los shaders.
    struct PointLight {
        glm::vec3 position  = glm::vec3(0.0f);
        glm::vec3 color     = glm::vec3(1.0f);
        float     intensity = 1.0f;

        // Coeficientes de atenuacion (por defecto: rango medio ~50 unidades)
        float constant  = 1.0f;
        float linear    = 0.09f;
        float quadratic = 0.032f;
    };

} // namespace cme
