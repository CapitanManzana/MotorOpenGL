#pragma once
#include <GLFW/glfw3.h>
#include "../windows/UIGroups.h"
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
namespace cme::editor {
	class Window;

	/// @brief Se encarga de controlar las centanas y la interfaz de usuario del motor
	class UIManager
	{
	private:
		std::vector<std::shared_ptr<Window>> _windows;
		std::function<void()> _createCubeCallback;

		fs::path _dataPath;
	public:
		UIManager();
		~UIManager();
		bool initCoreUI(GLFWwindow* window, fs::path dataPath);

		void render() const;
		void start();

		void bind() const;
		void unbind() const;

		void setCreateCubeCallback(std::function<void()> call) { _createCubeCallback = call; }

		std::weak_ptr<Window> getWindow(windowGroupID windowID) { return _windows[windowID]; }
	private:
		bool initImgui(GLFWwindow* window);
		void renderMenuBar() const;
		void launchRuntime(const std::string& scenePath) const;

	public:
		template<typename T, typename... Args>
		T* addWindow(Args&&... args) {
			auto window = std::make_unique<T>(std::forward<Args>(args)...);
			T* ptr = window.get();

			constexpr windowGroupID id = static_cast<windowGroupID>(getWindowID<T>);
			_windows[id] = std::move(window);
			return ptr;
		}
	};
}

