#pragma once
#include <windows/Window.h>

namespace cme::editor {
	/// @brief Ventana que se encarga de renderizar el viewport
	class ViewportWindow : public Window
	{
	private:
		unsigned int _fbo = 0;
		unsigned int _texture = 0;
		unsigned int _rbo = 0;      // depth buffer
		float _width = 800;
		float _height = 600;
	public:
		WINDOW_ID(windowGroupID::VIEWPORT)
		ViewportWindow(const char* name);

		~ViewportWindow();

		void initialResize();
		/// @brief Le dice a opengl que active el FBO, se tiene que llamar al principio
		void bindFBO() const;
		/// @brief Desactiva el FBO
		void unbindFBO() const;
	private:
		void initFBO();
		void resizeFBO(float w, float h);
	protected:
		virtual void renderWindowContent() override;
	};
}
