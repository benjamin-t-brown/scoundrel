#pragma once
#include "game/actions/AbstractAction.h"
#include "game/state.h"
#include "lib/sdl2wrapper/Logger.h"

namespace program {
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
      sdl2w::Logger().get(sdl2w::LogType::ERROR)
          << "Dispatch::init window is nullptr" << sdl2w::Logger::endl;
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
} // namespace program