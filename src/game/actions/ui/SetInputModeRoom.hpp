#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/stateHelpers.hpp"
#include "game/actions/ui/HoverCardInRoom.hpp"
#include "game/Dispatch.h"

namespace scoundrel {
namespace actions {

class SetInputModeRoom : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursor.visible = true;
    stateRef.ui.cursor.type = CARD;
    stateRef.ui.inputMode = ROOM;
    stateRef.ui.actionPreviewData.visible = false;
    stateRef.ui.cursorInds.confirm = 0;
    resetCardPositions(stateRef);
    Dispatch::get().addAction(
        std::make_unique<HoverCardInRoom>(stateRef.ui.cursorInds.room));
  }
};

} // namespace actions
} // namespace scoundrel