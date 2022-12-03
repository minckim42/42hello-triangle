#include "model.h"

void Model::draw(GLuint shader, const glm::mat4& world, const glm::mat4& view, const glm::mat4& projection)
{
   for (const Mesh::Ptr& mesh: meshes)
   {
      mesh->draw(shader, world * transform, view, projection);
   }
   for (const Ptr& child: children)
   {
      child->draw(shader, world * transform, view, projection);
   }
}

std::pair<glm::vec3, glm::vec3> Model::get_bounding_box(const glm::mat4& world)
{
   std::pair<glm::vec3, glm::vec3> result = {glm::vec3(FLT_MAX), glm::vec3(FLT_MIN)};
   for (const Mesh::Ptr& mesh: meshes)
   {
      std::pair<glm::vec3, glm::vec3> bounding_box = mesh->get_bounding_box(world);
      result.first = glm::min(result.first, bounding_box.first);
      result.second = glm::max(result.second, bounding_box.first);
   }
   for (const Model::Ptr& child: children)
   {
      std::pair<glm::vec3, glm::vec3> bounding_box = child->get_bounding_box(world);
      result.first = glm::min(result.first, bounding_box.first);
      result.second = glm::max(result.second, bounding_box.first);
   }
   return result;
}