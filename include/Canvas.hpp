#pragma once

#include <vector>

#include "Layer.hpp"
#include "Texture.hpp"
#include "Brush.hpp"


// TODO: define max dimensions for Canvas

namespace Im_Painter
{
	typedef unsigned int canvas_size_t;       // For canvas height/widht
	typedef unsigned int canvas_num_layers_t; // For number of layers in canvas
	typedef unsigned int active_layer_t;      // TODO

	class Canvas {
		private:
		canvas_size_t height, width;

		std::vector<Layer> layers;  // Bottom layers are at the start of the list
		std::vector<unsigned char> canvas_buffer;  // must have dimensions of the canvas

		Brush brush;

		void update_texture();


		public:
		/**
		 * Constructor. Creates a Canvas with a single blank layer. Note that this base layer is protected; you can't delete it.
		 */
		Canvas(canvas_size_t height, canvas_size_t width);


		/**
		 * Constructor. Creates a Canvas with a single layer.
		 * 
		 * You are free to delete the data pointer afterwards, since Layers have their own copies of the data.
		 */
		Canvas(unsigned char *data, canvas_size_t height, canvas_size_t width);


		~Canvas();


		/**
		 * Creates an array texture for GL to render.
		 */
		Texture to_texture(); // TODO: this might be very slow as # layers increases... how can we fix that?


		unsigned char *get_write_data();  // TODO: this function is rather silly and very unsafe. Needs rethinking.


		/**
		 * Returns number of layers
		 */
		canvas_num_layers_t get_num_layers();


		/**
		 * Gets canvas' image data, i.e. the output image, on the heap.
		 * 
		 * @return unsigned char * of the entire canvas data
		 */
		unsigned char *get_data();


		/**
		 * New blank layer.
		 */
		void new_layer();


		/**
		 * New layer from image layer.
		 * 
		 * @param data Image data. Must be 4 * height * width bytes in size.
		 */
		void new_layer(unsigned char *data);


		void paint(int x_mouse_pos, int y_mouse_pos);


		void set_brush_color(glm::vec4 color);


		void bind(unsigned int layer_index);


		canvas_size_t get_height();


		canvas_size_t get_width();
	};
} // namespace Im_Painter
