#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"


namespace Im_Painter {
	class Renderer {
		private:
		Shader shader;
		unsigned int VAO, VBO;


		public:
		Renderer(Shader &shader);


		Renderer();


		~Renderer();


		void render();


		void render(Texture &texture, unsigned int num_layers);


		void render(Canvas &canvas);

	};
} // namespace Im_Painter
