#pragma once
#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/EndRound.hpp"
#include "game/actions/ui/SetInputModeNone.hpp"
#include "game/utils/transform.hpp"

namespace scoundrel {
namespace actions {

class EquipWeapon : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    for (const auto& card : stateRef.weaponDefeated) {
      stateRef.discard.push_back(card);
    }
    const bool hadPreviousCard = stateRef.currentWeapon.has_value();
    if (hadPreviousCard) {
      stateRef.discard.push_back(stateRef.currentWeapon.value());
    }
    stateRef.weaponDefeated = {};
    stateRef.currentWeapon = stateRef.room[roomInd];
    stateRef.room.erase(stateRef.room.begin() + roomInd);

    if (hadPreviousCard) {
      transform::moveTo(stateRef.discard.back().pos, CARD_DISCARD_POS, 100);
    }

    VisualCard& vCard = stateRef.currentWeapon.value();

    stateRef.ui.soundsToPlay.push_back("equip_weapon");

    DISPATCH_ACTION(SetInputModeNone);

    transform::moveTo(vCard.pos, CARD_CENTER_SCREEN_POS, 100);
    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{100.}});

    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{400.}});

    stateRef.asyncActions.push_back(
        AsyncAction{std::make_unique<EndRound>(), timer::Timer{1.}});
  }

public:
  int roomInd = 0;

  EquipWeapon(int roomInd) : roomInd(roomInd) {}
};

} // namespace actions
} // namespace scoundrel