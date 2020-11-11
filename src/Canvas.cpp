#include "Canvas.hpp"

#include <string.h>  // be careful with this...
#include <assert.h>

#include <iostream>

namespace Im_Painter
{
	namespace {
	


	}
	
	
	Canvas::Canvas(canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		layers.push_back(Layer(height, width));
	}


	Canvas::Canvas(unsigned char *data, canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		layers.push_back(Layer(data, height, width));
	}


	Texture Canvas::to_texture() {
		// Note: this is dangerous, but should work!
		// Instead of memcopy, use appends
		assert(layers.size() > 0);
		unsigned int layer_size = height * width * 4;
		unsigned char data[layer_size * layers.size()];
		for (int i = 0; i < layers.size(); i++) {
			std::vector<unsigned char> &layer_data = layers[i].get_data();
			memcpy(data + i * layer_size, &layer_data[0], layer_data.size());
		}

		return Texture(width, height, layers.size(), data, RGBA, RGBA);
	}


	unsigned char *Canvas::get_write_data() {
		unsigned char *data = new unsigned char[height * width * 4];

		// This is hell!
		for (unsigned int i = 0; i < height * width; i++) {
			unsigned int index = 4 * i;

			for (unsigned int k = 0; k < layers.size(); k++) {
				// TODO: over operator and other things.
			}
		}

		return data;
	}


	canvas_num_layers_t Canvas::get_num_layers() {
		return this->layers.size();
	}


	unsigned char *Canvas::get_data() {
		// TODO: allocate data.
		return nullptr;
	}


	void Canvas::new_layer() {
		layers.push_back(Layer(height, width));
	}


	void Canvas::new_layer(unsigned char *data) {
		layers.push_back(Layer(data, height, width));
	}


	canvas_size_t Canvas::get_height() {
		return this->height;
	}


	canvas_size_t Canvas::get_width() {
		return this->width;
	}
} // namespace Im_Painter

