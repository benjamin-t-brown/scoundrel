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
#include "game/utils/cardanim.hpp"
#include "game/utils/transform.hpp"
#include "lib/sdl2wrapper/AssetLoader.h"
#include "lib/sdl2wrapper/Window.h"

namespace program {
GameManager::GameManager(sdl2w::Window& windowA)
    : window(windowA), r(Render(windowA)) {
  Dispatch::init(&state);
}

GameManager::~GameManager() {}

void GameManager::load() {
  sdl2w::loadAssetsFromFile("sprite", "assets/resSprites.txt");
  sdl2w::loadAssetsFromFile("sound", "assets/resSounds.txt");
  auto hiscores = hiscore::getHighScores();
  if (hiscores.size()) {
    state.wins = hiscores[0].score;
  }
}

void GameManager::start() {
  DISPATCH_ACTION(actions::SetInputModeMenu);
  libhtml::notifyGameReady();
  state.ui.soundsToPlay.push_back("start");
}

void GameManager::handleKeyPress(const std::string& key) {
  if (state.ui.inputMode == InputMode::CONFIRM) {
    if (isLeftKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      int nextInd = state.ui.cursorInds.confirm == 2
                        ? 2
                        : state.ui.cursorInds.confirm - 1;
      Dispatch::get().addAction(
          std::make_unique<actions::HoverConfirmButton>(nextInd));
    } else if (isRightKey(key)) {
      state.ui.soundsToPlay.push_back("ui_hover");
      int nextInd = state.ui.cursorInds.confirm == 2
                        ? 2
                        : state.ui.cursorInds.confirm + 1;
      Dispatch::get().addAction(
          std::make_unique<actions::HoverConfirmButton>(nextInd));
    } else if ((isDownKey(key) || isUpKey(key)) &&
               state.ui.confirmData.type == PICK_WEAPON_TO_ATTACK_WITH) {
      state.ui.soundsToPlay.push_back("ui_hover");
      int nextInd = state.ui.cursorInds.confirm == 2
                        ? 0
                        : 2;
      Dispatch::get().addAction(
          std::make_unique<actions::HoverConfirmButton>(nextInd));
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

namespace {
bool hitTestCard(int mx, int my, const VisualCard& card) {
  const int w = CARD_WIDTH * CARD_SCALE;
  const int h = CARD_HEIGHT * CARD_SCALE;
  return mx >= card.pos.x && mx < card.pos.x + w && my >= card.pos.y &&
         my < card.pos.y + h;
}

bool hitTestCentered(int mx, int my, int cx, int cy, int halfW, int halfH) {
  return mx >= cx - halfW && mx < cx + halfW && my >= cy - halfH &&
         my < cy + halfH;
}
} // namespace

void GameManager::handleMouseClick(int x, int y) {
  if (state.ui.inputMode == InputMode::MENU) {
    DISPATCH_ACTION(actions::StartGame);
  } else if (state.ui.inputMode == InputMode::ROOM) {
    for (size_t i = 0; i < state.room.size(); i++) {
      if (hitTestCard(x, y, state.room[i])) {
        state.ui.soundsToPlay.push_back("ui_select");
        Dispatch::get().addAction(
            std::make_unique<actions::HoverCardInRoom>(static_cast<int>(i)));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectRoomCard>(static_cast<int>(i)));
        return;
      }
    }
    const int btnHalfW = BUTTON_WIDTH * CARD_SCALE / 2;
    const int btnHalfH = BUTTON_HEIGHT * CARD_SCALE / 2;
    if (hitTestCentered(
            x, y, FLEE_UI_POS.first, FLEE_UI_POS.second, btnHalfW, btnHalfH)) {
      state.ui.soundsToPlay.push_back("ui_select");
      Dispatch::get().addAction(
          std::make_unique<actions::HoverFleeButtonInRoom>());
      Dispatch::get().addAction(
          std::make_unique<actions::SelectRoomCard>(-1));
      return;
    }
  } else if (state.ui.inputMode == InputMode::CONFIRM) {
    if (state.ui.confirmData.type == PICK_WEAPON_TO_ATTACK_WITH) {
      const int cardHalfW = CARD_WIDTH * CARD_SCALE / 2;
      const int cardHalfH = CARD_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_CARD_LEFT_POS.first,
                          CONFIRM_CARD_LEFT_POS.second,
                          cardHalfW,
                          cardHalfH)) {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverConfirmButton>(0));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectConfirm>(0));
        return;
      }
      if (hitTestCentered(x,
                          y,
                          CONFIRM_CARD_RIGHT_POS.first,
                          CONFIRM_CARD_RIGHT_POS.second,
                          cardHalfW,
                          cardHalfH)) {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverConfirmButton>(1));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectConfirm>(1));
        return;
      }
      const int btnHalfW2 = BUTTON_WIDTH * CARD_SCALE / 2;
      const int btnHalfH2 = BUTTON_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_BACK_POS.first,
                          CONFIRM_BACK_POS.second,
                          btnHalfW2,
                          btnHalfH2)) {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverConfirmButton>(2));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectConfirm>(2));
        return;
      }
    } else {
      const int btnHalfW = BUTTON_WIDTH * CARD_SCALE / 2;
      const int btnHalfH = BUTTON_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_LEFT_POS.first,
                          CONFIRM_LEFT_POS.second,
                          btnHalfW,
                          btnHalfH)) {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverConfirmButton>(0));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectConfirm>(0));
        return;
      }
      if (hitTestCentered(x,
                          y,
                          CONFIRM_RIGHT_POS.first,
                          CONFIRM_RIGHT_POS.second,
                          btnHalfW,
                          btnHalfH)) {
        Dispatch::get().addAction(
            std::make_unique<actions::HoverConfirmButton>(1));
        Dispatch::get().addAction(
            std::make_unique<actions::SelectConfirm>(1));
        return;
      }
    }
  } else if (state.ui.inputMode == InputMode::END_GAME_SCREEN) {
    state.ui.soundsToPlay.push_back("ui_select");
    DISPATCH_ACTION(actions::SetInputModeMenu);
  }
}

void GameManager::handleMouseMove(int x, int y) {
  if (state.ui.inputMode == InputMode::ROOM) {
    for (size_t i = 0; i < state.room.size(); i++) {
      if (hitTestCard(x, y, state.room[i])) {
        int ind = static_cast<int>(i);
        if (state.ui.cursorInds.room != ind || state.ui.isFleeSelected) {
          state.ui.soundsToPlay.push_back("ui_hover");
          Dispatch::get().addAction(
              std::make_unique<actions::HoverCardInRoom>(ind));
        }
        return;
      }
    }
    const int btnHalfW = BUTTON_WIDTH * CARD_SCALE / 2;
    const int btnHalfH = BUTTON_HEIGHT * CARD_SCALE / 2;
    if (hitTestCentered(
            x, y, FLEE_UI_POS.first, FLEE_UI_POS.second, btnHalfW, btnHalfH)) {
      if (!state.ui.isFleeSelected) {
        state.ui.soundsToPlay.push_back("ui_hover");
        Dispatch::get().addAction(
            std::make_unique<actions::HoverFleeButtonInRoom>());
      }
      return;
    }
  } else if (state.ui.inputMode == InputMode::CONFIRM) {
    int hitInd = -1;
    if (state.ui.confirmData.type == PICK_WEAPON_TO_ATTACK_WITH) {
      const int cardHalfW = CARD_WIDTH * CARD_SCALE / 2;
      const int cardHalfH = CARD_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_CARD_LEFT_POS.first,
                          CONFIRM_CARD_LEFT_POS.second,
                          cardHalfW,
                          cardHalfH)) {
        hitInd = 0;
      } else       if (hitTestCentered(x,
                                 y,
                                 CONFIRM_CARD_RIGHT_POS.first,
                                 CONFIRM_CARD_RIGHT_POS.second,
                                 cardHalfW,
                                 cardHalfH)) {
        hitInd = 1;
      }
      const int btnHalfW2 = BUTTON_WIDTH * CARD_SCALE / 2;
      const int btnHalfH2 = BUTTON_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_BACK_POS.first,
                          CONFIRM_BACK_POS.second,
                          btnHalfW2,
                          btnHalfH2)) {
        hitInd = 2;
      }
    } else {
      const int btnHalfW = BUTTON_WIDTH * CARD_SCALE / 2;
      const int btnHalfH = BUTTON_HEIGHT * CARD_SCALE / 2;
      if (hitTestCentered(x,
                          y,
                          CONFIRM_LEFT_POS.first,
                          CONFIRM_LEFT_POS.second,
                          btnHalfW,
                          btnHalfH)) {
        hitInd = 0;
      } else if (hitTestCentered(x,
                                 y,
                                 CONFIRM_RIGHT_POS.first,
                                 CONFIRM_RIGHT_POS.second,
                                 btnHalfW,
                                 btnHalfH)) {
        hitInd = 1;
      }
    }
    if (hitInd >= 0 && state.ui.cursorInds.confirm != hitInd) {
      state.ui.soundsToPlay.push_back("ui_hover");
      Dispatch::get().addAction(
          std::make_unique<actions::HoverConfirmButton>(hitInd));
    }
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
    cardanim::updateCardAnim(vCard->anim, dt);
  }
  if (state.currentWeapon.has_value()) {
    transform::update(state.currentWeapon.value().pos, dt);
    cardanim::updateCardAnim(state.currentWeapon.value().anim, dt);
  }

  transform::update(state.ui.cursor.pos, dt);
  cardanim::updateCardAnim(state.ui.fistAnim, dt);
  cardanim::updateCardAnim(state.ui.heartAnim, dt);

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
                       state.wins,
                       state.ui.menuCardsT);
    return;
  }

  if (state.ui.inputMode == InputMode::ROOM) {
    r.renderWeaponUi(state.currentWeapon);
    r.renderFleeUi(state.room.size() < 4 || state.ui.fleeCtr > 0);
  }

  r.renderHealthUi(state.playerHealth,
                    state.playerHealthMax,
                    state.ui.actionPreviewData,
                    state.ui.heartAnim);
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
    double fistOffsetX = cardanim::getAnimOffsetX(state.ui.fistAnim);
    double fistOffsetY = cardanim::getAnimOffsetY(state.ui.fistAnim);
    window.drawSprite("card_fist_0",
                      WEAPON_UI_POS.first + fistOffsetX,
                      WEAPON_UI_POS.second + fistOffsetY,
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

} // namespace program