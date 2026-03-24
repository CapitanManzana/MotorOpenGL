#include "Mesh.h"
#include "Shader.h"
#include <utils/logger.h>

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

        // Nos desvinculamos para no modificar este VAO por accidente después
        glBindVertexArray(0);
    }

    void Mesh::render() const {
        if (!_shader) {
            LOG_ERROR("La mesh no tiene shader asignado");
            return;
        }

        _shader->use();
        glBindVertexArray(_VAO);
        if (!_indices.empty()) {
            glDrawElements(mPrimitive, _indices.size() * 3, GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(mPrimitive, 0, _vertices.size());
        }

        glBindVertexArray(0);
    }
}