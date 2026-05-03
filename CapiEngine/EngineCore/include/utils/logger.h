#pragma once
#include <iostream>
#include <functional>
#include <sstream>
#include <utils/Singleton.h>
#include <format>

namespace cme {
    /// @brief Se encarga de mandar mensajes a la consola y a la ventana "ConsoleWindow" del motor
    class Logger : public Singleton<Logger> {
        friend class Singleton<Logger>;
    private:
        // Puntero a función para redirigir los logs
        std::function<void(const std::string&)> logCallback = nullptr;
    public:

        void setCallback(std::function<void(const std::string&)> cb) {
            logCallback = cb;
        }

        void clear() {
            logCallback = nullptr;
        }

        void send(const std::string& msg) {
            if (logCallback) logCallback(msg);
        }
    private:
        Logger() = default;
        ~Logger() {
            clear();
        }

        bool init() { return true; }
    };
}

inline cme::Logger& logger() {
    return *cme::Logger::Instance();
}

// Manda un mensaje a la consola
#define LOG_INFO(x) { \
    std::ostringstream _ss; _ss << "[INFO] " << x; \
    std::cout << _ss.str() << std::endl; \
    if (cme::Logger::HasInstance()) logger().send(_ss.str()); \
}

// Manda un aviso a la consola
#define LOG_WARN(x) { \
    std::ostringstream _ss; _ss << "[WARN] " << x << " (" << __FILE__ << ":" << __LINE__ << ")"; \
    std::cout << "\033[33m" << _ss.str() << "\033[0m" << std::endl; \
    if (cme::Logger::HasInstance()) logger().send(_ss.str()); \
}

// Manda un error a la consola
#define LOG_ERROR(x) { \
    std::ostringstream _ss; _ss << "[ERROR] " << x << " (" << __FILE__ << ":" << __LINE__ << ")"; \
    std::cerr << "\033[31m" << _ss.str() << "\033[0m" << std::endl; \
    if (cme::Logger::HasInstance()) logger().send(_ss.str()); \
}