#pragma once
#include <utils/json.hpp>
#include <stack>
#include <glm/vec3.hpp>

using json = nlohmann::json;

namespace capiEngine {
	/// @brief Se encarga de serializar en un JSON lo que sea de tipo Serializable
	class JsonSerializer
	{
	private:
		json _data;		// Toda la info del JSON
		json* _scope = nullptr;
		std::stack<json*> _scopeStack;

	public:
		JsonSerializer();

		/// @brief Escribe un valor en el scope actual
		/// @param key La clave del json
		/// @param value El valor par la clave
		void write(const std::string& key, float value);
		void write(const std::string& key, int value);
		void write(const std::string& key, const glm::vec3& value);
		void write(const std::string& key, const std::string& value);

		float readFloat(const std::string& key) const;
		int readInt(const std::string& key) const;
		glm::vec3 readVec3(const std::string& key) const;
		std::string readString(const std::string& key) const;

		void beginArray(const std::string& key);
		void pushObjectToArray();

		/// @brief Cambia de scope para leer o escribir en el json
		/// @param key La clave del scope
		void beginScope(const std::string& key);
		/// @brief Vuelve al scope anterior
		void endScope();

		/// @brief Guarda el json en el lugar definido
		/// @param path Donde guardar el archivo
		void save(const std::string& path) const;
		/// @brief Carga la infoormación de un archivo json
		/// @param path La ubicacion del archivo
		void load(const std::string& path);
	};
}

