add_library(
	stb
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/stb/stb_image.h  # we are only using image functionality
	${CMAKE_CURRENT_LIST_DIR}/stb/stb_image_write.h
)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stb)
set_target_properties(stb PROPERTIES FOLDER "stb")
