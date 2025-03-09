#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "logger.h"

namespace scoundrel {
namespace actions {

class DrawCardsForNewRoom : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    int numCardsToDraw = 3;
    if (stateRef.room.size() == 0) {
      numCardsToDraw = 4;
    }

    const int numCardsRemaining = static_cast<int>(stateRef.stack.size());

    if (numCardsRemaining < numCardsToDraw) {
      numCardsToDraw = numCardsRemaining;
    }

    if (numCardsRemaining <= 0) {
      logger::error("Cannot draw any more cards.  Deck is insufficient.");
      return;
    }

    for (int i = 0; i < numCardsToDraw; i++) {
      stateRef.room.push_back(stateRef.stack.back());
      stateRef.stack.pop_back();
      auto& roomCard = stateRef.room.back();
      roomCard.faceDown = false;
    }

    // stateRef.ui.soundsToPlay.push_back("draw_card");
  }
};

} // namespace actions
} // namespace scoundrel