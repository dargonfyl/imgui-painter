#include "Image_IO.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
// #include <stb_image_resize.h>  // IDK if I'll ever use this one.
#include <assert.h>
#include <exception>


namespace Im_Painter {
	Texture Image_IO::read(std::string path, bool alpha) {
		int width, height, num_channels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);

		assert(data && "Im_Painter::Image_IO::read: stbi_load returned no data");

		Texture_Format format = alpha ? RGBA : RGB;

		return Texture(width, height, data, format, format);
	}


	Texture Image_IO::read(const char *path, bool alpha) {  // IDK about this one, doesn't seem too useful.
		int width, height, num_channels;
		unsigned char *data = stbi_load(path, &width, &height, &num_channels, 0);  // STB mallocs the data

		assert(data && "Im_Painter::Image_IO::read: stbi_load returned no data");

		Texture_Format format = alpha ? RGBA : RGB;

		Texture tex = Texture(width, height, data, format, format);
		free(data);
		return tex;
	}


	void Image_IO::write(std::string path) {
		throw std::exception();
	}
}
