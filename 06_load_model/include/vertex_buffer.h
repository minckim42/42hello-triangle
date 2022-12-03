#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

struct Vertex
{
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec3 color;
   glm::vec2 texture_coord;
};

void set_vertex(
   GLuint& vao,
   GLuint& vbo,
   GLuint& ebo,
   const std::vector<Vertex>& vertices,
   std::vector<unsigned int>& indices);