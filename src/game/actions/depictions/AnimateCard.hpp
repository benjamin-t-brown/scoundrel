#pragma once

#include "game/actions/AbstractAction.h"
#include "game/state.h"
#include "game/utils/cardanim.hpp"

namespace program {
namespace actions {

enum CardLocation {
  LOC_DISCARD_BACK,
  LOC_WEAPON_DEFEATED,
  LOC_WEAPON,
  LOC_FIST
};

class AnimateCard : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;
    VisualCard* card = nullptr;

    switch (location) {
    case LOC_DISCARD_BACK:
      if (!stateRef.discard.empty()) {
        card = &stateRef.discard.back();
      }
      break;
    case LOC_WEAPON_DEFEATED:
      if (index >= 0 &&
          index < static_cast<int>(stateRef.weaponDefeated.size())) {
        card = &stateRef.weaponDefeated[index];
      }
      break;
    case LOC_WEAPON:
      if (stateRef.currentWeapon.has_value()) {
        card = &stateRef.currentWeapon.value();
      }
      break;
    case LOC_FIST: {
      cardanim::CardAnim& fist = stateRef.ui.fistAnim;
      if (animType == cardanim::CardAnim::NUDGE) {
        cardanim::startNudge(fist, duration, nudgeX, nudgeY);
      } else if (animType == cardanim::CardAnim::BUMP) {
        cardanim::startBump(fist, duration, intensity);
      } else if (animType == cardanim::CardAnim::SHAKE) {
        cardanim::startShake(fist, duration, intensity);
      }
      return;
    }
    }

    if (card == nullptr) {
      return;
    }

    if (animType == cardanim::CardAnim::BUMP) {
      cardanim::startBump(card->anim, duration, intensity);
    } else if (animType == cardanim::CardAnim::SHAKE) {
      cardanim::startShake(card->anim, duration, intensity);
    } else if (animType == cardanim::CardAnim::NUDGE) {
      cardanim::startNudge(card->anim, duration, nudgeX, nudgeY);
    }
  }

public:
  CardLocation location;
  int index;
  cardanim::CardAnim::Type animType;
  double duration;
  double intensity;
  double nudgeX = 0;
  double nudgeY = 0;

  AnimateCard(CardLocation location,
              int index,
              cardanim::CardAnim::Type animType,
              double duration,
              double intensity)
      : location(location), index(index), animType(animType),
        duration(duration), intensity(intensity) {}

  AnimateCard(CardLocation location,
              int index,
              cardanim::CardAnim::Type animType,
              double duration,
              double nudgeX,
              double nudgeY)
      : location(location), index(index), animType(animType),
        duration(duration), intensity(1.0), nudgeX(nudgeX), nudgeY(nudgeY) {}
};

} // namespace actions
} // namespace program
