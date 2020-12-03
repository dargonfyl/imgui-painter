#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colour_buffer, 0);

		glGenRenderbuffers(1, &this->RBO);
		assert(RBO != 0);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
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


	void Renderer::render(Texture &texture, unsigned int num_layers) {
		this->shader.activate();  // TODO: clean this shit up
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(600.0f, 800.0f, 1.0f));  // (height, width, z-scale)
		shader.set_mat4("u_model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("u_projection", projection);

		shader.set_int("u_num_layers", num_layers);

		assert(this->VAO != 0 && "VAO == 0 when trying to draw sprite");
		glBindVertexArray(this->VAO);

		// shader.set_int("u_sprite", GL_TEXTURE0);

		glActiveTexture(GL_TEXTURE0);
		texture.bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);
	}


	void Renderer::render(Canvas &canvas) {
		// TODO: framebuffer operations on OSX.
		// glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// // glEnable(GL_DEPTH_TEST);
		// glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


		this->shader.activate();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(600.0f, 800.0f, 1.0f));  // (height, width, z-scale)
		shader.set_mat4("u_model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("u_projection", projection);

		shader.set_bool("u_premultiply", true);

		assert(this->VAO != 0 && "VAO == 0 when trying to draw canvas");
		glBindVertexArray(this->VAO);

		glActiveTexture(GL_TEXTURE0);
		unsigned int i;
		float model_offset = 0.0f;
		for (i = 0; i < canvas.get_num_layers(); i++) {
			canvas.bind(i);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// glDisable(GL_DEPTH_TEST);
		// // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);

		// shader.set_bool("u_premultiply", false);

		// glBindTexture(GL_TEXTURE_2D, texture_colour_buffer);
		// glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);
	}
}
