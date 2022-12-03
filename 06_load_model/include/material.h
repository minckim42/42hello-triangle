#pragma once
#include "glad/glad.h"
#include "shader.h"
#include "texture.h"
#include <map>

class Material
{
public:
   typedef std::shared_ptr<Material> Ptr;
   typedef std::map<std::string, Ptr> Container;

   Texture::Ptr ambient = nullptr;
   Texture::Ptr diffuse = nullptr;
   Texture::Ptr specular = nullptr;
   glm::vec3 ka = {1.0f, 1.0f, 1.0f};
   glm::vec3 ks = {1.0f, 1.0f, 1.0f};
   glm::vec3 kd = {1.0f, 1.0f, 1.0f};

   Material() = default;
   ~Material() = default;
   Material(
      const Texture::Ptr& ambient,
      const Texture::Ptr& diffuse,
      const Texture::Ptr& specular);

   void bind(GLuint shader) const;

   Material(const Material& other) = delete;
   Material& operator=(const Material& other) = delete;
};