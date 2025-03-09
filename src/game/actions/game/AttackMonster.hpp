#pragma once

#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/EndRound.hpp"
#include "game/actions/depictions/ModifyPlayerHp.hpp"
#include "game/actions/depictions/PlaySound.hpp"
#include "game/actions/ui/SetInputModeNone.hpp"
#include "game/calculations.hpp"

namespace scoundrel {
namespace actions {

class AttackMonster : public AbstractAction {
protected:
  void act() override {
    State& stateRef = *state;

    const bool usedWeapon = useWeapon;
    const std::optional<VisualCard> nullOpt = std::nullopt;
    const int nextHp =
        calcNextHealthAfterAttack(stateRef.playerHealth,
                                  stateRef.room[roomInd],
                                  useWeapon ? stateRef.currentWeapon : nullOpt);

    int indWeaponInserted = 0;
    bool didNoDamage = nextHp == stateRef.playerHealth;
    if (usedWeapon) {
      bool didPush = false;
      for (size_t i = 0; i < stateRef.weaponDefeated.size(); i++) {
        auto& weaponDefeatedCard = stateRef.weaponDefeated[i];
        if (weaponDefeatedCard.card.value < stateRef.room[roomInd].card.value) {
          stateRef.weaponDefeated.insert(stateRef.weaponDefeated.begin() + i,
                                         stateRef.room[roomInd]);
          indWeaponInserted = i;
          didPush = true;
          break;
        }
      }
      if (!didPush) {
        indWeaponInserted = stateRef.weaponDefeated.size();
        stateRef.weaponDefeated.push_back(stateRef.room[roomInd]);
      }
    } else {
      stateRef.ui.renderFistAttack = true;
      stateRef.discard.push_back(stateRef.room[roomInd]);
    }
    stateRef.room.erase(stateRef.room.begin() + roomInd);

    VisualCard& monsterCard = usedWeapon
                                  ? stateRef.weaponDefeated[indWeaponInserted]
                                  : stateRef.discard.back();

    DISPATCH_ACTION(SetInputModeNone);

    transform::moveTo(monsterCard.pos, CARD_CENTER_SCREEN_POS, 100);
    if (usedWeapon) {
      transform::moveTo(
          stateRef.currentWeapon.value().pos,
          {CARD_CENTER_SCREEN_POS.first,
           CARD_CENTER_SCREEN_POS.second + CARD_HEIGHT * CARD_SCALE + 10},
          100);
    } else if (stateRef.currentWeapon.has_value()) {
      transform::moveTo(stateRef.currentWeapon.value().pos,
                        {stateRef.currentWeapon.value().pos.x - 20,
                         stateRef.currentWeapon.value().pos.y + 100},
                        100);
    }
    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{100.}});

    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{400.}});
    int wormSoundInd = 1;
    if (monsterCard.card.value > 10) {
      wormSoundInd = 3;
    } else if (monsterCard.card.value > 5) {
      wormSoundInd = 2;
    }
    stateRef.asyncActions.push_back(AsyncAction{
        std::make_unique<PlaySound>("worm" + std::to_string(wormSoundInd)),
        timer::Timer{1}});

    if (didNoDamage) {
      stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{600.}});
      stateRef.asyncActions.push_back(AsyncAction{
          std::make_unique<PlaySound>("no_damage"), timer::Timer{400}});
    } else {
      stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{600.}});
    }

    int currentPlayerHp = stateRef.playerHealth;
    int hpDiff = currentPlayerHp - nextHp;
    for (int i = 0; i < hpDiff; i++) {
      stateRef.asyncActions.push_back(
          AsyncAction{std::make_unique<ModifyPlayerHp>(-1), timer::Timer{33.}});
    }

    stateRef.asyncActions.push_back(AsyncAction{nullptr, timer::Timer{400.}});

    stateRef.asyncActions.push_back(
        AsyncAction{std::make_unique<EndRound>(), timer::Timer{1.}});
  }

public:
  int roomInd = 0;
  bool useWeapon = false;

  AttackMonster(int roomInd, bool useWeapon)
      : roomInd(roomInd), useWeapon(useWeapon) {}
};

} // namespace actions
} // namespace scoundrel