#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "lib/sdl2wrapper/Localization.h"

namespace program {
namespace actions {

class SetInputModeMenu : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursor.visible = false;
    stateRef.ui.cursor.type = CARD;
    stateRef.ui.inputMode = MENU;
    stateRef.ui.actionPreviewData.visible = false;
    stateRef.ui.titleMessage =
        sdl2w::L10n::trans(LOCSTR("Scoundrel"));
    stateRef.ui.instructionMessage = sdl2w::L10n::trans(
        LOCSTR("Press any button."));
  }
};

} // namespace actions
} // namespace program