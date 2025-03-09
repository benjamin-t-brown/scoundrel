/*
This method of localization is taken from
https://gamedev.stackexchange.com/questions/168362/handling-localization-strings-in-a-performant-game-engine-c

It uses a hash map to store the strings, and a constexpr hash function to
generate the keys.

You would use this as so:

drawText(Localization::trans(LOCSTR(HelloWorld)), 0, 0);
**/

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace SDL2Wrapper {

#define FNV_PRIME_32 16777619u
#define FNV_OFFSET_32 2166136261u

#ifdef SDL2WRAPPER_ENABLE_LOCALIZATION
#define LOCSTR(token) hash(#token)
#else
#define LOCSTR(token) token
#endif

// Thanks CoPilot
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
constexpr int hash(const char* data) {
  uint32_t hash = FNV_OFFSET_32;
  while (*data) {
    hash ^= static_cast<uint32_t>(*data++);
    hash *= FNV_PRIME_32;
  }
  return hash;
}

class Localization {
  static std::string language;
  static std::unordered_map<int, std::string> locStrings;

public:
  static void init();
  static std::string trans(int id);

  // used for accommodating SDL2WRAPPER_ENABLE_LOCALIZATION enabled/disabled
  static std::string trans(const std::string& str);
  static std::string trans(const char* str);
};

} // namespace SDL2Wrapper
