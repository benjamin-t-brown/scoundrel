#pragma once
#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/ui/SetInputModeConfirm.hpp"

namespace program {
namespace actions {

class SelectRoomCard : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    if (roomInd < 0) {
      if (stateRef.room.size() < 4 || stateRef.ui.fleeCtr > 0) {
        stateRef.ui.soundsToPlay.push_back("cancel");
        return;
      }

      Dispatch::get().addAction(
          std::make_unique<SetInputModeConfirm>(roomInd, CONFIRM_FLEE));
      return;
    }

    VisualCard& vCard = stateRef.room[roomInd];

    std::pair<int, int> nextPos = {vCard.pos.x, vCard.pos.y + 10};
    transform::moveTo(vCard.pos, nextPos, 100);

    stateRef.ui.soundsToPlay.push_back("ui_select");

    if (vCard.card.suit == HEARTS) {
      Dispatch::get().addAction(
          std::make_unique<SetInputModeConfirm>(roomInd, CONFIRM_HEAL));
    } else if (vCard.card.suit == DIAMONDS) {
      Dispatch::get().addAction(std::make_unique<SetInputModeConfirm>(
          roomInd, CONFIRM_CHOOSE_WEAPON));
    } else {
      Dispatch::get().addAction(std::make_unique<SetInputModeConfirm>(
          roomInd, PICK_WEAPON_TO_ATTACK_WITH));
    }
  }

public:
  int roomInd;
  SelectRoomCard(int roomInd) : roomInd(roomInd) {}
};

} // namespace actions
} // namespace program