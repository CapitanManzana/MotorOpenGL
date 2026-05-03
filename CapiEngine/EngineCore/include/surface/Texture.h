#ifndef _H_Texture_H_
#define _H_Texture_H_
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>

namespace cme {
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		Texture(const Texture& tex) = delete;            // no copy constructor
		Texture& operator=(const Texture& tex) = delete; // no copy assignment

		void load(const std::string& filename,
			GLubyte alpha = 255); // load from file and upload to GPU

		void bind();
		void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };

		GLuint width() const { return mWidth; };
		GLuint height() const { return mHeight; };

		void setWrap(GLuint wp); // GL_REPEAT, GL_CLAMP_TO_EDGE, ...

		void loadColorBuffer(GLsizei width, GLsizei height, GLuint buffer = GL_FRONT);

		GLuint id() { return mId; }
		std::string name() { return _name; }
		void setName(std::string n) { _name = n; }
	protected:
		void init();

		int mWidth = 0;
		int mHeight = 0;
		GLuint mId = 0;

		std::string _name;

		unsigned char* _image = nullptr;
	};
}
#endif //_H_Texture_H_
