#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"

namespace program {
namespace actions {

class ModifyPlayerHp : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    stateRef.playerHealth += modifyBy;
    if (stateRef.playerHealth > stateRef.playerHealthMax) {
      stateRef.playerHealth = stateRef.playerHealthMax;
    }
    if (stateRef.playerHealth < 0) {
      stateRef.playerHealth = 0;
    }

    stateRef.ui.soundsToPlay.push_back("modify_hp_short");
  }

public:
  int modifyBy = 0;

  ModifyPlayerHp(int modifyBy) : modifyBy(modifyBy) {}
};

} // namespace actions
} // namespace program