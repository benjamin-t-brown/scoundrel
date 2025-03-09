#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "lib/sdl2wrapper/Localization.h"

namespace scoundrel {
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
        SDL2Wrapper::Localization::trans(LOCSTR("Scoundrel"));
    stateRef.ui.instructionMessage = SDL2Wrapper::Localization::trans(
        LOCSTR("Press any button."));
  }
};

} // namespace actions
} // namespace scoundrel