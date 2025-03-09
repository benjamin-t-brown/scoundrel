#include "GameManager.h"
#include "LibHTML.h"
#include "client/keys.hpp"
#include "game/Dispatch.h"
#include "game/actions/game/StartGame.hpp"
#include "game/actions/ui/HoverCardInRoom.hpp"
#include "game/actions/ui/HoverConfirmButton.hpp"
#include "game/actions/ui/HoverFleeButtonInRoom.hpp"
#include "game/actions/ui/SelectConfirm.hpp"
#include "game/actions/ui/SelectRoomCard.hpp"
#include "game/actions/ui/SetInputModeMenu.hpp"
#include "game/state.h"
#include "game/utils/timer.hpp"
#include "game/utils/transform.hpp"
#include "lib/sdl2wrapper/AssetLoader.h"
#include "lib/sdl2wrapper/Window.h"

namespace scoundrel {
GameManager::GameManager(SDL2Wrapper::Window& windowA)
    : window(windowA), r(Render(windowA)) {
  Dispatch::init(&state);
}

GameManager::~GameManager() {}

void GameManager::load() {
  SDL2Wrapper::loadAssetsFromFile("sprite", "assets/resSprites.txt");
  SDL2Wrapper::loadAssetsFromFile("sound", "assets/resSounds.txt");
}

void GameManager::start() {
  DISPATCH_ACTION(actions::SetInputModeMenu);
  libhtml::notifyGameReady();
}

void GameManager::handleKeyPress(const std::string& key) {
  // logger::info(("Keypress: " + key).c_str());
  if (state.ui.inputMode == InputMode::CONFIRM) {
    if (isLeftKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      Dispatch::get().addAction(std::make_unique<actions::HoverConfirmButton>(
          state.ui.cursorInds.confirm - 1));
    } else if (isRightKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      Dispatch::get().addAction(std::make_unique<actions::HoverConfirmButton>(
          state.ui.cursorInds.confirm + 1));
    } else if (isConfirmKey(key)) {
      Dispatch::get().addAction(std::make_unique<actions::SelectConfirm>(
          state.ui.cursorInds.confirm));
    } else if (isCancelKey(key)) {
      state.ui.soundsToPlay.push_back("ui_select");
      Dispatch::get().addAction(std::make_unique<actions::SetInputModeRoom>());
    }
  } else if (state.ui.inputMode == InputMode::ROOM) {
    if (isLeftKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      Dispatch::get().addAction(std::make_unique<actions::HoverCardInRoom>(
          state.ui.cursorInds.room - 1));
    } else if (isRightKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      Dispatch::get().addAction(std::make_unique<actions::HoverCardInRoom>(
          state.ui.cursorInds.room + 1));
    } else if (isDownKey(key) || isUpKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      if (state.ui.isFleeSelected) {
        Dispatch::get().addAction(std::make_unique<actions::HoverCardInRoom>(
            state.ui.cursorInds.room));
      } else {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverFleeButtonInRoom>());
      }
    } else if (isConfirmKey(key)) {
      Dispatch::get().addAction(std::make_unique<actions::SelectRoomCard>(
          state.ui.isFleeSelected ? -1 : state.ui.cursorInds.room));
    }
  } else if (state.ui.inputMode == InputMode::END_GAME_SCREEN) {
    state.ui.soundsToPlay.push_back("ui_select");
    DISPATCH_ACTION(actions::SetInputModeMenu);
  } else if (state.ui.inputMode == InputMode::MENU) {
    if (key == "Escape") {
      return;
    }
    DISPATCH_ACTION(actions::StartGame);
  }
}

void GameManager::update(int dt) {
  std::vector<VisualCard*> allCards;
  for (auto& vCard : state.discard) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.stack) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.room) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.weaponDefeated) {
    allCards.push_back(&vCard);
  }
  for (auto vCard : allCards) {
    transform::update(vCard->pos, dt);
  }
  if (state.currentWeapon.has_value()) {
    transform::update(state.currentWeapon.value().pos, dt);
  }

  transform::update(state.ui.cursor.pos, dt);

  if (!state.asyncActions.empty()) {
    AsyncAction& delayedAction = state.asyncActions.front();
    if (delayedAction.action != nullptr) {
      delayedAction.action->execute(&state);
      delayedAction.action = nullptr;
    }
    timer::update(delayedAction.timer, dt);
    if (timer::isComplete(delayedAction.timer)) {
      state.asyncActions.erase(state.asyncActions.begin());
    }
  }

  if (state.ui.inputMode == InputMode::MENU) {
    state.ui.menuCardsT += dt;
  }

  Dispatch::get().update();

  if (!state.ui.soundsToPlay.empty()) {
    for (const std::string& sound : state.ui.soundsToPlay) {
      window.playSound(sound);
    }
    state.ui.soundsToPlay.clear();
  }
}

void GameManager::render() {
  if (state.ui.inputMode == InputMode::MENU) {
    r.renderMenuScreen(state.ui.titleMessage,
                       state.ui.instructionMessage,
                       state.ui.menuCardsT);
    return;
  }

  if (state.ui.inputMode == InputMode::ROOM) {
    r.renderWeaponUi(state.currentWeapon);
    r.renderFleeUi(state.room.size() < 4 || state.ui.fleeCtr > 0);
  }

  r.renderHealthUi(
      state.playerHealth, state.playerHealthMax, state.ui.actionPreviewData);
  r.renderDeckNumber(static_cast<int>(state.stack.size()));

  std::vector<VisualCard*> allCards;
  for (auto& vCard : state.discard) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.stack) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.room) {
    allCards.push_back(&vCard);
  }
  for (auto& vCard : state.weaponDefeated) {
    allCards.push_back(&vCard);
  }
  for (auto vCard : allCards) {
    r.renderCard(*vCard);
  }

  if (state.currentWeapon.has_value()) {
    r.renderWeaponCard(state.currentWeapon.value(), state.weaponDefeated);
  }
  if (state.ui.renderFistAttack) {
    window.drawSprite("card_fist_0",
                      WEAPON_UI_POS.first,
                      WEAPON_UI_POS.second,
                      true,
                      0,
                      {CARD_SCALE, CARD_SCALE});
  }

  if (state.ui.inputMode == InputMode::CONFIRM) {
    r.renderConfirm(state.ui.confirmData, state.ui.cursorInds.confirm);
    if (state.ui.confirmData.type != PICK_WEAPON_TO_ATTACK_WITH) {
      r.renderWeaponUi(state.currentWeapon);
    }
  }

  if (state.ui.cursor.visible) {
    r.renderCursor(state.ui.cursor);
  }

  if (state.ui.inputMode == InputMode::END_GAME_SCREEN) {
    r.renderEndGameScreen(state.ui.titleMessage, state.ui.instructionMessage);
  }
}

} // namespace scoundrel