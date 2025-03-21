#pragma once

#include "game/state.h"
#include "game/actions/AbstractAction.h"

namespace program {
namespace actions {

class HoverFleeButtonInRoom : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    stateRef.ui.isFleeSelected = true;
    stateRef.ui.cursor.type = BUTTON;

    transform::moveTo(stateRef.ui.cursor.pos, FLEE_UI_POS, 100);
  }
};

} // namespace actions
} // namespace program