#pragma once

#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/EndRound.hpp"
#include "game/actions/depictions/ModifyPlayerHp.hpp"
#include "game/actions/ui/SetInputModeNone.hpp"

namespace program {
namespace actions {

class Heal : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    int nextHp = stateRef.playerHealth;
    nextHp += stateRef.room[roomInd].card.value;
    if (nextHp > stateRef.playerHealthMax) {
      nextHp = stateRef.playerHealthMax;
    }
    stateRef.discard.push_back(stateRef.room[roomInd]);
    stateRef.room.erase(stateRef.room.begin() + roomInd);

    VisualCard& vCard = stateRef.discard.back();

    stateRef.ui.soundsToPlay.push_back("drink_potion");

    DISPATCH_ACTION(SetInputModeNone);

    transform::moveTo(vCard.pos, CARD_CENTER_SCREEN_POS, 100);
    transform::moveTo(stateRef.ui.cursor.pos, CARD_CENTER_SCREEN_POS, 100);
    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{100.}});

    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{400.}});

    int currentPlayerHp = stateRef.playerHealth;
    int hpDiff = nextHp - currentPlayerHp;
    for (int i = 0; i < hpDiff; i++) {
      stateRef.asyncActions.push_back(
          AsyncAction{std::make_unique<ModifyPlayerHp>(1), timer::Timer{33.}});
    }

    stateRef.asyncActions.push_back(
        AsyncAction{std::make_unique<EndRound>(), timer::Timer{1.}});
  }

public:
  int roomInd = 0;

  Heal(int roomInd) : roomInd(roomInd) {}
};

} // namespace actions
} // namespace program