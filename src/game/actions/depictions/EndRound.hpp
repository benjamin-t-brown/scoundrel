#pragma once

#include "LibHTML.h"
#include "game/Dispatch.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/DrawCardsForNewRoom.hpp"
#include "game/actions/ui/SetInputModeEndGameScreen.hpp"
#include "game/actions/ui/SetInputModeRoom.hpp"
#include "game/state.h"

namespace scoundrel {
namespace actions {

class EndRound : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursorInds.room = 0;
    stateRef.ui.renderFistAttack = false;

    if (stateRef.playerHealth <= 0) {
      // defeat
      libhtml::notifyGameCompleted(false);
      DISPATCH_ACTION(SetInputModeEndGameScreen, false);
      return;
    }

    if (stateRef.room.size() <= 1 && stateRef.stack.size() == 0) {
      // victory!
      libhtml::notifyGameCompleted(true);
      DISPATCH_ACTION(SetInputModeEndGameScreen, true);
      return;
    }

    stateRef.ui.soundsToPlay.push_back("draw_card");
    if (stateRef.ui.fleeCtr > 0) {
      stateRef.ui.fleeCtr--;
    }

    if (stateRef.room.size() <= 1) {
      DISPATCH_ACTION(DrawCardsForNewRoom);
    }

    DISPATCH_ACTION(SetInputModeRoom);
  }
};

} // namespace actions
} // namespace scoundrel