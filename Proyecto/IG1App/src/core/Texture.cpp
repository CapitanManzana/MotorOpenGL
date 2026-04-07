#include "Texture.h"
#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <utils/logger.h>
#include <format>

namespace cme {
	Texture::~Texture()
	{
		if (mId != 0)
			glDeleteTextures(1, &mId);
	}

	void
		Texture::init()
	{
		glGenTextures(1, &mId);
		glBindTexture(GL_TEXTURE_2D, mId);

		// Filters and clamping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP
	}

	void
		Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, mId);
	}

	void
		Texture::load(const std::string& name, GLubyte alpha) {
		if (mId == 0) init();
		int nrChanel;
		_image = stbi_load(name.c_str(), &mWidth, &mHeight, &nrChanel, 0);

		if (!_image) {
			LOG_ERROR(std::format("No se pudo cargar la textura con nombre: {}", name));
			return;
		}

		GLenum format = GL_RGB;
		if (nrChanel == 1)      format = GL_RED;
		else if (nrChanel == 3) format = GL_RGB;
		else if (nrChanel == 4) format = GL_RGBA;

		GLint level = 0;  // Base image level
		GLint border = 0; // No border

		glBindTexture(GL_TEXTURE_2D, mId);
		glTexImage2D(GL_TEXTURE_2D, level, format, mWidth, mHeight, border, format,
			GL_UNSIGNED_BYTE, _image);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void
		Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP_TO_EDGE, ...
	{
		glBindTexture(GL_TEXTURE_2D, mId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::loadColorBuffer(GLsizei width, GLsizei height, GLuint buffer) {
		glReadBuffer(buffer);
		glBindTexture(GL_TEXTURE_2D, mId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}