#pragma once
#include <utils/json.hpp>
#include <stack>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using json = nlohmann::ordered_json;

namespace cme {
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
		void write(const std::string& key, const glm::vec4& value);
		void write(const std::string& key, const std::string& value);

		/// @brief Lee y devuelve un valor de punto flotante asociado a la clave especificada.
		/// @param key Clave cuyo valor float se desea leer.
		/// @return El valor de tipo float asociado a la clave.
		float readFloat(const std::string& key) const;
		/// @brief Lee y devuelve un valor entero asociado a la clave especificada.
		/// @param key Clave cuyo valor entero se desea leer.
		/// @return El valor de tipo entero asociado a la clave.
		int readInt(const std::string& key) const;
		/// @brief Lee y devuelve un valor de tipo glm::vec3 asociado a la clave especificada.
		/// @param key Clave cuyo valor glm::vec3 se desea leer.
		/// @return El valor de tipo glm::vec3 asociado a la clave.
		glm::vec3 readVec3(const std::string& key) const;
		/// @brief Lee y devuelve un valor de tipo glm::vec4 asociado a la clave especificada.
		/// @param key Clave cuyo valor glm::vec4 se desea leer.
		/// @return El valor de tipo glm::vec4 asociado a la clave.
		glm::vec4 readVec4(const std::string& key) const;
		/// @brief Lee y devuelve un valor de tipo string asociado a la clave especificada.
		/// @param key Clave cuyo valor string se desea leer.
		/// @return El valor de tipo string asociado a la clave.
		std::string readString(const std::string& key) const;

		/// @brief Inicia un nuevo array en el scope actual con la clave dada, permitiendo agregar elementos a ese array posteriormente.
		/// @param key Clave del array que se desea iniciar.
		void beginArray(const std::string& key);
		/// @brief Obtiene el tamaño del array en el scope actual.
		/// @return El tamaño del array.
		size_t getArraySize() const;
		/// @brief Entra en un elemento específico del array en el scope actual.
		/// @param index Índice del elemento al que se desea entrar.
		void enterElement(size_t index);
		/// @brief Agrega un nuevo objeto al array en el scope actual y entra en ese nuevo objeto, permitiendo agregar propiedades a ese objeto posteriormente.
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

