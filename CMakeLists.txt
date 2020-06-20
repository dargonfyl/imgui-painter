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


# Dependencies
# TODO: a CMakeLists.txt for /dependencies? It would make this part a lot less ugly
# OpenGL
find_package(OpenGL REQUIRED)
if(WIN32)
	target_link_libraries(main opengl32)
elseif(APPLE)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(main ${OPENGL_LIBRARIES})
endif()


# GLAD
add_subdirectory(dependencies/glad-3.3/)
target_link_libraries(main glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # These prevent GLFW from building unecessary stuff
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glfw)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/include)
target_link_libraries(main glfw)


# GLM
set(GLM_TEST_ENABLE      OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS    OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS    OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glm)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glm/glm)
target_link_libraries(main glm)
