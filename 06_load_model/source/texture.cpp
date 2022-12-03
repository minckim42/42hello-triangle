#include "texture.h"

Texture::Texture(const std::filesystem::path& path)
{
   stbi_set_flip_vertically_on_load(true);
   int width, height, channel;
   unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 4);
   if (data == nullptr)
   {
      //throw std::runtime_error("Error: Failed to open image: " + path.string());
      std::cerr << "Warning: Failed to open image: " << path.string() << std::endl;
      return;
   }

#ifdef DSA
   glCreateTextures(GL_TEXTURE_2D, 1, &id);
   glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTextureStorage2D(id, 1, GL_RGBA8, width, height);
   glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
   glGenerateTextureMipmap(id);

#else
   // 텍스처 버퍼 생성
   glGenTextures(1, &id);
   glBindTexture(GL_TEXTURE_2D, id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   glGenerateMipmap(GL_TEXTURE_2D);

#endif
   stbi_image_free(data);
}
Texture::~Texture()
{
   if (id)
   {
      glDeleteTextures(1, &id);
   }
}

GLuint Texture::get_id() const noexcept
{
   return id;
}

void Texture::bind(GLuint shader_program, GLuint unit, const std::string_view name)
{
   if (!id)
   {
      return;
   }
#ifdef DSA
   glBindTextureUnit(unit, id);
   glUniform1i(glGetUniformLocation(shader_program, name.data()), unit);
#else
   glActiveTexture(GL_TEXTURE0 + unit);
   glBindTexture(GL_TEXTURE_2D, id);
   glUniform1i(glGetUniformLocation(shader_program, name.data()), unit);
#endif
}