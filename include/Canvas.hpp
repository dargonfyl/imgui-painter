#pragma once

#include "Layer.hpp"
#include "Texture.hpp"
#include <vector>

namespace Im_Painter
{
	typedef unsigned int canvas_size_t;       // For canvas height/widht
	typedef unsigned int canvas_num_layers_t; // For number of layers in canvas


	class Canvas {
		private:
		canvas_size_t height, width;

		std::vector<Layer> layers;


		public:
		/**
		 * Constructor. Creates a Canvas with a single blank layer. Note that this base layer is protected; you can't delete it.
		 */
		Canvas(canvas_size_t height, canvas_size_t width);


		/**
		 * Constructor. Creates a Canvas with a single layer.
		 * I really, really, really, really hope you've malloc'd your data, or this is going to fail spectacularly.
		 */
		Canvas(unsigned char *data, canvas_size_t height, canvas_size_t width);


		/**
		 * Creates an array texture for GL to render.
		 */
		Texture to_texture(); // TODO: this might be very slow as # layers increases... how can we fix that?


		canvas_num_layers_t get_num_layers();


		/**
		 * New blank layer.
		 */
		void new_layer();


		/**
		 * New layer from image layer.
		 */
		void new_layer(unsigned char *data, canvas_size_t height, canvas_size_t width);
	};
} // namespace Im_Painter
