// #include "Layer.hpp"

// #include <string.h>  // memset


// namespace Im_Painter {
// 	Layer::Layer(layer_size_t height, layer_size_t width) {
// 		unsigned int total_bytes = 4 * height * width;
// 		unsigned char buf[total_bytes];
// 		memset(buf, static_cast<unsigned char>(128), total_bytes);
// 		this->data = std::vector<unsigned char>(buf, buf + total_bytes);
// 	}


// 	Layer::Layer(unsigned char *data, layer_size_t height, layer_size_t width) {
// 		unsigned int total_bytes = 4 * height * width;
// 		this->data = std::vector<unsigned char>(data, data + total_bytes);  // Iterator version, start to end (inclusive)
// 	}


// 	void Layer::merge(Layer to_merge) {

// 	}

// 	std::vector<unsigned char> &Layer::get_data() {
// 		return this->data;
// 	}
// } // namespace Im_Painter
