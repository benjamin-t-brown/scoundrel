#pragma once

#include "game/state.h"
#include "game/utils/transform.hpp"

namespace program {

inline void resetCardPositions(State& stateRef) {
  const int spacing = 4;
  const int topOffset = 4;
  const int leftOffset =
      WINDOW_WIDTH - (CARD_WIDTH * CARD_SCALE + spacing) * 4 - 6;
  for (size_t i = 0; i < stateRef.room.size(); i++) {
    auto& roomCard = stateRef.room[i];
    transform::moveTo(
        roomCard.pos,
        {leftOffset + (i) * (CARD_WIDTH * CARD_SCALE + spacing), topOffset},
        300);
  }

  if (stateRef.currentWeapon.has_value()) {
    transform::moveTo(stateRef.currentWeapon.value().pos, WEAPON_CARD_POS, 300);
  }
  int minX = CARD_WEAPON_DEFEATED_POS.first;
  int maxX = WINDOW_WIDTH - CARD_WIDTH * CARD_SCALE - 4;
  int calcDist = transform::normalize(
      1,
      0,
      std::max(static_cast<int>(stateRef.weaponDefeated.size()), 1),
      0,
      maxX - minX);
  for (size_t i = 0; i < stateRef.weaponDefeated.size(); i++) {
    auto& weaponDefeatedCard = stateRef.weaponDefeated[i];
    transform::moveTo(
        weaponDefeatedCard.pos,
        {CARD_WEAPON_DEFEATED_POS.first +
             i * std::min((CARD_WIDTH * CARD_SCALE / 4 + 12), calcDist),
         WEAPON_CARD_POS.second},
        300);
  }
  for (size_t i = 0; i < stateRef.discard.size(); i++) {
    auto& discardCard = stateRef.discard[i];
    transform::moveTo(discardCard.pos,
                      {CARD_DISCARD_POS.first, CARD_DISCARD_POS.second},
                      300);
  }
  for (size_t i = 0; i < stateRef.stack.size(); i++) {
    auto& stackCard = stateRef.stack[i];
    transform::moveTo(stackCard.pos, CARD_STACK_POS, 300);
  }
}

} // namespace program
