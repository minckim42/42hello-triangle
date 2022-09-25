#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

void key_manager(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

string file_loader(const filesystem::path& file_name)
{
    ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        throw string("Error: Failed to open file: ") + file_name.string();
    }
    stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

GLFWwindow* init_window()
{
    if (!glfwInit())
    {
        throw string("Error: Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 안티엘리어싱 x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 최대버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 최소버전: 그냥 glfw 버전
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 프로파일 버전: 코어
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
    if (window == NULL)
    {
        throw string("Error: Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // OpenGL 함수 포인터와 실제 함수를 매핑
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw string("Error: Failed to initialize GLAD");
    }
    return window;
}

GLuint complie_shader(const filesystem::path& vertex_shader_path, const filesystem::path& fragment_shader_path)
{
    GLuint shader_program;
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        // 버텍스 셰이더
        {
            vertex_shader = glCreateShader(GL_VERTEX_SHADER); // 셰이더 오브젝트 생성
            string shader_source = file_loader(vertex_shader_path);
            const char* shader_source_ptr = shader_source.data();
            glShaderSource(vertex_shader, 1, &shader_source_ptr, nullptr); // 셰이더 소스파일 로드
            glCompileShader(vertex_shader); // 셰이더 컴파일
            int success;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success); // 잘 됐는지 확인
            if (!success)
            {
                char log[512];
                glGetShaderInfoLog(vertex_shader, 512, nullptr, log); // 로그 확인
                throw string("Error: Failed to compile vertex shader:\n") + log;
            }
        }
        // 프래그먼트 셰이더
        {
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            string shader_source = file_loader(fragment_shader_path);
            const char* shader_source_ptr = shader_source.data();
            glShaderSource(fragment_shader, 1, &shader_source_ptr, nullptr);
            glCompileShader(fragment_shader);
            int success;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char log[512];
                glGetShaderInfoLog(fragment_shader, 512, nullptr, log);
                throw string("Error: Failed to compile fragment shader:\n") + log;
            }
        }
        // 셰이더 프로그램 링크
        {
            shader_program = glCreateProgram();
            glAttachShader(shader_program, vertex_shader);
            glAttachShader(shader_program, fragment_shader);
            glLinkProgram(shader_program);
            int success;
            glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char log[512];
                glGetProgramInfoLog(shader_program, 512, nullptr, log);
                throw string("Error: Failed to link shader program:\n") + log;
            }
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    return shader_program;
}


int main_process()
{
    // GLFW 윈도우 초기화
    GLFWwindow* window = init_window();
    
    // 셰이더
    GLuint shader_program = complie_shader(
        filesystem::path(ROOT_PATH) / "source/basic.vert", 
        filesystem::path(ROOT_PATH) / "source/basic.frag"
    );

    // 출력할 데이터
    vector<Vertex> vertices = {
        {{ 0.5f,  0.5f,  0.0f}, {0.8f, 0.4f, 0.3f}},
        {{ 0.5f, -0.5f,  0.0f}, {0.1f, 0.2f, 0.4f}},
        {{-0.5f, -0.5f,  0.0f}, {0.2f, 0.4f, 0.3f}},
        {{-0.5f,  0.5f,  0.0f}, {0.8f, 0.6f, 0.2f}},
    };

    vector<GLuint> indices = {
        0, 1, 3,
        1, 2, 3
    };

    // 버퍼
    GLuint vertex_array_obj;
    GLuint vertex_buffer_obj;
    GLuint element_buffer_obj;

    glCreateVertexArrays(1, &vertex_array_obj);
    glCreateBuffers(1, &vertex_buffer_obj);
    glCreateBuffers(1, &element_buffer_obj);
    
    glBindVertexArray(vertex_array_obj);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);


    // 렌더링 루프
    float foo = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        key_manager(window);

        // 렌더링
        //      버퍼 초기화
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //      유니폼
        foo += 0.01f;
        GLuint location_foo = glGetUniformLocation(shader_program, "foo");
        glUniform1f(location_foo, sinf(foo));

        //      그리기
        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vertex_array_obj);
    glDeleteBuffers(1, &vertex_buffer_obj);
    glDeleteBuffers(1, &element_buffer_obj);
    glDeleteProgram(shader_program);

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
