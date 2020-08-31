#include "main.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/**
 * Resizes viewport should the window resize.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

/**
 * Key callback for a GLFW window.
 */
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// Exit key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


/**
 * Initializes a GLFW window & sets GLFW/GL states.
 */
GLFWwindow *init_window() {
	// INITIALIZATION
	if (!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	#ifndef _WIN32
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No idea why, but this breaks everything on Windows 7
	#endif

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	glfwWindowHint(GLFW_RESIZABLE, false);

	// Creates window
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world!", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Disables cursor

	// GLAD initialize; we must do this before all OpenGL lib functions
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	// GL config; these don't change much
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);  // Blend for alpha things
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}


/**
 * Read a shader program from a file path.
 */
std::string read_shader(const char *path) {
	try {
		std::ifstream filestream(path);
		std::stringstream codestream;

		codestream << filestream.rdbuf();

		filestream.close();

		return codestream.str();
	} catch (std::exception e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		exit(EXIT_FAILURE);
	}
}


void check_shader_error(GLuint shader, std::string extra_message) {
	int success;
	char err_message[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, err_message);
		std::cerr << extra_message << err_message << std::endl;
		exit(EXIT_FAILURE);
	}
}


int main() {
	GLFWwindow *window = init_window();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
	};

	// Set up buffers
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// Compile shaders & link
	std::string vertex_code = read_shader("../shaders/triangle.vs");
	std::string fragment_code = read_shader("../shaders/triangle.fs");

	const char *cvertex_code = vertex_code.c_str();  // Note: doing std::stringstream::str().c_str() causes funny problems in linking
	const char *cfragment_code = fragment_code.c_str();
	GLuint vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &cvertex_code, 0);
	glCompileShader(vs);
	check_shader_error(vs, "Vertex shader:\n");

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &cfragment_code, 0);
	glCompileShader(fs);
	check_shader_error(fs, "Fragment shader:\n");

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);

	// Check for shader program errors
	int success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		char err_message[512];
		glGetProgramInfoLog(shader_program, 512, NULL, err_message);
		std::cerr << "Error: Program failed to link\n" << err_message << std::endl;
		exit(EXIT_FAILURE);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return 0;
}
