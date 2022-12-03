#include "file_loader.h"

std::string file_loader(const std::filesystem::path& file_name)
{
   std::ifstream ifs(file_name);
   if (!ifs.is_open())
   {
      throw std::runtime_error("Error: Failed to open file: " + file_name.string());
   }
   std::stringstream ss;
   ss << ifs.rdbuf();
   ifs.close();
   return ss.str();
}