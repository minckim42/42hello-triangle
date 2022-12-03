#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

inline
GLFWwindow* init_window(int width, int height, std::string_view name)
{
   if (!glfwInit())
   {
      throw std::runtime_error("Error: Failed to initialize GLFW");
   }
   glfwWindowHint(GLFW_SAMPLES, 4); // 안티엘리어싱 x4
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 최대버전: 그냥 glfw 버전
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 최소버전: 그냥 glfw 버전
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 프로파일 버전: 코어
#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   GLFWwindow* window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
   if (window == NULL)
   {
      throw std::runtime_error("Error: Failed to create GLFW window");
   }
   glfwMakeContextCurrent(window);

   // OpenGL 함수 포인터와 실제 함수를 매핑
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      throw std::runtime_error("Error: Failed to initialize GLAD");
   }
   return window;
}
