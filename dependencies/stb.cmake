# credit to http://mariobadr.com/creating-a-header-only-library-with-cmake.html

add_library(
	stb
	INTERFACE
)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb)
