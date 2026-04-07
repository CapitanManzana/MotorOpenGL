#include "Mesh.h"
#include "Shader.h"
#include <utils/logger.h>
#include <managers/ResourceManager.h>
#include <core/Material.h>

namespace cme {
    Mesh::~Mesh() {
        if (_VAO != 0) {
            glDeleteVertexArrays(1, &_VAO);
            glDeleteBuffers(1, &_VBO);
            _VAO = 0;
            _VBO = 0;

            if (_CBO != 0) {
                glDeleteBuffers(1, &_CBO);
                _CBO = 0;
            }

            if (_EBO != 0) {
                glDeleteBuffers(1, &_EBO);
                _EBO = 0;
            }

            if (_NBO != 0) {
                glDeleteBuffers(1, &_NBO);
                _NBO = 0;
            }

            if (_TBO != 0) {
                glDeleteBuffers(1, &_TBO);
                _TBO = 0;
            }
        }

        if (_LightVAO != 0) {
            glDeleteVertexArrays(1, &_LightVAO);
            glDeleteBuffers(1, &_VBO);
            _VAO = 0;
            _VBO = 0;
        }
    }

    void Mesh::initBuffers() {
        // Creamos y activamos el recepcionista (VAO) primero
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);

        // Posiciones (VBO)
        glGenBuffers(1, &_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        // Colores (CBO)
        if (!_vColor.empty()) {
            glGenBuffers(1, &_CBO);
            glBindBuffer(GL_ARRAY_BUFFER, _CBO);
            glBufferData(GL_ARRAY_BUFFER, _vColor.size() * sizeof(glm::vec4), _vColor.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);
            glEnableVertexAttribArray(1);
        }

        // Índices (EBO) Si la figura decide usarlos
        if (!_indices.empty()) {
            glGenBuffers(1, &_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(glm::uvec3), _indices.data(), GL_STATIC_DRAW);
        }

        if (!_normals.empty()) {
            glGenBuffers(1, &_NBO);
            glBindBuffer(GL_ARRAY_BUFFER, _NBO);
            glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), _normals.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glEnableVertexAttribArray(2);
        }

        if (!_texCoords.empty()) {
            glGenBuffers(1, &_TBO);
            glBindBuffer(GL_ARRAY_BUFFER, _TBO);
            glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(glm::vec2), _texCoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
            glEnableVertexAttribArray(3);
        }

        // Nos desvinculamos para no modificar este VAO por accidente después
        glBindVertexArray(0);
    }

    void Mesh::render() const {
        if (!_mat) {
            LOG_ERROR("La mesh no tiene shader asignado");
            return;
        }

        if (!_shader) _mat->apply();
        else _shader->use();

        glBindVertexArray(_VAO);
        if (!_indices.empty()) {
            glDrawElements(mPrimitive, _indices.size() * 3, GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(mPrimitive, 0, _vertices.size());
        }

        glBindVertexArray(0);
    }

    void Mesh::getLocalAABB(glm::vec3& outMin, glm::vec3& outMax) const {
        if (_vertices.empty()) {
            outMax = glm::vec3(0.0f);
            outMin = glm::vec3(0.0f);
            return;
        }

        outMin = _vertices[0];
        outMax = _vertices[0];

        for (const auto& v : _vertices) {
            outMin.x = std::min(outMin.x, v.x);
            outMin.y = std::min(outMin.y, v.y);
            outMin.z = std::min(outMin.z, v.z);

            outMax.x = std::max(outMax.x, v.x);
            outMax.y = std::max(outMax.y, v.y);
            outMax.z = std::max(outMax.z, v.z);
        }
    }

    void Mesh::setLightSource(bool value) {
        if (value) {
            glGenVertexArrays(1, &_LightVAO);
            glBindVertexArray(_LightVAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);

            _mat->shader = rscrM().getShader("lightSource");
        }
        else {
            if (_LightVAO != 0) {
                glDeleteVertexArrays(1, &_LightVAO);
                glDeleteBuffers(1, &_VBO);
                _VAO = 0;
                _VBO = 0;
            }

            _mat->shader = rscrM().getShader("default");
        }

        _isLightSource = value;
    }

    void Mesh::calculateNormalMatrix() {
        _normalMatrix = glm::transpose(glm::inverse(glm::mat3(_model)));
    }

    void Mesh::setModelMatrix(glm::mat4 model) {
        _model = model;
        if (_LightVAO != 0) calculateNormalMatrix();
    }
}