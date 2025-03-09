#include "Localization.h"

namespace SDL2Wrapper {

std::string Localization::language;
std::unordered_map<int, std::string> Localization::locStrings;

void Localization::init() {
  // TODO load string file
}

std::string Localization::trans(int id) {
  auto it = locStrings.find(id);
  if (it != locStrings.end()) {
    return it->second;
  }
  return "?MISSING?";
}

std::string Localization::trans(const std::string& str) { return str; }
std::string Localization::trans(const char* str) { return std::string(str); }

} // namespace SDL2Wrapper