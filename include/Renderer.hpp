#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"


namespace Im_Painter {
	class Renderer {
		private:
		Shader shader;  // Shader ojects are incredibly small, so no worries about copy constructors.

		// TODO: This is ugly and probably bad practice. Fix this.
		Shader framebuffer_shader = Shader("/Users/dennis/dev/imgui-painter/shaders/quad.vs", "/Users/dennis/dev/imgui-painter/shaders/framebuffer.fs");
		unsigned int VAO, VBO, FBO, RBO;
		unsigned int texture_colour_buffer;

		public:
		Renderer(Shader &shader);


		~Renderer();


		void render(Canvas &canvas);


		/**
		 * Get data from the active layer that has presumably been HSV-filtered.
		 * 
		 * @param Canvas
		 */
		unsigned char *get_hsv_filtered_layer_buffer(Canvas &canvas);


		/**
		 * Gets data from framebuffer object to export.
		 * 
		 * A note that this will include whatever conditions are applied to the layers (i.e. not visible) in the final product.
		 * 
		 * @param Canvas
		 */
		unsigned char *get_data_to_export(Canvas &canvas);
	};
} // namespace Im_Painter
