#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "lib/sdl2wrapper/Localization.h"

namespace scoundrel {
namespace actions {

class SetInputModeEndGameScreen : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursor.visible = false;
    stateRef.ui.cursor.type = CARD;
    stateRef.ui.inputMode = END_GAME_SCREEN;
    stateRef.ui.actionPreviewData.visible = false;
    stateRef.isVictory = isVictory;
    stateRef.ui.titleMessage =
        isVictory
            ? sdl2w::L10n::trans(LOCSTR("You win!"))
            : sdl2w::L10n::trans(LOCSTR("You are defeated."));
    stateRef.ui.instructionMessage = sdl2w::L10n::trans(
        LOCSTR("Press any button to continue."));
    if (isVictory) {
      stateRef.ui.soundsToPlay.push_back("victory");
    } else {
      stateRef.ui.soundsToPlay.push_back("defeat");
    }
  }

public:
  bool isVictory;
  SetInputModeEndGameScreen(bool isVictory) : isVictory(isVictory) {}
};

} // namespace actions
} // namespace scoundrel