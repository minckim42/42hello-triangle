cmake_minimum_required(VERSION 3.13)

project(
	HelloTriangle
)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR
	"glfw-3.3.8"
)

set(SOURCE_FILES
	source/glad.c
	source/main.cpp
)

add_executable(HelloTriangle
	${SOURCE_FILES}
)

add_subdirectory(
	${GLFW_DIR}
)

target_include_directories(HelloTriangle
	PUBLIC
    include
	${GLFW_DIR}/include
)

target_link_libraries(HelloTriangle
	glfw
)

target_compile_definitions(HelloTriangle
	PRIVATE
    ROOT_PATH="${CMAKE_SOURCE_DIR}"
)