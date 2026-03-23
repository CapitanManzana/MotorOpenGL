#pragma once

namespace capiEngine {
    class JsonSerializer;

    /// @brief Interfaz que hace que los objetos se vuelvan serializables
    class Serializable {
    public:
        virtual void serialize(capiEngine::JsonSerializer& s) const = 0;
        virtual void deserialize(capiEngine::JsonSerializer& s) = 0;
    };
}