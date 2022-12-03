#pragma once
#include <filesystem>
#include <fstream>
#include <stdexcept>

std::string file_loader(const std::filesystem::path& file_name);