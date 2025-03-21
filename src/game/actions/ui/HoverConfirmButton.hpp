#pragma once

#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/calculations.hpp"
#include <utility>

namespace scoundrel {
namespace actions {

class HoverConfirmButton : public AbstractAction {
protected:
  int ind = 0;

  void act() override {
    State& stateRef = *state;
    stateRef.ui.cursorInds.confirm = ind;
    if (ind < 0) {
      stateRef.ui.cursorInds.confirm = 0;
    } else if (ind > 1) {
      stateRef.ui.cursorInds.confirm = 1;
    }

    std::pair<int, int> indPos =
        stateRef.ui.confirmData.type == ConfirmType::PICK_WEAPON_TO_ATTACK_WITH
            ? stateRef.ui.cursorInds.confirm == 0
                  ? std::make_pair(CONFIRM_CARD_LEFT_POS.first -
                                       CARD_WIDTH * CARD_SCALE / 2,
                                   CONFIRM_CARD_LEFT_POS.second -
                                       CARD_HEIGHT * CARD_SCALE / 2)
                  : std::make_pair(CONFIRM_CARD_RIGHT_POS.first -
                                       CARD_WIDTH * CARD_SCALE / 2,
                                   CONFIRM_CARD_RIGHT_POS.second -
                                       CARD_HEIGHT * CARD_SCALE / 2)
        : stateRef.ui.cursorInds.confirm == 0 ? CONFIRM_LEFT_POS
                                              : CONFIRM_RIGHT_POS;

    if (stateRef.ui.confirmData.type ==
        ConfirmType::PICK_WEAPON_TO_ATTACK_WITH) {
      stateRef.ui.actionPreviewData.visible = true;
      stateRef.ui.actionPreviewData.nextHp = calcNextHealthAfterAttack(
          stateRef.playerHealth,
          stateRef.room[stateRef.ui.confirmData.confirmCardRoomInd],
          stateRef.ui.cursorInds.confirm == 0 ? stateRef.currentWeapon
                                              : std::nullopt);
    } else if (stateRef.ui.confirmData.type == ConfirmType::CONFIRM_HEAL) {
      stateRef.ui.actionPreviewData.visible = true;
      stateRef.ui.actionPreviewData.nextHp = calcNextHealthAfterHealCard(
          stateRef.playerHealth,
          stateRef.playerHealthMax,
          stateRef.room[stateRef.ui.confirmData.confirmCardRoomInd]);
    } else {
      stateRef.ui.actionPreviewData.visible = false;
    }

    transform::moveTo(stateRef.ui.cursor.pos, indPos, 100);
  }

public:
  HoverConfirmButton(int ind) : ind(ind) {}
};

} // namespace actions
} // namespace scoundrel