#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string.h>
#include <stb_image_resize.h>

#ifdef __APPLE__
// There is no preprocessor ID for macos retina displays, so this is the next best thing
#define FRAMEBUFFER_X 800 * 2
#define FRAMEBUFFER_Y 600 * 2
#else
#define FRAMEBUFFER_X 800
#define FRAMEBUFFER_Y 600
#endif


// Functions for GL pipeline
namespace {
	// TODO: error checks
	int generate_framebuffer(unsigned int &FBO, unsigned int &texture_colour_buffer, unsigned int &RBO) {
		glGenFramebuffers(1, &FBO);
		assert(FBO != 0);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &texture_colour_buffer);
		assert(texture_colour_buffer != 0);
		glBindTexture(GL_TEXTURE_2D, texture_colour_buffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FRAMEBUFFER_X, FRAMEBUFFER_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colour_buffer, 0);

		glGenRenderbuffers(1, &RBO);
		assert(RBO != 0);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FRAMEBUFFER_X, FRAMEBUFFER_Y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return 0;
	}
} // namespace


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

		generate_framebuffer(FBO, texture_colour_buffer, RBO);
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

		assert(this->VAO != 0 && "VAO == 0 when trying to draw canvas");
		glBindVertexArray(this->VAO);

		glActiveTexture(GL_TEXTURE0);
		unsigned int i;
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

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);
	}


	unsigned char *Renderer::get_hsv_filtered_layer_buffer(Canvas &canvas) {
		unsigned int hsv_FBO, hsv_tex, hsv_RBO;

		generate_framebuffer(hsv_FBO, hsv_tex, hsv_RBO);

		assert(hsv_RBO && hsv_tex && hsv_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hsv_FBO);

		shader.activate();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(600.0f, 800.0f, 1.0f));  // (height, width, z-scale)
		shader.set_mat4("u_model", model);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		shader.set_mat4("u_projection", projection);

		glBindVertexArray(VAO);

		shader.set_float("u_hue_shift", canvas.get_hue_shift());
		shader.set_float("u_saturation_shift", canvas.get_saturation_shift());
		shader.set_float("u_value_shift", canvas.get_value_shift());
		shader.set_bool("u_premultiply", false);

		canvas.bind(canvas.get_active_layer_index());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __APPLE__
		size_t im_size = sizeof(unsigned char) * 600 * 800 * 4 * 2 * 2;
#else
		size_t im_size = sizeof(unsigned char) * 600 * 800 * 4;
#endif
		unsigned char *data = (unsigned char *)malloc(im_size);
		glBindTexture(GL_TEXTURE_2D, hsv_tex);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void *)data);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glActiveTexture(0);
		glUseProgram(0);

		glDeleteFramebuffers(1, &hsv_FBO);
		glDeleteTextures(1, &hsv_tex);
		glDeleteRenderbuffers(1, &hsv_RBO);

		// If macOS, resize for proper dimensions
		#ifdef __APPLE__
		unsigned char *resized_data = (unsigned char *)malloc(sizeof(unsigned char) * 600 * 800 * 4);
		int result = stbir_resize_uint8(data, 2 * 800, 2 * 600, 0, resized_data, 800, 600, 0, 4);
		delete[] data;
		assert(result);
		assert(resized_data);

		// Data will be upside down. Flip so that it's correct.
		unsigned int width = 800;
		unsigned int height = 600;
		for (unsigned int w = 0; w < width; w++) {
			for (unsigned int h = 0; h < height / 2; h++) {
				unsigned int old_location = 4 * (h * width + w);
				unsigned int new_location = 4 * ((height - h - 1) * width + w);

				// Swap
				unsigned char old_location_channels[4] = {
					resized_data[old_location],
					resized_data[old_location + 1],
					resized_data[old_location + 2],
					resized_data[old_location + 3],
				};
				for (int k = 0; k < 4; k++) {
					resized_data[old_location + k] = resized_data[new_location + k];
					resized_data[new_location + k] = old_location_channels[k];
				}
			}
		}

		return resized_data;
		#else
		return data;
		#endif
	}
}
