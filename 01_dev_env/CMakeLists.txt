# 프로젝트에서 요구하는 CMake의 최소 버전
cmake_minimum_required(VERSION 3.11)

# 프로젝트
project(
	hello_cmake # 프로젝트명
)

# 컴파일 소스
add_executable(
	hello_cmake_executable # 실행파일 이름
	main.cpp # 소스
)