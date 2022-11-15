# 프로젝트에서 요구하는 CMake의 최소 버전
cmake_minimum_required(VERSION 3.13)

# 프로젝트
project(
	HelloTriangle # 프로젝트명
)

set(CMAKE_CXX_STANDARD 17) # C++ 버전 설정

# 환경변수 설정
set(GLFW_DIR # Key
	"./glfw-3.3.8" # Value
)

set(GLAD_DIR
	"./glad"
)

set(GLM_DIR
	"./glm"
)

set(GLAD_SRC
	${GLAD_DIR}/src/glad.c
)

set(SOURCE_FILES
	${GLAD_SRC}
	source/main.cpp
)

# 컴파일 소스
add_executable(HelloTriangle # 실행파일 이름
	${SOURCE_FILES}
)

# 서브 디렉토리 추가
add_subdirectory(
	${GLFW_DIR}
)

# 인클루드 디렉토리 추가
target_include_directories(HelloTriangle # 실행파일 이름
	PUBLIC # 외부에서 이 빌드를 실행할 때도 인클루드에 포함되도록(PRIVATE이면 이 빌드에서만 적용됨)
    include
	${GLM_DIR}
	${GLFW_DIR}/include
	${GLAD_DIR}/include
)

# 라이브러리 추가
target_link_libraries(HelloTriangle
	glfw
)

# 프로젝트에 define 추가
target_compile_definitions(HelloTriangle
	PRIVATE
    ROOT_PATH="${CMAKE_SOURCE_DIR}"
)