#pragma once

#include "game/utils/timer.hpp"
#include "game/utils/transform.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace scoundrel {

namespace actions {
class AbstractAction;
}

constexpr const int CARD_WIDTH = 32;
constexpr const int CARD_HEIGHT = 48;
constexpr const int CARD_SCALE = 3;
constexpr const int WINDOW_WIDTH = 640;
constexpr const int WINDOW_HEIGHT = 480;

constexpr const std::pair<int, int> CONFIRM_LEFT_POS = {
    WINDOW_WIDTH / 2 - CARD_WIDTH * CARD_SCALE, WINDOW_HEIGHT / 2};
constexpr const std::pair<int, int> CONFIRM_RIGHT_POS = {
    WINDOW_WIDTH / 2 + CARD_WIDTH * CARD_SCALE, WINDOW_HEIGHT / 2};
constexpr const std::pair<int, int> CONFIRM_CARD_LEFT_POS = {
    WINDOW_WIDTH / 2 - (CARD_WIDTH * CARD_SCALE - 20), WINDOW_HEIGHT / 2 + 75};
constexpr const std::pair<int, int> CONFIRM_CARD_RIGHT_POS = {
    WINDOW_WIDTH / 2 + (CARD_WIDTH * CARD_SCALE - 20), WINDOW_HEIGHT / 2 + 75};
constexpr const std::pair<int, int> WEAPON_UI_POS = {
    WINDOW_WIDTH / 2, WINDOW_HEIGHT - (CARD_HEIGHT * CARD_SCALE / 2) - 10};
constexpr const std::pair<int, int> WEAPON_CARD_POS = {
    WEAPON_UI_POS.first - CARD_WIDTH * CARD_SCALE / 2,
    WEAPON_UI_POS.second - CARD_HEIGHT* CARD_SCALE / 2};
constexpr const std::pair<int, int> CARD_CENTER_SCREEN_POS = {
    WINDOW_WIDTH / 2 - CARD_WIDTH * CARD_SCALE / 2,
    WINDOW_HEIGHT / 2 - CARD_HEIGHT* CARD_SCALE / 2};
constexpr const std::pair<int, int> CARD_DISCARD_POS = {
    WINDOW_WIDTH + CARD_WIDTH * CARD_SCALE + CARD_WIDTH * CARD_SCALE / 2,
    WINDOW_HEIGHT + CARD_HEIGHT* CARD_SCALE + CARD_HEIGHT* CARD_SCALE / 2};
constexpr const std::pair<int, int> CARD_WEAPON_DEFEATED_POS = {
    WINDOW_WIDTH / 2 + CARD_WIDTH * CARD_SCALE / 2 + 10,
    WEAPON_CARD_POS.second};
constexpr const std::pair<int, int> FLEE_UI_POS = {WINDOW_WIDTH / 2, 190};
constexpr const std::pair<int, int> CARD_STACK_POS = {4, 4};

enum Suit { HEARTS, DIAMONDS, SPADES, CLUBS };

struct Card {
  int value = 2;
  Suit suit = HEARTS;
};

struct VisualCard {
  Card card;
  transform::Transform pos;
  bool faceDown = false;
};

enum CursorType { CARD, BUTTON };
struct Cursor {
  bool visible = true;
  transform::Transform pos;
  CursorType type = CARD;
};

struct UiCardCursorInds {
  int room = 0;
  int confirm = 0;
};

enum ConfirmType {
  CONFIRM_HEAL,
  CONFIRM_FLEE,
  CONFIRM_CHOOSE_WEAPON,
  PICK_WEAPON_TO_ATTACK_WITH
};

struct ConfirmData {
  ConfirmType type = CONFIRM_HEAL;
  int confirmCardRoomInd = 0;
  std::string text;
  std::string leftHelpText;
  std::string rightHelpText;
  bool leftDisabled = false;
  bool rightDisabled = false;
};

enum InputMode { NONE, ROOM, CONFIRM, END_GAME_SCREEN, MENU };

struct ActionPreviewData {
  bool visible = false;
  int nextHp = 0;
};

struct AsyncAction {
  std::unique_ptr<actions::AbstractAction> action;
  timer::Timer timer;
};

struct UiState {
  InputMode inputMode = NONE;
  Cursor cursor;
  UiCardCursorInds cursorInds;
  ConfirmData confirmData;
  bool isFleeSelected = false;
  bool renderFistAttack = false;
  int fleeCtr = 0;

  ActionPreviewData actionPreviewData;
  std::string titleMessage;
  std::string instructionMessage;

  std::vector<std::string> soundsToPlay;

  int menuCardsT = 0;

  std::pair<int, int> activeWeaponPos = {
      WINDOW_WIDTH / 2 - CARD_WIDTH * CARD_SCALE, WINDOW_HEIGHT / 2};
  std::pair<int, int> noWeaponPos = {WINDOW_WIDTH / 2 + CARD_WIDTH * CARD_SCALE,
                                     WINDOW_HEIGHT / 2};
};

struct State {
  std::optional<VisualCard> currentWeapon;
  std::vector<VisualCard> weaponDefeated;
  std::vector<VisualCard> room;
  std::vector<VisualCard> stack;
  std::vector<VisualCard> discard;

  std::vector<AsyncAction> asyncActions;

  UiState ui;

  int playerHealth = 20;
  int playerHealthMax = 20;
  bool isVictory = false;
};

} // namespace scoundrel