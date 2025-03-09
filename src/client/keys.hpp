#pragma once

#include "lib/sdl2wrapper/Localization.h"
#include <string>

namespace scoundrel {
inline bool isConfirmKey(const std::string& key) {
  return key == "Return" || key == "Space" || key == "X";
}
inline std::string getConfirmKeyLabel() {
  return SDL2Wrapper::Localization::trans(LOCSTR("(X)"));
}
inline bool isCancelKey(const std::string& key) {
  return key == "Z" || key == "Escape";
}
inline std::string getCancelKeyLabel() {
  return SDL2Wrapper::Localization::trans(LOCSTR("(Z)"));
}
inline bool isAuxKey(const std::string& key) { return key == "C"; }
inline std::string getAuxKeyLabel() {
  return SDL2Wrapper::Localization::trans(LOCSTR("(C)"));
}
inline bool isArrowKey(const std::string& key) {
  return key == "Left" || key == "Right" || key == "Up" || key == "Down";
}
inline bool isLeftKey(const std::string& key) { return key == "Left"; }
inline bool isRightKey(const std::string& key) { return key == "Right"; }
inline bool isUpKey(const std::string& key) { return key == "Up"; }
inline bool isDownKey(const std::string& key) { return key == "Down"; }
} // namespace scoundrel