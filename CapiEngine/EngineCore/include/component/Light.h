#pragma once
#include <ec/component.h>
#include <glm/vec4.hpp> 
#include <component/Transform.h>
#include <lighting/PointLight.h>

namespace cme {

    /// @brief Componente que registra una luz puntual en el LightManager.
    ///        Requiere un Transform en la misma entidad.
    class Light : public ec::Component, public ec::UpdateComponent
    {
    public:
        PointLight  _pointLight;    // Datos de la luz (posicion se sincroniza cada frame)
        int         _lightIndex = -1; // Indice en el LightManager (-1 = no registrada)
        Transform* _tr = nullptr;

        __CMPID_DECL__(ec::comp::LIGHT)

        Light() = default;
        ~Light() override;

        void initComponent() override;
        void update() override;  // Sincroniza la posicion con el Transform cada frame

        void serialize(JsonSerializer& s) const override;
        void deserialize(JsonSerializer& s)       override;

        std::string serializeID() const override { return "Light"; }

        // --- Accessors ---
        const PointLight& pointLight()  const { return _pointLight; }
        PointLight& pointLight() { return _pointLight; }

        glm::vec3 getPosition() const { return _tr->getPosition(); }
    };
}
