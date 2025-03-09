#pragma once

#include "game/state.h"
#include <string>

namespace SDL2Wrapper {
class Window;
}

namespace scoundrel {

class Render {
  SDL2Wrapper::Window& window;
  static double cardScale;

  int cardValueToSpriteIndex(int value);
  std::string cardSuitToName(Suit suit);

public:
  Render(SDL2Wrapper::Window& windowA);

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
                        const std::string& instructionMessage, int menuCardsT);
};
} // namespace scoundrel