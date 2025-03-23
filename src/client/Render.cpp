#include "Render.h"
#include "game/calculations.hpp"
#include "game/state.h"
#include "game/utils/transform.hpp"
#include "lib/sdl2wrapper/Localization.h"
#include "lib/sdl2wrapper/SDL2Includes.h"
#include "lib/sdl2wrapper/Window.h"
#include <sstream>

namespace program {

double Render::cardScale = 3.;

Render::Render(sdl2w::Window& windowA) : window(windowA) {}

int Render::cardValueToSpriteIndex(int value) { return 14 - value; }

std::string Render::cardSuitToName(Suit suit) {
  switch (suit) {
  case HEARTS:
    return "hearts";
  case DIAMONDS:
    return "diamonds";
  case SPADES:
    return "spades";
  case CLUBS:
  default:
    return "clubs";
  }
}

void Render::renderCard(const VisualCard& vCard) {
  std::stringstream ss;
  if (vCard.faceDown) {
    ss << "cards_back_0";
  } else {
    ss << "cards_" << cardSuitToName(vCard.card.suit) << "_"
       << cardValueToSpriteIndex(vCard.card.value);
  }

  window.drawSprite(ss.str(),
                    vCard.pos.x + CARD_WIDTH * cardScale / 2,
                    vCard.pos.y + CARD_HEIGHT * cardScale / 2,
                    true,
                    0,
                    {cardScale, cardScale});
}

void Render::renderWeaponCard(const VisualCard& vCard,
                              std::vector<VisualCard>& weaponDefeated) {
  renderCard(vCard);

  int effectiveValue =
      std::max(0, calcWeaponAttackableValue(vCard, weaponDefeated));
  if (effectiveValue < 15) {
    window.setCurrentFont("default", 22);
    window.drawText("(" + std::to_string(effectiveValue) + ")",
                    vCard.pos.x + (vCard.card.value >= 10 ? 44 : 28),
                    vCard.pos.y,
                    {0, 0, 0});
  }
}

void Render::renderCursor(const Cursor& cursor) {
  if (cursor.type == CARD) {
    const std::string sprite = "ui_card_selected_0";
    window.drawSprite(sprite,
                      cursor.pos.x + CARD_WIDTH * cardScale / 2,
                      cursor.pos.y + CARD_HEIGHT * cardScale / 2,
                      true,
                      0,
                      {cardScale, cardScale});
  } else {
    const std::string sprite = "ui_button_selected_0";
    window.drawSprite(
        sprite, cursor.pos.x, cursor.pos.y, true, 0, {cardScale, cardScale});
  }
}

void Render::renderConfirm(const ConfirmData& confirmData, int cursorInd) {
  const std::pair<int, int> textPos = {window.renderWidth / 2,
                                       window.renderHeight / 2 -
                                       CARD_HEIGHT * CARD_SCALE / 2 + 8};

  window.setCurrentFont("default", 22);
  window.drawTextCentered(
      confirmData.text, textPos.first, textPos.second, {255, 255, 255});

  if (cursorInd == 0) {
    if (!confirmData.leftHelpText.empty()) {
      window.setCurrentFont("default", 18);
      window.drawTextCentered(confirmData.leftHelpText,
                              textPos.first,
                              textPos.second + 22,
                              {200, 200, 200});
    }
  } else if (cursorInd == 1) {
    if (!confirmData.rightHelpText.empty()) {
      window.setCurrentFont("default", 18);
      window.drawTextCentered(confirmData.rightHelpText,
                              textPos.first,
                              textPos.second + 22,
                              {200, 200, 200});
    }
  }

  if (confirmData.type == PICK_WEAPON_TO_ATTACK_WITH) {
    window.drawSprite("ui_card_button_0",
                      CONFIRM_CARD_LEFT_POS.first,
                      CONFIRM_CARD_LEFT_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
    if (confirmData.leftDisabled) {
      window.drawSprite("card_disabled_0",
                        CONFIRM_CARD_LEFT_POS.first,
                        CONFIRM_CARD_LEFT_POS.second,
                        true,
                        0,
                        {CARD_SCALE, CARD_SCALE});
    }

    window.drawSprite("ui_card_button_0",
                      CONFIRM_CARD_RIGHT_POS.first,
                      CONFIRM_CARD_RIGHT_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});

    // HACK manually assume the fist card here
    window.drawSprite("card_fist_0",
                      CONFIRM_CARD_RIGHT_POS.first,
                      CONFIRM_CARD_RIGHT_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
  } else {
    window.setCurrentFont("default", 22);
    window.drawSprite("ui_button_0",
                      CONFIRM_LEFT_POS.first,
                      CONFIRM_LEFT_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
    window.drawTextCentered(sdl2w::L10n::trans(LOCSTR("Yes")),
                            CONFIRM_LEFT_POS.first,
                            CONFIRM_LEFT_POS.second,
                            {255, 255, 255});
    window.drawSprite("ui_button_0",
                      CONFIRM_RIGHT_POS.first,
                      CONFIRM_RIGHT_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
    window.drawTextCentered(sdl2w::L10n::trans(LOCSTR("No")),
                            CONFIRM_RIGHT_POS.first,
                            CONFIRM_RIGHT_POS.second,
                            {255, 255, 255});
  }
}

void Render::renderWeaponUi(std::optional<VisualCard>& weapon) {
  window.drawSprite("ui_card_button_0",
                    WEAPON_UI_POS.first,
                    WEAPON_UI_POS.second,
                    true,
                    0,
                    {CARD_SCALE, CARD_SCALE});

  if (!weapon.has_value()) {
    window.drawSprite("card_fist_0",
                      WEAPON_UI_POS.first,
                      WEAPON_UI_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
  }
}

void Render::renderFleeUi(bool isDisabled) {
  window.setCurrentFont("default", 22);
  window.drawSprite("ui_button_0",
                    FLEE_UI_POS.first,
                    FLEE_UI_POS.second,
                    true,
                    0,
                    {CARD_SCALE, CARD_SCALE});
  window.drawTextCentered(sdl2w::L10n::trans(LOCSTR("Flee")),
                          FLEE_UI_POS.first,
                          FLEE_UI_POS.second,
                          {255, 255, 255});
  if (isDisabled) {
    window.drawSprite("ui_button_disabled_0",
                      FLEE_UI_POS.first,
                      FLEE_UI_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
  }
}

void Render::renderDeckNumber(int numCards) {
  window.setCurrentFont("default", 22);
  window.drawTextCentered(std::to_string(numCards),
                          CARD_WIDTH * CARD_SCALE / 2,
                          CARD_HEIGHT * CARD_SCALE + 16,
                          SDL_Color{255, 255, 255});
}

void Render::renderHealthUi(int hp,
                            int maxHp,
                            const ActionPreviewData& actionPreviewData) {
  window.drawSprite("ui_heart_0",
                    10,
                    window.renderHeight - 12 * CARD_SCALE - 13,
                    false,
                    0,
                    {CARD_SCALE, CARD_SCALE});
  window.setCurrentFont("default", 36);
  window.drawTextCentered(std::to_string(hp),
                          75,
                          window.renderHeight - 12 * CARD_SCALE / 2 - 11,
                          hp < maxHp ? SDL_Color{255, 83, 74}
                                     : SDL_Color{255, 255, 255});

  if (actionPreviewData.visible) {
    window.drawSprite("ui_arrow_up_0",
                      75,
                      window.renderHeight - 12 * CARD_SCALE - 32,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
    window.drawTextCentered(std::to_string(actionPreviewData.nextHp),
                            75,
                            window.renderHeight - 12 * CARD_SCALE / 2 - 90,
                            actionPreviewData.nextHp == hp
                                ? SDL_Color{255, 255, 255}
                                : (actionPreviewData.nextHp >= hp
                                       ? SDL_Color{161, 239, 121}
                                       : SDL_Color{255, 83, 74}));
  }
}

void Render::renderEndGameScreen(const std::string& message,
                                 const std::string& instructionMessage) {

  SDL_Texture* bgRect = window.getStaticColorTexture(
      window.renderWidth, window.renderHeight, SDL_Color{0, 0, 0, 200});
  window.drawTexture(
      bgRect, 0, 0, window.renderWidth, window.renderHeight, false);

  window.setCurrentFont("default", 48);
  window.drawTextCentered(message,
                          window.renderWidth / 2,
                          window.renderHeight / 2,
                          SDL_Color{255, 255, 255});

  window.setCurrentFont("default", 22);
  window.drawTextCentered(instructionMessage,
                          window.renderWidth / 2,
                          window.renderHeight / 2 + 100,
                          SDL_Color{255, 255, 255});
}

void Render::renderMenuScreen(const std::string& message,
                              const std::string& instructionMessage,
                              int numWins,
                              int menuCardsT) {
  SDL_Texture* bgRect = window.getStaticColorTexture(
      window.renderWidth, window.renderHeight, SDL_Color{0, 0, 0, 255});
  window.drawTexture(
      bgRect, 0, 0, window.renderWidth, window.renderHeight, false);

  window.setCurrentFont("default", 48);
  window.drawTextCentered(message,
                          window.renderWidth / 2 - 2,
                          window.renderHeight / 2 - 50 + 14 - 2,
                          SDL_Color{190, 100, 100});
  window.drawTextCentered(message,
                          window.renderWidth / 2,
                          window.renderHeight / 2 - 50 + 14,
                          SDL_Color{255, 255, 255});

  window.setCurrentFont("default", 22);
  window.drawTextCentered(instructionMessage,
                          window.renderWidth / 2,
                          window.renderHeight / 2 + 50,
                          SDL_Color{255, 255, 255});

  window.setCurrentFont("default", 22);
  window.drawTextCentered(sdl2w::L10n::trans(LOCSTR("Wins")) + ": " +
                              std::to_string(numWins),
                          window.renderWidth / 2,
                          window.renderHeight / 2 + 12,
                          SDL_Color{200, 200, 200});

  int numCardsWide = window.renderWidth / CARD_WIDTH * CARD_SCALE + 1;

  int offset = transform::normalize(
      menuCardsT % 1000, 0, 1000, 0, CARD_WIDTH * CARD_SCALE);

  window.drawSprite("cards_hearts_5",
                    window.renderWidth / 7,
                    window.renderHeight / 2 - 2,
                    true,
                    0,
                    {cardScale, cardScale});

  window.drawSprite("cards_spades_3",
                    window.renderWidth * 6 / 7,
                    window.renderHeight / 2 - 2,
                    true,
                    0,
                    {cardScale, cardScale});

  for (int i = 0; i < numCardsWide; i++) {
    window.drawSprite("cards_back_0",
                      i * CARD_WIDTH * CARD_SCALE - CARD_WIDTH * CARD_SCALE +
                          offset,
                      0,
                      false,
                      0,
                      {cardScale, cardScale});
    window.drawSprite("cards_back_0",
                      i * CARD_WIDTH * CARD_SCALE - CARD_WIDTH * CARD_SCALE -
                          offset,
                      window.renderHeight - CARD_HEIGHT * CARD_SCALE,
                      false,
                      0,
                      {cardScale, cardScale});
  }
}

} // namespace program