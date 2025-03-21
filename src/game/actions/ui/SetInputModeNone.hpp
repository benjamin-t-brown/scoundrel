#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"

namespace program {
namespace actions {

class SetInputModeNone : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursor.visible = false;
    stateRef.ui.inputMode = NONE;
    stateRef.ui.actionPreviewData.visible = false;
    stateRef.ui.cursorInds.confirm = 0;
  }
};

} // namespace actions
} // namespace program