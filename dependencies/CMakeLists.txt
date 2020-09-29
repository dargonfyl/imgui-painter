# GLAD
add_subdirectory(glad-3.3/)


# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # These prevent GLFW from building unecessary stuff
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw/)


# GLM
set(GLM_TEST_ENABLE      OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS    OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS    OFF CACHE INTERNAL "")
add_subdirectory(glm/)


# IMGUI
include(imgui.cmake)


# stb
include(stb.cmake)
