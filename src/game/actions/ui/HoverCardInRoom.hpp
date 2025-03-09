#pragma once
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/calculations.hpp"

namespace scoundrel {
namespace actions {

class HoverCardInRoom : public AbstractAction {
protected:
  int ind = 0;

  void act() override {
    State& stateRef = *state;

    if (stateRef.room.size() == 0) {
      return;
    }

    stateRef.ui.cursorInds.room = ind;
    while (stateRef.ui.cursorInds.room < 0) {
      stateRef.ui.cursorInds.room += stateRef.room.size();
    }
    stateRef.ui.cursorInds.room %= stateRef.room.size();
    stateRef.ui.isFleeSelected = false;
    stateRef.ui.cursor.type = CARD;

    VisualCard& vCard = stateRef.room[stateRef.ui.cursorInds.room];
    if (vCard.card.suit == Suit::HEARTS) {
      stateRef.ui.actionPreviewData.visible = true;
      stateRef.ui.actionPreviewData.nextHp = calcNextHealthAfterHealCard(
          stateRef.playerHealth, stateRef.playerHealthMax, vCard);
    } else if (vCard.card.suit == Suit::SPADES ||
               vCard.card.suit == Suit::CLUBS) {
      stateRef.ui.actionPreviewData.visible = true;
      if (stateRef.currentWeapon.has_value()) {
        int cardAttackableValue = calcWeaponAttackableValue(
            stateRef.currentWeapon.value(), stateRef.weaponDefeated);
        if (cardAttackableValue > vCard.card.value) {
          stateRef.ui.actionPreviewData.nextHp = calcNextHealthAfterAttack(
              stateRef.playerHealth, vCard, stateRef.currentWeapon);
        } else {
          const std::optional<VisualCard> nullOpt = std::nullopt;
          stateRef.ui.actionPreviewData.nextHp =
              calcNextHealthAfterAttack(stateRef.playerHealth, vCard, nullOpt);
        }
      } else {
        stateRef.ui.actionPreviewData.nextHp = calcNextHealthAfterAttack(
            stateRef.playerHealth, vCard, stateRef.currentWeapon);
      }
    } else {
      stateRef.ui.actionPreviewData.visible = false;
    }

    std::pair<int, int> indPos = {
        stateRef.room[stateRef.ui.cursorInds.room].pos.nextX,
        stateRef.room[stateRef.ui.cursorInds.room].pos.nextY};
    transform::moveTo(stateRef.ui.cursor.pos, indPos, 100);
  }

public:
  HoverCardInRoom(int ind) : ind(ind) {}
};

} // namespace actions
} // namespace scoundrel