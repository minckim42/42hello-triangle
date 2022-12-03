#include "material.h"

Material::Material(const Texture::Ptr& ambient,
   const Texture::Ptr& diffuse,
   const Texture::Ptr& specular):
   ambient(ambient), diffuse(diffuse), specular(specular)
{}

void Material::bind(GLuint shader) const
{
   if (ambient)
   {
      ambient->bind(shader, 0, "ambient");
   }
   if (diffuse)
   {
      diffuse->bind(shader, 1, "diffuse");
   }
   if (specular)
   {
      specular->bind(shader, 2, "specular");
   }
   set_uniform(shader, "ka", ka);
   set_uniform(shader, "ks", ks);
   set_uniform(shader, "kd", kd);
}