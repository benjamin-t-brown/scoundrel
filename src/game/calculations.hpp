#pragma once

#include "game/state.h"

namespace scoundrel {

inline int calcNextHealthAfterHealCard(int hp, int maxHp, VisualCard healCard) {
  int healValue = healCard.card.value;
  return std::min(hp + healValue, maxHp);
}

inline int calcWeaponCardValue(const VisualCard& weaponCard) {
  return weaponCard.card.value;
}

inline int
calcWeaponAttackableValue(const VisualCard& weaponCard,
                          const std::vector<VisualCard>& defeatedEnemies) {
  if (defeatedEnemies.empty()) {
    return 15;
  }
  return defeatedEnemies.back().card.value;
}

inline int
calcNextHealthAfterAttack(int hp,
                          const VisualCard& monsterCard,
                          const std::optional<VisualCard>& weaponCard) {
  int nextHp = 0;
  if (weaponCard.has_value()) {
    int weaponValue = calcWeaponCardValue(weaponCard.value());
    nextHp = hp - std::max(0, monsterCard.card.value - weaponValue);
  } else {
    nextHp = hp - monsterCard.card.value;
  }
  if (nextHp < 0) {
    nextHp = 0;
  }
  return nextHp;
}

} // namespace scoundrel