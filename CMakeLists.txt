cmake_minimum_required(VERSION 3.0)
project(ggg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add project files
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# User files
# Add source files
file(GLOB_RECURSE SRCFILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp
)

# TODO: Put the imgui stuff into some sort of wrapper (like imgui-cmake), rather than directly adding it to src.
# file(GLOB IMGUI_SRCFILES ${CMAKE_SOURCE_DIR}/dependencies/imgui/*.cpp)

# file(GLOB IMGUI_HEADER_FILES
# 	${CMAKE_SOURCE_DIR}/dependencies/imgui/*.h
# )

# set(IMGUI_IMPL
# 	${CMAKE_SOURCE_DIR}/dependencies/imgui/examples/imgui_impl_glfw.h
# 	${CMAKE_SOURCE_DIR}/dependencies/imgui/examples/imgui_impl_glfw.cpp
# 	${CMAKE_SOURCE_DIR}/dependencies/imgui/examples/imgui_impl_opengl3.cpp
# 	${CMAKE_SOURCE_DIR}/dependencies/imgui/examples/imgui_impl_opengl3.h
# )

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	# "${CMAKE_SOURCE_DIR}/dependencies/imgui/"
	# "${CMAKE_SOURCE_DIR}/dependencies/imgui/examples/"
)

# add_executable(main ${SRCFILES} ${HEADER_FILES} ${IMGUI_SRCFILES} ${IMGUI_HEADER_FILES} ${IMGUI_IMPL})
add_executable(main ${SRCFILES} ${HEADER_FILES})


# OpenGL
find_package(OpenGL REQUIRED)
if(WIN32)
	target_link_libraries(main opengl32)
elseif(APPLE)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(main ${OPENGL_LIBRARIES})
endif()


# Include dependencies
add_subdirectory(dependencies/)
target_link_libraries(main glad glm glfw imgui)
