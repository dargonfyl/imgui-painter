#pragma once

#include <vector>

#include "Layer.hpp"
#include "Texture.hpp"
#include "Brush.hpp"


// TODO: define max dimensions for Canvas
// TODO: iterator for layers
namespace Im_Painter
{
	typedef int canvas_size_t;                 // For canvas height/widht
	typedef unsigned int canvas_num_layers_t;  // For number of layers in canvas
	typedef unsigned int layer_index_t;        // Index into the layers. Should be changed to a pointer.

	class Canvas {
		private:
		canvas_size_t height, width;

		std::vector<Layer *> layers;              // Bottom layers are at the start of the list
		std::vector<unsigned char> active_layer_buffer;
		layer_index_t active_layer_index;
		bool dirty;


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
		Texture to_texture();


		unsigned char *get_write_data();


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


		void update_canvas();


		void paint(Brush &brush, int x_mouse_pos, int y_mouse_pos);


		void bind(unsigned int layer_index);


		void switch_active_layer(layer_index_t layer_index);


		Texture_id_t layer_texture_id(layer_index_t layer_index);


		void delete_layer(layer_index_t layer_index);


		void toggle_layer_visibility(layer_index_t layer_index);


		bool get_layer_visibility(layer_index_t layer_index);


		layer_index_t get_active_layer_index();


		canvas_size_t get_height();


		canvas_size_t get_width();
	};
} // namespace Im_Painter
