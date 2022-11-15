#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void key_manager(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main_process()
{
    if (!glfwInit())
    {
        throw runtime_error("Error: Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 안티엘리어싱 x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 최대버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 최소버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 프로파일 버전: 코어
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
    // GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", glfwGetPrimaryMonitor(), nullptr); // 전체화면
    if (window == nullptr)
    {
        throw runtime_error("Error: Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // OpenGL 함수 포인터와 실제 함수를 매핑
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw runtime_error("Error: Failed to initialize GLAD");
    }

    // 렌더링 루프
    while (!glfwWindowShouldClose(window))
    {
        key_manager(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

int main()
{
    try
    {
        return main_process();
    }
    catch(const string& err)
    {
        std::cerr << err << '\n';
        glfwTerminate();
        return -1;
    }
}
