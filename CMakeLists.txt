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

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)

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
