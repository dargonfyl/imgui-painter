#include "Image_IO.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>
#include <assert.h>
#include <exception>


namespace Im_Painter {
	// TODO: hard coded image resizes
	Canvas Image_IO::canvas_from_image(std::string path) {
		int width, height, num_channels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &num_channels, 4);

		assert(data && "Im_Painter::Image_IO::read: stbi_load returned no data");  // TODO: non-assert error check

		unsigned char *resized_data = new unsigned char[800 * 600 * 4];
		stbir_resize_uint8(data, width, height, 0, resized_data, 800, 600, 0, 4);

		Canvas canvas = Canvas(resized_data, 600, 800);
		stbi_image_free(data);
		stbi_image_free(resized_data);
		return canvas;
	}


	void Image_IO::layer_from_image(std::string path, Canvas &canvas) {
		int width, height, num_channels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);

		assert(data && "Im_Painter::Image_IO::read: stbi_load returned no data");

		unsigned char *resized_data = new unsigned char[800 * 600 * 4];
		stbir_resize_uint8(data, width, height, 0, resized_data, 800, 600, 0, 4);

		canvas.new_layer(resized_data);
		stbi_image_free(data);
		stbi_image_free(resized_data);
	}


	// Texture Image_IO::read(const char *path, bool alpha) {
	// 	int width, height, num_channels;
	// 	unsigned char *data = stbi_load(path, &width, &height, &num_channels, 0);  // STB mallocs the data

	// 	assert(data && "Im_Painter::Image_IO::read: stbi_load returned no data");

	// 	Texture_Format format = alpha ? RGBA : RGB;
	// 	Texture tex = Texture(width, height, 1, data, format, format);
	// 	free(data);
	// 	return tex;
	// }


	void Image_IO::write(std::string path, Canvas &canvas) {
		canvas_size_t height = canvas.get_height();
		canvas_size_t width = canvas.get_width();
		
		// How the hell do I get the canvas data?
		unsigned char *data = canvas.get_data();

		int write_result = stbi_write_png(path.c_str(), width, height, 4, data, 4);
		delete[] data;

		assert(write_result == 0 && "Image write failed");
		// if (write_result != 0) {
		// 	exit(EXIT_FAILURE);
		// }
	}
}
