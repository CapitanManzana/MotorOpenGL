#include "JsonSerializer.h"
#include <utils/logger.h>
#include <iostream>
#include <fstream>

namespace cme {
	JsonSerializer::JsonSerializer() {
		_scope = &_data;
	}

	void JsonSerializer::beginScope(const std::string& key) {
		if (!_scope) return;
		if (!(*_scope).contains(key)) {
			(*_scope)[key] = json::object();
		}
		_scopeStack.push(&(*_scope)[key]);
		_scope = _scopeStack.top();
	}

	void JsonSerializer::endScope() {
		_scopeStack.pop();
		if (!_scopeStack.empty()) _scope = _scopeStack.top();
		else _scope = &_data;
	}

	// Crea un array si no existe y entra en él
	void JsonSerializer::beginArray(const std::string& key) {
		if (!_scope) return;
		if (!(*_scope).contains(key)) {
			(*_scope)[key] = json::array();
		}
		_scopeStack.push(&(*_scope)[key]);
		_scope = _scopeStack.top();
	}

	// Añade un objeto vacío `{}` al array actual y entra en él
	void JsonSerializer::pushObjectToArray() {
		if (!_scope || !_scope->is_array()) {
			LOG_WARN("Intentando hacer pushObjectToArray en un scope que no es array");
			return;
		}
		_scope->push_back(json::object());
		_scopeStack.push(&_scope->back());
		_scope = _scopeStack.top();
	}

	// ---- Métodos para iterar Arrays en Lectura ----
	size_t JsonSerializer::getArraySize() const {
		if (_scope && _scope->is_array()) {
			return _scope->size();
		}
		return 0;
	}

	void JsonSerializer::enterElement(size_t index) {
		if (_scope && _scope->is_array() && index < _scope->size()) {
			// Metemos el elemento [index] en la pila para que sea nuestro nuevo scope
			_scopeStack.push(&(*_scope)[index]);
			_scope = _scopeStack.top();
		}
		else {
			LOG_WARN(std::format("Intentando acceder a un indice invalido o el scope no es un array (indice: {})", index));
		}
	}

	void JsonSerializer::save(const std::string& path) const {
		std::ofstream archivo(path);
		archivo << std::setw(4) << _data << std::endl;
	}

	void JsonSerializer::load(const std::string& path) {
		std::ifstream archivo(path);
		if (!archivo) {
			LOG_ERROR(std::format("Archivo no encontrado al serializar ({})", path));
			return;
		}

		archivo >> _data;
	}

	// ---- Métodos de Lectura/Escritura ----

	void JsonSerializer::write(const std::string& key, float value) {
		if (_scope) (*_scope)[key] = value;
	}

	void JsonSerializer::write(const std::string& key, int value) {
		if (_scope) (*_scope)[key] = value;
	}

	void JsonSerializer::write(const std::string& key, const glm::vec3& value) {
		if (_scope) (*_scope)[key] = {value.x, value.y, value.z};
	}

	void JsonSerializer::write(const std::string& key, const glm::vec4& value) {
		if (_scope) (*_scope)[key] = { value.r, value.g, value.b, value.a };
	}

	void JsonSerializer::write(const std::string& key, const std::string& value) {
		if (_scope) (*_scope)[key] = value;
	}

	float JsonSerializer::readFloat(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			return (*_scope)[key];
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return 0;
	}

	int JsonSerializer::readInt(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			return (*_scope)[key];
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return 0;
	}

	glm::vec3 JsonSerializer::readVec3(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			auto& array = (*_scope)[key];
			return glm::vec3(array.at(0), array.at(1), array.at(2));
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return glm::vec3(0,0,0);
	}

	glm::vec4 JsonSerializer::readVec4(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			auto& array = (*_scope)[key];
			return glm::vec4(array.at(0), array.at(1), array.at(2), array.at(3));
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return glm::vec4(0, 0, 0, 0);
	}

	std::string JsonSerializer::readString(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			return (*_scope)[key];
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return "";
	}
}