cmake_minimum_required(VERSION 3.13)

project(
	06_model_load
)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR
	"./glfw-3.3.8"
)

set(GLAD_DIR
	"./glad"
)

set(GLM_DIR
	"./glm"
)

set(ASSIMP_DIR
	"./assimp"
)

set(GLAD_SRC
	${GLAD_DIR}/src/glad.c
)

set(SOURCE_FILES
	${GLAD_SRC}
	source/assimp_loader.cpp
	source/camera.cpp
	source/file_loader.cpp
	source/main.cpp
	source/material.cpp
	source/mesh.cpp
	source/model.cpp
	source/move.cpp
	source/shader.cpp
	source/stb_image.c
	source/texture.cpp
	source/vertex_buffer.cpp
)

add_executable(06_model_load
	${SOURCE_FILES}
)

add_subdirectory(
	${GLFW_DIR}
)

add_subdirectory(
	${ASSIMP_DIR}
)

target_include_directories(06_model_load
	PUBLIC
    include
	${GLM_DIR}
	${GLFW_DIR}/include
	${GLAD_DIR}/include
	${ASSIMP_DIR}/include
)

target_link_libraries(06_model_load
	glfw
	assimp
)

target_compile_definitions(06_model_load
	PRIVATE
    ROOT_PATH="${CMAKE_SOURCE_DIR}"
)

target_compile_definitions(06_model_load
		PUBLIC
		DSA
)