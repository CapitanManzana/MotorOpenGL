#include "ViewportWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <managers/SceneManager.h>
#include <core/Scene.h>
#include <core/Camera.h>
#include <managers/InputManager.h>

namespace cme::ui {
    ViewportWindow::ViewportWindow(const char* name) : Window(name) {
        initFBO();
    }

	ViewportWindow::~ViewportWindow() {
		glDeleteFramebuffers(1, &_fbo);
		glDeleteTextures(1, &_texture);
		glDeleteRenderbuffers(1, &_rbo);
	}

	void ViewportWindow::initFBO() {
        glGenFramebuffers(1, &_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        // Textura donde se renderiza la imagen
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        // Depth buffer para que funcione el depth test
        glGenRenderbuffers(1, &_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void ViewportWindow::bindFBO() const {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glViewport(0, 0, _width, _height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ViewportWindow::unbindFBO() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ViewportWindow::renderWindowContent() {
        ImVec2 size = ImGui::GetContentRegionAvail();

        // Si la ventana cambia de tamaño, redimensionar el FBO
        if (size.x != _width || size.y != _height) {
            const_cast<ViewportWindow*>(this)->resizeFBO(size.x, size.y);
        }

        inpM().setViewportHovered(ImGui::IsWindowHovered());

        // Mostrar la textura
        ImGui::Image((ImTextureID)(intptr_t)_texture, size, ImVec2(0, 1), ImVec2(1, 0));
    }

    void ViewportWindow::resizeFBO(float w, float h) {
        _width = w; _height = h;

        sceneM().activeScene()->getCamera()->onResize(w, h);

        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    }

    void ViewportWindow::initialResize() {
        _width = 0;
        _height = 0;
    }
}