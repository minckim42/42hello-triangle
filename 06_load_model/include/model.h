#pragma once
#include <vector>
#include "mesh.h"

struct Model
{
   typedef std::shared_ptr<Model> Ptr;

   Model() = default;
   ~Model() = default;

   glm::mat4 transform = glm::mat4(1.0f);
   std::vector<Mesh::Ptr> meshes;
   std::vector<Ptr> children;

   void draw(GLuint shader, const glm::mat4& world, const glm::mat4& view, const glm::mat4& projection);
   [[nodiscard]] std::pair<glm::vec3, glm::vec3> get_bounding_box(const glm::mat4& world);
};