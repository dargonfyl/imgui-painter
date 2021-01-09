#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"


namespace Im_Painter {
	class Renderer {
		private:
		Shader shader;

		// TODO: This is ugly and bad practice. Fix this.
		Shader framebuffer_shader = Shader("/Users/dennis/dev/imgui-painter/shaders/quad.vs", "/Users/dennis/dev/imgui-painter/shaders/framebuffer.fs");
		unsigned int VAO, VBO, FBO, RBO;
		unsigned int texture_colour_buffer;

		public:
		Renderer(Shader &shader);


		~Renderer();


		void render(Canvas &canvas);

	};
} // namespace Im_Painter
