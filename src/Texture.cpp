#include "Texture.hpp"

#include <glad/glad.h>
#include <assert.h>
#include <iostream>

namespace Im_Painter
{
	Texture::Texture(unsigned int width,
					 unsigned int height,
					 unsigned int layer_count,
					 unsigned char *data,
					 Texture_Format internal_format,
					 Texture_Format image_format) {
		assert(width > 0 && "Im_Painter::Texture::constructor: width is 0 ");
		assert(height > 0 && "Im_Painter::Texture::constructor: height is 0 ");
		assert(data && "Im_Painter::Texture::constructor: data is empty");
		assert(layer_count > 0 && "Im_Painter::Texture::constructor: layer_count is 0.");

		glGenTextures(1, &this->id);
		assert(this->id != 0 && "Im_Painter::Texture::constructor: glGenTextures set id to 0");

		this->width = width;
		this->height = height;

		assert(internal_format == RGBA || internal_format == RGB);
		assert(image_format == RGBA || image_format == RGB);

		this->internal_format = internal_format == RGB ? GL_RGB : GL_RGBA;
		this->image_format = image_format == RGB ? GL_RGB : GL_RGBA;

		assert(this->internal_format == GL_RGB || this->internal_format == GL_RGBA);
		assert(this->image_format == GL_RGB || this->image_format == GL_RGBA);
		// Texture 3D
		// glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

		// glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->internal_format, width, height, layer_count, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		// glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);

		// glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		// Texture 2D
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		// TODO: we may need set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	Texture::~Texture() {
		glDeleteTextures(1, &this->id);
	}


	void Texture::update(unsigned int layer_count, unsigned char *data) {
		assert(id != 0 && "Im_Painter::Texture::update: id is 0");
		assert(data && "Data does not exist");

		// glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		// glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, width, height, layer_count, 0, image_format, GL_UNSIGNED_BYTE, data);
		// glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);

		// glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		// Texture 2D
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	Texture_id_t Texture::get_id() {
		return this->id;
	}


	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, this->id);
	}


	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	unsigned char *Texture::get_texture_data() {
		int num_channels = internal_format == GL_RGBA ? 4 : 3;
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * num_channels);
		glBindTexture(GL_TEXTURE_2D, this->id);
		glGetTexImage(GL_TEXTURE_2D, 0, internal_format, GL_UNSIGNED_BYTE, (void *)data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return data;
	}
} // namespace Im_Painter

