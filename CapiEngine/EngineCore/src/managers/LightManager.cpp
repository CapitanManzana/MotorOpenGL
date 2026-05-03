#include "managers/LightManager.h"
#include <surface/Shader.h>
#include <utils/logger.h>
#include <format>

namespace cme {

    LightManager& LightManager::get() {
        static LightManager instance;
        return instance;
    }

    int LightManager::addLight(const PointLight& light) {
        if ((int)_lights.size() >= MAX_POINT_LIGHTS) {
            LOG_WARN(std::format("LightManager: maximo de luces puntuales alcanzado ({})", MAX_POINT_LIGHTS));
            return -1;
        }
        _lights.push_back(light);
        return (int)_lights.size() - 1;
    }

    void LightManager::removeLight(int index) {
        if (index < 0 || index >= (int)_lights.size()) return;
        _lights.erase(_lights.begin() + index);
    }

    void LightManager::clearLights() {
        _lights.clear();
    }

    void LightManager::setLight(int index, const PointLight& light) {
        if (index < 0 || index >= (int)_lights.size()) return;
        _lights[index] = light;
    }

    void LightManager::uploadToShader(Shader* shader) const {
        if (!shader) return;

        const int count = (int)_lights.size();
        shader->use();

        for (int i = 0; i < count; ++i) {
            const std::string base = std::format("pointLights[{}].", i);
            shader->setUniform(base + "position",  _lights[i].position);
            shader->setUniform(base + "color",     _lights[i].color);
            shader->setUniform(base + "intensity",  _lights[i].intensity);
            shader->setUniform(base + "constant",   _lights[i].constant);
            shader->setUniform(base + "linear",     _lights[i].linear);
            shader->setUniform(base + "quadratic", _lights[i].quadratic);
        }

        shader->setUniform("numPointLights", count);
    }

} // namespace cme
