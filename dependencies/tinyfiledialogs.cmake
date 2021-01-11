add_library(
	tinyfiledialogs
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs/tinyfiledialogs.c
	${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs/tinyfiledialogs.h
)
target_include_directories(tinyfiledialogs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs)


set_target_properties(tinyfiledialogs PROPERTIES FOLDER "tinyfiledialogs")
