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

	size_t JsonSerializer::getScopeSize() const {
		if (_scope && _scope->is_object()) return _scope->size();
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
	JsonValue JsonSerializer::readFromArray(int index) {
		if (_scope && _scope->is_array() && index < (int)_scope->size()) {
			auto& item = (*_scope)[index];

			if (item.is_number_integer()) return item.get<int>();
			if (item.is_number_float())   return item.get<float>();
			if (item.is_boolean())        return item.get<bool>();
			if (item.is_string())         return item.get<std::string>();
			if (item.is_array()) {
				if (item.size() == 2) return glm::vec2(item[0], item[1]);
				if (item.size() == 3) return glm::vec3(item[0], item[1], item[2]);
				if (item.size() == 4) return glm::vec4(item[0], item[1], item[2], item[3]);
			}

			LOG_WARN("Tipo no soportado en readFromArray");
			return JsonValue{ 0 };
		}
		LOG_WARN(std::format("Indice invalido o scope no es array (indice: {})", index));
		return JsonValue{ 0 };
	}

	void JsonSerializer::pushToArray(JsonValue value) {
		if (!_scope || !_scope->is_array()) {
			LOG_WARN("Se intenta hacer push a un array cuando no lo es");
			return;
		}

		std::visit([&](auto&& val) {
			using T = std::decay_t<decltype(val)>;

			if constexpr (std::is_same_v<T, glm::vec2>)
				_scope->push_back({ val.x, val.y });
			else if constexpr (std::is_same_v<T, glm::vec3>)
				_scope->push_back({ val.x, val.y, val.z });
			else if constexpr (std::is_same_v<T, glm::vec4>)
				_scope->push_back({ val.x, val.y, val.z, val.w });
			else
				_scope->push_back(val); // int, float, bool, string funcionan directo
			}, value);
	}


	void JsonSerializer::write(const std::string& key, float value) {
		if (_scope) (*_scope)[key] = value;
	}

	void JsonSerializer::write(const std::string& key, int value) {
		if (_scope) (*_scope)[key] = value;
	}


	void JsonSerializer::write(const std::string& key, const glm::vec2& value) {
		if (_scope) (*_scope)[key] = { value.x, value.y };
	}

	void JsonSerializer::write(const std::string& key, const glm::vec3& value) {
		if (_scope) (*_scope)[key] = { value.x, value.y, value.z };
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

	glm::vec2 JsonSerializer::readVec2(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			auto& array = (*_scope)[key];
			return glm::vec2(array.at(0), array.at(1));
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return glm::vec2(0,0);
	}

	glm::vec3 JsonSerializer::readVec3(const std::string& key) const {
		if (_scope && (*_scope).contains(key)) {
			auto& array = (*_scope)[key];
			return glm::vec3(array.at(0), array.at(1), array.at(2));
		}

		LOG_ERROR(std::format("No se encontro el scope o la key no existe (key: {})", key));
		return glm::vec3(0, 0, 0);
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

	std::string JsonSerializer::getKey(int index) {
		if (index < 0 || index >= (int)_scope->size()) return "";
		auto it = std::next(_scope->begin(), index);
		return it.key();
	}

	JsonValue JsonSerializer::getValue(int index) {
		if (index < 0 || index >= (int)_scope->size()) return JsonValue{ 0 };

		auto& item = *std::next(_scope->begin(), index);

		if (item.is_number_integer()) return item.get<int>();
		if (item.is_number_float())   return item.get<float>();
		if (item.is_boolean())        return item.get<bool>();
		if (item.is_string())         return item.get<std::string>();
		if (item.is_array()) {
			if (item.size() == 2) return glm::vec2(item[0], item[1]);
			if (item.size() == 3) return glm::vec3(item[0], item[1], item[2]);
			if (item.size() == 4) return glm::vec4(item[0], item[1], item[2], item[3]);
		}

		LOG_WARN("Tipo no soportado en getValue");
		return JsonValue{ 0 };
	}
}