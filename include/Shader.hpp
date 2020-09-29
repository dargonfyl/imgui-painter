#pragma once

#include <string>
#include <glm/glm.hpp>


namespace Im_Painter {
	typedef unsigned int Shader_id_t;

	/**
	 * Shader class. Handles all shader functionality, including uniform setting. 
	 */
	class Shader {
		private:
		Shader_id_t id;  // Equal to the shader's program id in OpenGL. These are set by OpenGL, so there is no order whatsoever in them.


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
		 * 
		 */
		Shader_id_t get_id();


		/**
		 * Activates a shader for use.
		 */
		void activate();

		/**
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_int(const std::string &name, int value);


		/**
		 * Sets a uniform for a float.
		 * 
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_float(const std::string &name, float value);


		/**
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_bool(const std::string &name, bool value);


		/**
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_vec2(const std::string & name, glm::vec2 value);


		/**
		 * Sets a vec3 uniform.
		 * 
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_vec3(const std::string &name, glm::vec3 value);


		/**
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_vec4(const std::string &name, glm::vec4 value);


		/**
		 * Sets a mat4 uniform.
		 * 
		 * @param name  Uniform name
		 * @param value Uniform value
		 */
		void set_mat4(const std::string &name, glm::mat4 value);
	};
}  // namespace Im_Painter
