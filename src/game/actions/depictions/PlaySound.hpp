#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"

namespace scoundrel {
namespace actions {

class PlaySound : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    stateRef.ui.soundsToPlay.push_back(soundName);
  }

public:
  std::string soundName;

  PlaySound(const std::string& soundName) : soundName(soundName) {}
};

} // namespace actions
} // namespace scoundrel