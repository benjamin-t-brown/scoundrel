#pragma once

#include "lib/sdl2wrapper/Localization.h"
#include <string>

namespace program {
// uses nintendo layout
inline bool isConfirmKey(const std::string& key) {
  return key == "Space" || key == "X"; // A
}
inline std::string getConfirmKey() { return "X"; }
inline std::string getConfirmKeyLabel() {
  return sdl2w::L10n::trans(LOCSTR("(A)"));
}
inline bool isCancelKey(const std::string& key) {
  return key == "Z" || key == "Left Ctrl"; // B
}
inline std::string getCancelKey() { return "Z"; }
inline std::string getCancelKeyLabel() {
  return sdl2w::L10n::trans(LOCSTR("(B)"));
}
inline bool isAuxKey(const std::string& key) {
  return key == "C" || key == "Left Shift"; // Y
}
inline std::string getAuxKey() { return "C"; }
inline std::string getAuxKeyLabel() {
  return sdl2w::L10n::trans(LOCSTR("(Y)"));
}
inline bool isAssistKey(const std::string& key) {
  return key == "S" || key == "Left Alt"; // X
}
inline std::string getAssistKey() { return "S"; }
inline std::string getAssistKeyLabel() {
  return sdl2w ::L10n::trans(LOCSTR("(X)"));
}
inline bool isLeftTriggerKey(const std::string& key) {
  return key == "Tab"; // L2
}
inline std::string getLeftTriggerKey() { return "Tab"; }
inline std::string getLeftTriggerKeyLabel() {
  return sdl2w::L10n::trans(LOCSTR("(LT)"));
}
inline bool isRightTriggerKey(const std::string& key) {
  return key == "Backspace"; // R2
}
inline std::string getRightTriggerKey() { return "Backspace"; }
inline std::string getRightTriggerKeyLabel() {
  return sdl2w::L10n::trans(LOCSTR("(RT)"));
}
inline bool isStartKey(const std::string& key) {
  return key == "Return"; // Start
}
inline std::string getStartKey() { return "Return"; }
inline std::string getStartKeyLabel() {
  return sdl2w::L10n ::trans(LOCSTR("(Start)"));
}
inline bool isSelectKey(const std::string& key) {
  return key == "Right Ctrl"; // Select
}
inline std::string getSelectKey() { return "Right Ctrl"; }
inline std::string getSelectKeyLabel() {
  return sdl2w::L10n ::trans(LOCSTR("(Select)"));
}
inline bool isArrowKey(const std::string& key) {
  return key == "Left" || key == "Right" || key == "Up" || key == "Down";
}
inline bool isLeftKey(const std::string& key) { return key == "Left"; }
inline std::string getLeftKey() { return "Left"; }
inline bool isRightKey(const std::string& key) { return key == "Right"; }
inline std::string getRightKey() { return "Right"; }
inline bool isUpKey(const std::string& key) { return key == "Up"; }
inline std::string getUpKey() { return "Up"; }
inline bool isDownKey(const std::string& key) { return key == "Down"; }
inline std::string getDownKey() { return "Down"; }

} // namespace program