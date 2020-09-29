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

	const char *glsl_version = "#version 330 core";

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
 * Initializes imgui functionality
 */
void init_imgui(GLFWwindow *window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable some nice imgui features
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
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

	init_imgui(window);

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
	};

	// Compile shaders & link
	// Im_Painter::Shader *shader = new Im_Painter::Shader("../shaders/triangle.vs", "../shaders/triangle.fs");
	Im_Painter::Shader shader = Im_Painter::Shader("../shaders/quad.vs", "../shaders/quad.fs");

	// Renderer setup
	Im_Painter::Renderer *renderer = new Im_Painter::Renderer(shader);

	Im_Painter::Texture tex = Im_Painter::Image_IO::read("../data/awesomeface.png", true);

	ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	bool show_demo_window = true;
	bool show_another_window = false;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer->render(tex);

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
