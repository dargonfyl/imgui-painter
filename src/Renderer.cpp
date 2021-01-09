#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __APPLE__
#define FRAMEBUFFER_X 800 * 2
#define FRAMEBUFFER_Y 600 * 2
#else
#define FRAMEBUFFER_X 800
#define FRAMEBUFFER_Y 600
#endif


namespace Im_Painter {
	Renderer::Renderer(Shader &shader) {
		this->shader = shader;
		assert(shader.get_id() != 0 && "Renderer::constructor: shader id is 0!");

		// Deal with render data
		float layer[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 
		
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &this->VAO);
		assert(this->VAO != 0 && "Renderer::constructor: glGenVertexArrays returned 0");  // TODO: non-assert gl checks
		glBindVertexArray(VAO);

		glGenBuffers(1, &this->VBO);
		assert(this->VAO != 0 && "Renderer::constructor: glGenBuffers returned 0");
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(layer), layer, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(2 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenFramebuffers(1, &this->FBO);
		assert(FBO != 0);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &this->texture_colour_buffer);
		assert(texture_colour_buffer != 0);
		glBindTexture(GL_TEXTURE_2D, texture_colour_buffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FRAMEBUFFER_X, FRAMEBUFFER_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colour_buffer, 0);

		glGenRenderbuffers(1, &this->RBO);
		assert(RBO != 0);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FRAMEBUFFER_X, FRAMEBUFFER_Y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	Renderer::~Renderer() {
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteFramebuffers(1, &this->FBO);
		glDeleteRenderbuffers(1, &this->RBO);
	}


	void Renderer::render(Canvas &canvas) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		this->shader.activate();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(600.0f, 800.0f, 1.0f));  // (height, width, z-scale)
		shader.set_mat4("u_model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("u_projection", projection);

		shader.set_bool("u_premultiply", true);

		shader.set_float("u_hue_shift", canvas.get_hue_shift());
		shader.set_float("u_saturation_shift", canvas.get_saturation_shift());
		shader.set_float("u_value_shift", canvas.get_value_shift());

		assert(this->VAO != 0 && "VAO == 0 when trying to draw canvas");
		glBindVertexArray(this->VAO);

		glActiveTexture(GL_TEXTURE0);
		unsigned int i;
		float model_offset = 0.0f;
		for (i = 0; i < canvas.get_num_layers(); i++) {
			if (!canvas.get_layer_visibility(i)) continue;

			if (i == canvas.get_active_layer_index()) {
				shader.set_float("u_hue_shift", canvas.get_hue_shift());
				shader.set_float("u_saturation_shift", canvas.get_saturation_shift());
				shader.set_float("u_value_shift", canvas.get_value_shift());
			} else {
				shader.set_float("u_hue_shift", 0.0f);
				shader.set_float("u_saturation_shift", 0.0f);
				shader.set_float("u_value_shift", 0.0f);
			}
			canvas.bind(i);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);

		framebuffer_shader.activate();

		framebuffer_shader.set_mat4("u_model", model);
		framebuffer_shader.set_mat4("u_projection", projection);

		glBindTexture(GL_TEXTURE_2D, texture_colour_buffer);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);
	}
}
