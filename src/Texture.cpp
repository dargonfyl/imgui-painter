#include "Texture.hpp"

#include <glad/glad.h>
#include <assert.h>


namespace Im_Painter
{
	Texture::Texture(unsigned int width, unsigned int height, unsigned char *data, Texture_Format internal_format, Texture_Format image_format) {
		assert(width > 0 && "Im_Painter::Texture::constructor: width is 0 ");
		assert(height > 0 && "Im_Painter::Texture::constructor: height is 0 ");
		assert(data && "Im_Painter::Texture::constructor: data is empty");

		glGenTextures(1, &this->id);
		assert(this->id != 0 && "Im_Painter::Texture::constructor: glGenTextures set id to 0");

		this->width = width;
		this->height = height;

		assert(internal_format == RGBA || internal_format == RGB);
		assert(image_format == RGBA || image_format == RGB);

		this->interal_format = internal_format == RGB ? GL_RGB : GL_RGBA;
		this->image_format = image_format == RGB ? GL_RGB : GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexImage2D(GL_TEXTURE_2D, 0, this->interal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);  // TODO: do we even need this?

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // TODO: this stuff
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	Texture::~Texture() {
		glDeleteTextures(1, &this->id);
	}


	void Texture::update(unsigned char *data) {
		assert(this->id != 0 && "Im_Painter::Texture::update: id is 0");
		glBindTexture(GL_TEXTURE_2D, this->id);

		glTexImage2D(GL_TEXTURE_2D, 0, this->interal_format, this->width, this->height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	Texture_id_t Texture::get_id() {
		return this->id;
	}


	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
} // namespace Im_Painter

