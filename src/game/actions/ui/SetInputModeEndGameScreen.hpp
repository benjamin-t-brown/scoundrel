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
            ? SDL2Wrapper::Localization::trans(LOCSTR("You win!"))
            : SDL2Wrapper::Localization::trans(LOCSTR("You are defeated."));
    stateRef.ui.instructionMessage = SDL2Wrapper::Localization::trans(
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