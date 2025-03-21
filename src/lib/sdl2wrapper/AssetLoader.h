#pragma once

#include <string>

namespace sdl2w {
void loadAssetsFromFile(const std::string& type, const std::string& path);
std::string loadFileAsString(const std::string& path);
} // namespace sdl2w