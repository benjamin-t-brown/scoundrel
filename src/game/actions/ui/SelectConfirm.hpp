#pragma once

#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/game/AttackMonster.hpp"
#include "game/actions/game/EquipWeapon.hpp"
#include "game/actions/game/Flee.hpp"
#include "game/actions/game/Heal.hpp"
#include "game/actions/ui/SetInputModeRoom.hpp"

namespace scoundrel {
namespace actions {

class SelectConfirm : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    int roomInd = stateRef.ui.confirmData.confirmCardRoomInd;

    if (selectInd == 0 && stateRef.ui.confirmData.leftDisabled) {
      stateRef.ui.soundsToPlay.push_back("cancel");
      return;
    }
    if (selectInd == 1 && stateRef.ui.confirmData.rightDisabled) {
      stateRef.ui.soundsToPlay.push_back("cancel");
      return;
    }

    stateRef.ui.soundsToPlay.push_back("ui_select");

    switch (stateRef.ui.confirmData.type) {
    case CONFIRM_FLEE:
      if (selectInd == 1) {
        Dispatch::get().addAction(std::make_unique<SetInputModeRoom>());
        break;
      }
      Dispatch::get().addAction(std::make_unique<Flee>());
      break;
    case CONFIRM_HEAL:
      if (selectInd == 1) {
        Dispatch::get().addAction(std::make_unique<SetInputModeRoom>());
        break;
      }
      Dispatch::get().addAction(std::make_unique<Heal>(roomInd));
      break;
    case CONFIRM_CHOOSE_WEAPON:
      if (selectInd == 1) {
        Dispatch::get().addAction(std::make_unique<SetInputModeRoom>());
        break;
      }
      Dispatch::get().addAction(std::make_unique<EquipWeapon>(roomInd));
      break;
    case PICK_WEAPON_TO_ATTACK_WITH:
      // fist
      if (selectInd == 1) {
        Dispatch::get().addAction(
            std::make_unique<AttackMonster>(roomInd, false));
      } else {
        Dispatch::get().addAction(
            std::make_unique<AttackMonster>(roomInd, true));
      }
      break;
    }
  }

  int selectInd;

public:
  SelectConfirm(int selectInd) : selectInd(selectInd) {}
};

} // namespace actions
} // namespace scoundrel