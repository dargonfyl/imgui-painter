#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

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

	};
} // namespace Im_Painter
