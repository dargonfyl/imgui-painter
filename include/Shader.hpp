#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>

namespace Im_Painter {
	/**
	 * Shader class. Handles all shader functionality, including uniform setting. 
	 */
	class Shader {
		private:
		GLuint id;  // Equal to the shader's program id in OpenGL. These are set by OpenGL, so there is no order whatsoever in them.


		public:
		/**
		 * Default constructor. Sets everything to 0, and cannot be used.
		 */
		Shader();


		/**
		 * Constructor for Shader class. Takes in paths to the Shaders.
		 * 
		 * @param vertex_path
		 * @param fragment_path
		 */
		Shader(std::string vertex_path, std::string fragment_path);


		/**
		 * Deconstructor. Deletes the associated shader program.
		 */
		~Shader();


		/**
		 * Activates a shader for use.
		 */
		void activate();

		/**
		 * 
		 */
		void set_int(const std::string &name, int value);


		/**
		 * Sets a uniform for a float.
		 * 
		 * @param name
		 * @param value
		 */
		void set_float(const std::string &name, float value);


		/**
		 * 
		 */
		void set_bool(const std::string &name, bool value);


		/**
		 * 
		 */
		void set_vec2(const std::string & name, glm::vec2 value);


		/**
		 * Sets a vec3 uniform.
		 * 
		 * @param name
		 * @param value
		 */
		void set_vec3(const std::string &name, glm::vec3 value);


		/**
		 * 
		 */
		void set_vec4(const std::string &name, glm::vec4 value);


		/**
		 * Sets a mat4 uniform.
		 * 
		 * @param name
		 * @param value
		 */
		void set_mat4(const std::string &name, glm::mat4 value);
	};
}  // namespace Im_Painter
