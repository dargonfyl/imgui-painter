#pragma once


#include <Shader.hpp>


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
	};
} // namespace Im_Painter
