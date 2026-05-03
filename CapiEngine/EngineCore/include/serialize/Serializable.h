#pragma once

namespace cme {
    class JsonSerializer;

    /// @brief Interfaz que hace que los objetos se vuelvan serializables
    class Serializable {
    public:
		/// @brief Serializa el objeto en un JSON utilizando el JsonSerializer proporcionado
        /// @param s El JsonSerializer que se utilizará para serializar el objeto
        virtual void serialize(JsonSerializer& s) const = 0;
        /// @brief Deserializa el objeto desde un JSON utilizando el JsonSerializer proporcionado
        /// @param s El JsonSerializer que se utilizará para deserializar el objeto
        virtual void deserialize(JsonSerializer& s) = 0;
    };
}