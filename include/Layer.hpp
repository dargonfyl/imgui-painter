#pragma once

#include <vector>


namespace Im_Painter {
	typedef unsigned int layer_size_t;

	/**
	 * Layer class.
	 * 
	 * All layers have a depth of 4.
	 */
	class Layer {
		private:
		std::vector<unsigned char> data;


		public:
		/**
		 * Initializes a blank layer (i.e. memsets 0's).
		 */
		Layer(layer_size_t height, layer_size_t width);


		/**
		 * Layer class. Holds the data of an image.
		 */
		Layer(unsigned char *data, layer_size_t height, layer_size_t width);


		/**
		 * Merges a layer into this layer. This calling layer is on top.
		 */
		void merge(Layer to_merge);


		std::vector<unsigned char> &get_data();
	};

} // namespace Im_Painter
