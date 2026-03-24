#pragma once

namespace cme {
    class JsonSerializer;

    /// @brief Interfaz que hace que los objetos se vuelvan serializables
    class Serializable {
    public:
        virtual void serialize(JsonSerializer& s) const = 0;
        virtual void deserialize(JsonSerializer& s) = 0;
    };
}