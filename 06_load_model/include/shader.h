#pragma once
#include "glad/glad.h"
#include "file_loader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

void check_shader_compile_status(GLuint shader);
GLuint compile_shader(const std::filesystem::path& source_path, int shader_type);
void check_link_status(GLuint shader_program);
GLuint link_shader(GLuint vertex_shader, GLuint fragment_shader);
GLuint compile_shader(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path);
void set_uniform(GLuint shader, std::string_view name, int val);
void set_uniform(GLuint shader, std::string_view name, float val);
void set_uniform(GLuint shader, std::string_view name, glm::vec2 val, int count = 1);
void set_uniform(GLuint shader, std::string_view name, glm::vec3 val, int count = 1);
void set_uniform(GLuint shader, std::string_view name, glm::vec4 val, int count = 1);
void set_uniform(GLuint shader, std::string_view name, glm::mat2 val, int count = 1, bool transpose = false);
void set_uniform(GLuint shader, std::string_view name, glm::mat3 val, int count = 1, bool transpose = false);
void set_uniform(GLuint shader, std::string_view name, glm::mat4 val, int count = 1, bool transpose = false);