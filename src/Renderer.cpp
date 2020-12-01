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
	}


	Renderer::~Renderer() {
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
	}


	void Renderer::render() {
		this->shader.activate();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.20f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
		shader.set_mat4("model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("projection", projection);

		shader.set_vec4("u_colour", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));  // TODO: ughhhhhh fix uniform names

		assert(this->VAO != 0 && "VAO == 0 when trying to draw sprite");
		glBindVertexArray(this->VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
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
		this->shader.activate();  // TODO: clean this shit up
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(600.0f, 800.0f, 1.0f));  // (height, width, z-scale)
		shader.set_mat4("u_model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("u_projection", projection);

		// shader.set_int("u_num_layers", canvas.get_num_layers());

		assert(this->VAO != 0 && "VAO == 0 when trying to draw sprite");
		glBindVertexArray(this->VAO);

		// shader.set_int("u_sprite", GL_TEXTURE0);

		glActiveTexture(GL_TEXTURE0);

		for (unsigned int i = 0; i < canvas.get_num_layers(); i++) {
			canvas.bind(i);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);
	}
}
