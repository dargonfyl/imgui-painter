#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"


namespace Im_Painter {
	class Renderer {
		private:
		Shader shader;
		unsigned int VAO, VBO, FBO, RBO;
		unsigned int texture_colour_buffer;


		public:
		Renderer(Shader &shader);


		~Renderer();


		void render(Texture &texture, unsigned int num_layers);


		void render(Canvas &canvas);

	};
} // namespace Im_Painter
