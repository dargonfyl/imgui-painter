add_library(
	imgui
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_opengl3.h  # Not sure if you need to include these here
	${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_glfw.h
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui/examples)
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)  # Sometimes, MSVC doesn't have correct preprocessor definitions

# GLAD and GLFW should have already been found, otherwise the impl files won't work.
target_link_libraries(imgui glfw)
target_link_libraries(imgui glad)

set_target_properties(imgui PROPERTIES FOLDER "imgui")
