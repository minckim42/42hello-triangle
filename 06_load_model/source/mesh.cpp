#include "mesh.h"

Mesh::~Mesh()
{
   if (vao)
   {
      glDeleteVertexArrays(1, &vao);
   }
   if (vbo)
   {
      glDeleteBuffers(1, &vbo);
   }
   if (ebo)
   {
      glDeleteBuffers(1, &ebo);
   }
}

void Mesh::setup()
{
   set_vertex(vao, vbo, ebo, vertices, indices);
}

void Mesh::draw(GLuint shader, const glm::mat4& world, const glm::mat4& view, const glm::mat4& projection)
{
   glBindVertexArray(vao);
   glUseProgram(shader);
   set_uniform(shader, "view", view);
   set_uniform(shader, "model", world);
   set_uniform(shader, "projection", projection);
   material->bind(shader);
   glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

std::pair<glm::vec3, glm::vec3> Mesh::get_bounding_box(const glm::mat4& world) const
{
   std::pair<glm::vec3, glm::vec3> result = {glm::vec3(FLT_MAX), glm::vec3(FLT_MIN)};
   for (const Vertex& vertex: vertices)
   {
      glm::vec3 position_tmp = world * glm::vec4(vertex.position, 1.0f);
      result.first = glm::min(result.first, position_tmp);
      result.second = glm::max(result.second, position_tmp);
   }
   return result;
}
