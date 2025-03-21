#pragma once

#include "game/state.h"
#include <string>

namespace sdl2w {
class Window;
}

namespace program {

class Render {
  sdl2w::Window& window;
  static double cardScale;

  int cardValueToSpriteIndex(int value);
  std::string cardSuitToName(Suit suit);

public:
  Render(sdl2w::Window& windowA);

  void renderCard(const VisualCard& vCard);
  void renderWeaponCard(const VisualCard& vCard,
                        std::vector<VisualCard>& weaponDefeated);
  void renderCursor(const Cursor& cursor);
  void renderConfirm(const ConfirmData& confirmData, int cursorInd);
  void renderWeaponUi(std::optional<VisualCard>& weapon);
  void renderFleeUi(bool isDisabled);
  void renderDeckNumber(int numCards);
  void
  renderHealthUi(int hp, int maxHp, const ActionPreviewData& actionPreviewData);
  void renderEndGameScreen(const std::string& message,
                           const std::string& instructionMessage);
  void renderMenuScreen(const std::string& message,
                        const std::string& instructionMessage,
                        int numWins,
                        int menuCardsT);
};
} // namespace program