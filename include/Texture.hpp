#pragma once

#include <vector>


namespace Im_Painter {
	typedef unsigned int Texture_id_t;  // Generated by GL

	enum Texture_Format {  // These are the only ones we need
		RGB,
		RGBA
	};


	/**
	 * Texture class to pass to GL; in the shader, it will be a sampler2DArray.
	 */
	class Texture {
		private:
		Texture_id_t id;
		unsigned int height, width;
		unsigned int internal_format, image_format;
		std::vector<unsigned char> tex;

		public:
		/**
		 * Allocates the texture based on height & width.
		 * 
		 * @param width            Image width. Non-zero.
		 * @param height           Image height. Non-zero.
		 * @param layer_count      Number of layers. Nonzero.
		 * @param data             Image data. Must exist.
		 * @param internal_format  Internal format. Must be A Texture_Format enum
		 * @param image_format     Image format. Must be A Texture_Format enum
		 */
		Texture(unsigned int width,
				unsigned int height,
				unsigned int layer_count,
				unsigned char *data,
				Texture_Format internal_format,
				Texture_Format image_format);


		/**
		 * Deconstructor; de-allocates the texture in GL.
		 */
		~Texture();


		/**
		 * Update a texture with more data.
		 * 
		 * @param layer_count Number of layers
		 * @param data        Texture data
		 */
		void update(unsigned int layer_count, unsigned char *data);


		/**
		 * Returns id of this texture.
		 */
		Texture_id_t get_id();


		/**
		 * Bind texture to use.
		 */
		void bind();


		/**
		 * Unbinds a texture. Note that you could call the GL function to reset.
		 */
		void unbind();
	};
} // namespace Im_Painter
