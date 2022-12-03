#pragma once
#include "glm/glm.hpp"
#include "material.h"
#include "vertex_buffer.h"
#include <vector>

struct Mesh
{
   typedef std::shared_ptr<Mesh> Ptr;
   typedef std::map<std::string, Ptr> Container;

   std::vector<Vertex> vertices;
   std::vector<GLuint> indices;
   GLuint vao = 0;
   GLuint vbo = 0;
   GLuint ebo = 0;
   std::shared_ptr<Material> material;

   Mesh() = default;
   ~Mesh();

   void setup();
   void draw(GLuint shader, const glm::mat4& world, const glm::mat4& view, const glm::mat4& projection);
   [[nodiscard]] std::pair<glm::vec3, glm::vec3> get_bounding_box(const glm::mat4& world) const;

   Mesh(const Mesh& other) = delete;
   Mesh(Mesh&& other) = delete;
   Mesh& operator=(const Mesh& other) = delete;
   Mesh& operator=(Mesh&& other) = delete;
private:

};
