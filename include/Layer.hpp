#pragma once

#include <vector>

#include <Texture.hpp>


namespace Im_Painter {
	typedef unsigned int layer_size_t;

	/**
	 * Layer class.
	 * 
	 * All layers have a depth of 4.
	 */
	class Layer {
		private:
		bool visible;

		Texture *texture;


		public:
		/**
		 * Initializes a blank layer (i.e. memsets 0's).
		 */
		Layer(layer_size_t height, layer_size_t width);


		/**
		 * Layer class. Holds the data of an image.
		 */
		Layer(unsigned char *data, layer_size_t height, layer_size_t width);


		~Layer();


		/**
		 * Merges a layer into this layer. This calling layer is on top.
		 */
		void merge(Layer &to_merge);


		/**
		 * Update the layer's data.
		 */
		void update(unsigned char *data);


		/**
		 * Gets texture id associated with this layer.
		 */
		Texture_id_t get_texture_id();

		/**
		 * Takes this layer's data on the GPU and puts it into a buffer.
		 */
		void get_data(std::vector<unsigned char> &buffer);


		void bind();


		bool is_visible();


		void toggle_visible();
	};

} // namespace Im_Painter
