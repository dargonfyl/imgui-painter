#include "Canvas.hpp"

#include <string.h>  // be careful with this...
#include <assert.h>
#include <stdlib.h>

#include <iostream>

namespace Im_Painter
{
	namespace {
		// TODO: declare a bunch of factories here.


	}


	Canvas::Canvas(canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		active_layer_index = 0;
		active_layer_buffer = std::vector<unsigned char>(4 * height * width, static_cast<unsigned char>(0));
		dirty = false;
		layers.push_back(new Layer(height, width));
	}


	Canvas::Canvas(unsigned char *data, canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		active_layer_index = 0;
		active_layer_buffer = std::vector<unsigned char>(data, data + 4 * height * width);
		dirty = false;
		layers.push_back(new Layer(data, height, width));
	}


	Canvas::~Canvas() {
		for (Layer *layer : layers) {
			delete layer;
		}
	}


	void Canvas::update_texture() {
		// assert(layers.size() > 0);
		// if (layers.size() == 1) {
		// 	active_layer = std::vector<unsigned char>(layers[0]->get_data());
		// 	return;
		// }
		
		// int layer_size = height * width * 4;
		// assert(active_layer.size() == layer_size);
		// std::vector<unsigned char> &base_layer = layers[0]->get_data();
		// for (int j = 0; j < height * width; j++) {
		// 	// Update with alpha composite; final values are the values that are behind
		// 	int i = j * 4;
		// 	float final_r = base_layer[i] / 255.0f;
		// 	float final_g = base_layer[i + 1] / 255.0f;
		// 	float final_b = base_layer[i + 2] / 255.0f;
		// 	float final_a = base_layer[i + 3] / 255.0f;
		// 	for (int k = 1; k < layers.size(); k++) {
		// 		std::vector<unsigned char> &front_layer = layers[k]->get_data();
		// 		float front_a = front_layer[i + 3] / 255.0f;

		// 		float front_r = front_layer[i] / 255.0f * front_a;
		// 		float front_g = front_layer[i + 1] / 255.0f * front_a;
		// 		float front_b = front_layer[i + 2] / 255.0f * front_a;

		// 		float back_r = final_r * final_a;
		// 		float back_g = final_g * final_a;
		// 		float back_b = final_b * final_a;

		// 		final_r = front_r + back_r * (1.0 - front_a);
		// 		final_g = front_g + back_g * (1.0 - front_a);
		// 		final_b = front_b + back_b * (1.0 - front_a);
		// 		final_a = front_a + final_a * (1.0 - front_a);
		// 	}

		// 	active_layer[i] = static_cast<unsigned char>(final_r * 255.0f);
		// 	active_layer[i + 1] = static_cast<unsigned char>(final_g * 255.0f);
		// 	active_layer[i + 2] = static_cast<unsigned char>(final_b * 255.0f);
		// 	active_layer[i + 3] = static_cast<unsigned char>(final_a * 255.0f);
		// }
	}


	Texture Canvas::to_texture() {
		// TODO: refactor
		// Note: this is dangerous, but should work!
		// Instead of memcopy, use appends
		assert(layers.size() > 0);
		unsigned int layer_size = height * width * 4;
		unsigned char *data = (unsigned char *)malloc(layer_size * layers.size());
		// for (int i = 0; i < layers.size(); i++) {
		// 	std::vector<unsigned char> &layer_data = layers[i];
		// 	memcpy(data + i * layer_size, &layer_data[0], layer_data.size());
		// }

		// std::vector<unsigned char> &layer_data = layers[0];
		// unsigned char *data = &layer_data[0];
		// std::cout << static_cast<int>(layers[0].get_data()[0]) << std::endl;
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
		// TODO: allocate and write data.
		return nullptr;
	}


	void Canvas::new_layer() {
		layers.push_back(new Layer(height, width));
		std::cout << layers.size() << std::endl;
		// update_texture();
	}


	void Canvas::new_layer(unsigned char *data) {
		layers.push_back(new Layer(data, height, width));
		// update_texture();
	}


	void Canvas::update_canvas() {
		if (!dirty) return;

		assert(active_layer_index < layers.size());
		Layer *layer = layers[active_layer_index];

		layer->update(&active_layer_buffer[0]);


		dirty = false;
	}


	void Canvas::paint(Brush &brush, int x_mouse_pos, int y_mouse_pos) {
		if (x_mouse_pos >= width || x_mouse_pos < 0) return;
		if (y_mouse_pos >= height || y_mouse_pos < 0) return;

		int pos = 4 * (y_mouse_pos * width + x_mouse_pos);
		unsigned char *layer_location = &active_layer_buffer[pos];

		brush.use(active_layer_buffer, x_mouse_pos, y_mouse_pos, width, height);
		dirty = true;
	}


	void Canvas::bind(unsigned int layer_index) {
		assert(layer_index < layers.size());
		layers[layer_index]->bind();
	}


	void Canvas::switch_active_layer(layer_index_t layer_index) {
		assert(layer_index < layers.size());
		if (layer_index == active_layer_index) return;
		// if (layer_index >= layers.size()) return;

		active_layer_index = layer_index;
		layers[active_layer_index]->get_data(active_layer_buffer);
	}


	Texture_id_t Canvas::layer_texture_id(layer_index_t layer_index) {
		// assert(layer_index < layers.size());
		if (layer_index >= layers.size()) {
			return 0;
		}
		return layers[layer_index]->get_texture_id();
	}


	void Canvas::delete_layer(layer_index_t layer_index) {
		assert(layer_index < layers.size());

		// Must have at least 1 layer.
		if (layers.size() == 1) return;

		if (layer_index < active_layer_index) {
			active_layer_index -= 1;
		} else if (active_layer_index == layer_index) {
			if (active_layer_index == layers.size() - 1) {
				active_layer_index -= 1;
			}
		}
		layers.erase(layers.begin() + layer_index);

		layers[active_layer_index]->get_data(active_layer_buffer);
	}


	void Canvas::toggle_layer_visibility(layer_index_t layer_index) {
		assert(layer_index < layers.size());
		layers[layer_index]->toggle_visible();
	}


	bool Canvas::get_layer_visibility(layer_index_t layer_index) {
		assert(layer_index < layers.size());
		return layers[layer_index]->is_visible();
	}


	canvas_size_t Canvas::get_height() {
		return this->height;
	}


	canvas_size_t Canvas::get_width() {
		return this->width;
	}
} // namespace Im_Painter

