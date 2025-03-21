#pragma once

#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/EndRound.hpp"
#include "game/actions/stateHelpers.hpp"
#include "game/actions/ui/SetInputModeNone.hpp"

namespace program {
namespace actions {

class Flee : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    while (stateRef.room.size() > 0) {
      const int randI = rand() % static_cast<int>(stateRef.room.size());
      stateRef.stack.insert(stateRef.stack.begin(), stateRef.room[randI]);
      stateRef.room.erase(stateRef.room.begin() + randI);
      auto& stackCard = stateRef.stack.front();
      stackCard.faceDown = true;
    }

    stateRef.ui.fleeCtr += 2;
    stateRef.ui.soundsToPlay.push_back("flee");

    DISPATCH_ACTION(SetInputModeNone);

    resetCardPositions(stateRef);

    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{600.}});

    stateRef.asyncActions.push_back(
        AsyncAction{std::make_unique<EndRound>(), timer::Timer{1.}});
  }
};

} // namespace actions
} // namespace program