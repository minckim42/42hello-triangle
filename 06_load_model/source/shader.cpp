#include "shader.h"

void check_shader_compile_status(GLuint shader)
{
   int success;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // 잘 됐는지 확인
   if (!success)
   {
      char log[512];
      glGetShaderInfoLog(shader, 512, nullptr, log); // 로그 확인
      throw std::runtime_error(std::string("Error: Failed to compile shader:\n") + log);
   }
}

GLuint compile_shader(const std::filesystem::path& source_path, int shader_type)
{
   GLuint shader;
   shader = glCreateShader(shader_type); // 셰이더 오브젝트 생성
   std::string shader_source = file_loader(source_path);
   const char* shader_source_ptr = shader_source.data();
   glShaderSource(shader, 1, &shader_source_ptr, nullptr); // 셰이더 소스파일 로드
   glCompileShader(shader); // 셰이더 컴파일
   check_shader_compile_status(shader);
   return shader;
}

void check_link_status(GLuint shader_program)
{
   int success;
   glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[512];
      glGetProgramInfoLog(shader_program, 512, nullptr, log);
      throw std::runtime_error(std::string("Error: Failed to link shader program:\n") + log);
   }
}

GLuint link_shader(GLuint vertex_shader, GLuint fragment_shader)
{
   GLuint shader_program = glCreateProgram();
   glAttachShader(shader_program, vertex_shader);
   glAttachShader(shader_program, fragment_shader);
   glLinkProgram(shader_program);
   check_link_status(shader_program);
   return shader_program;
}

GLuint compile_shader(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path)
{
   GLuint vertex_shader = compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
   GLuint fragment_shader = compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
   GLuint shader_program = link_shader(vertex_shader, fragment_shader);
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);
   return shader_program;
}

void set_uniform(GLuint shader, std::string_view name, int val)
{ glUniform1i(glGetUniformLocation(shader, name.data()), val); }

void set_uniform(GLuint shader, std::string_view name, float val)
{ glUniform1f(glGetUniformLocation(shader, name.data()), val); }

void set_uniform(GLuint shader, std::string_view name, glm::vec2 val, int count)
{ glUniform2fv(glGetUniformLocation(shader, name.data()), count, glm::value_ptr(val)); }

void set_uniform(GLuint shader, std::string_view name, glm::vec3 val, int count)
{ glUniform3fv(glGetUniformLocation(shader, name.data()), count, glm::value_ptr(val)); }

void set_uniform(GLuint shader, std::string_view name, glm::vec4 val, int count)
{ glUniform4fv(glGetUniformLocation(shader, name.data()), count, glm::value_ptr(val)); }

void set_uniform(GLuint shader, std::string_view name, glm::mat2 val, int count, bool transpose)
{ glUniformMatrix2fv(glGetUniformLocation(shader, name.data()), count, transpose, glm::value_ptr(val)); }

void set_uniform(GLuint shader, std::string_view name, glm::mat3 val, int count, bool transpose)
{ glUniformMatrix3fv(glGetUniformLocation(shader, name.data()), count, transpose, glm::value_ptr(val)); }

void set_uniform(GLuint shader, std::string_view name, glm::mat4 val, int count, bool transpose)
{ glUniformMatrix4fv(glGetUniformLocation(shader, name.data()), count, transpose, glm::value_ptr(val)); }