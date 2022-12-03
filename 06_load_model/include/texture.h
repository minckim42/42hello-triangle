#pragma once
#include "glad/glad.h"
#include "stb_image.h"
#include <filesystem>
#include <stdexcept>
#include <map>
#include <memory>
#include <iostream>

class Texture
{
public:
   typedef std::shared_ptr<Texture> Ptr;
   typedef std::map<std::string, Ptr> Container;

   Texture(const std::filesystem::path& path);
   ~Texture();

   [[nodiscard]] GLuint get_id() const noexcept;
   void bind(GLuint shader_program, GLuint unit, const std::string_view name);

private:
   GLuint id = 0;
};