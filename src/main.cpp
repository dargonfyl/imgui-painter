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


	if (key >= 0 && key < 1024) { // These are the standard keyboard keys
		if (action == GLFW_RELEASE) Im_Painter::UI::handle_keyboard(key);
	}
}

int x_mouse_position = 0;
int y_mouse_position = 0;
int num_updates = 0;
float last_frame = 0;
bool click = false;

void canvas_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	x_mouse_position = static_cast<int>(xpos);
	y_mouse_position = static_cast<int>(ypos);
	
	float current_frame = (float)glfwGetTime();

	// Frame debug
	// std::cout << current_frame - last_frame << std::endl;
	last_frame = current_frame;
}

void canvas_click_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		click = true;
		// std::cout << "Pressed" << std::endl;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		click = false;
		// std::cout << "Released" << std::endl;
	}
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
	glEnable(GL_BLEND);  // TODO: you may need to use premultiplied alpha
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

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

	// TODO: stuff here for customizability
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
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
	glfwSetCursorPosCallback(window, canvas_cursor_callback);
	glfwSetMouseButtonCallback(window, canvas_click_callback);

	init_imgui(window);

	// Compile shaders & link
	// Im_Painter::Shader *shader = new Im_Painter::Shader("../shaders/triangle.vs", "../shaders/triangle.fs");
	Im_Painter::Shader shader = Im_Painter::Shader("/Users/dennis/dev/imgui-painter/shaders/quad.vs", "/Users/dennis/dev/imgui-painter/shaders/quad_single.fs");

	Im_Painter::Canvas canvas = Im_Painter::Image_IO::canvas_from_image("/Users/dennis/dev/imgui-painter/data/canvas.jpg");

	// Renderer setup
	Im_Painter::Renderer *renderer = new Im_Painter::Renderer(shader);

	// Im_Painter::Canvas canvas = Im_Painter::Image_IO::canvas_from_image("../data/canvas.jpg");
	// Im_Painter::Image_IO::layer_from_image("../data/awesomeface.png", canvas);
	// Im_Painter::Image_IO::layer_from_image("../data/thunder.png", canvas);

	ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	bool show_demo_window = true;
	bool show_layers_window = true;

	Im_Painter::Brush brush = Im_Painter::Brush(10);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // TODO: Clear colour should not be white in the end.
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		if (click) {
			canvas.paint(brush, x_mouse_position, y_mouse_position);
		}

		canvas.update_canvas();
		renderer->render(canvas);

		// Im_Painter::Texture tex = canvas.to_texture();
		// renderer->render(tex, canvas.get_num_layers());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

        Im_Painter::UI::show_menu_toolbar_window(canvas);
		Im_Painter::UI::show_colours_window(brush);

		if (show_layers_window) {
			Im_Painter::UI::show_layers_window(canvas, show_layers_window);
		}

		Im_Painter::UI::show_brush_window(brush);

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

	delete renderer;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
