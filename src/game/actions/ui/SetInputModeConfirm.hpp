#pragma once
#include "game/Dispatch.h"
#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/ui/HoverConfirmButton.hpp"
#include "game/calculations.hpp"
#include "lib/sdl2wrapper/Localization.h"

namespace scoundrel {
namespace actions {

class SetInputModeConfirm : public AbstractAction {
protected:
  std::string getMessage() {
    switch (type) {
    case CONFIRM_HEAL:
      return sdl2w::L10n::trans(LOCSTR("Use this card to heal?"));
    case CONFIRM_FLEE:
      return sdl2w::L10n::trans(
          LOCSTR("Are you sure you wish to flee?"));
    case CONFIRM_CHOOSE_WEAPON:
      return sdl2w::L10n::trans(LOCSTR("Pick up this weapon?"));
    case PICK_WEAPON_TO_ATTACK_WITH:
      return sdl2w::L10n::trans(
          LOCSTR("Attack with which weapon?"));
    default:
      return "";
    }
  }

  void act() override {
    State& stateRef = *state;
    stateRef.ui.confirmData.type = type;
    stateRef.ui.confirmData.text = getMessage();
    stateRef.ui.confirmData.leftHelpText = "";
    stateRef.ui.confirmData.rightHelpText = "";
    stateRef.ui.confirmData.confirmCardRoomInd = roomInd;
    stateRef.ui.confirmData.leftDisabled = false;
    stateRef.ui.confirmData.rightDisabled = false;
    stateRef.ui.inputMode = CONFIRM;
    stateRef.ui.cursor.visible = true;
    stateRef.ui.cursor.type =
        type == PICK_WEAPON_TO_ATTACK_WITH ? CARD : BUTTON;

    if (type == PICK_WEAPON_TO_ATTACK_WITH) {
      if (stateRef.currentWeapon.has_value()) {
        int attackableValue = calcWeaponAttackableValue(
            stateRef.currentWeapon.value(), stateRef.weaponDefeated);

        if (stateRef.room[roomInd].card.value >= attackableValue) {
          stateRef.ui.cursorInds.confirm = 1;
          stateRef.ui.confirmData.leftDisabled = true;
          stateRef.ui.confirmData.leftHelpText =
              sdl2w::L10n::trans(LOCSTR("You must attack a weaker monster with this weapon."));
        }
        transform::moveTo(
            stateRef.currentWeapon.value().pos,
            {CONFIRM_CARD_LEFT_POS.first - CARD_WIDTH * CARD_SCALE / 2,
             CONFIRM_CARD_LEFT_POS.second - CARD_HEIGHT * CARD_SCALE / 2},
            100);

        for (size_t i = 0; i < stateRef.weaponDefeated.size(); i++) {
          auto& weaponDefeatedCard = stateRef.weaponDefeated[i];
          transform::moveTo(
              weaponDefeatedCard.pos,
              {weaponDefeatedCard.pos.x,
               weaponDefeatedCard.pos.y + CARD_HEIGHT * CARD_SCALE / 3 + 12},
              100);
        }
      } else {
        stateRef.ui.cursorInds.confirm = 1;
        stateRef.ui.confirmData.leftHelpText =
            sdl2w::L10n::trans(LOCSTR("You have no weapon."));
        stateRef.ui.confirmData.leftDisabled = true;
      }
    } else if (type == CONFIRM_CHOOSE_WEAPON) {
      if (stateRef.currentWeapon.has_value()) {
        stateRef.ui.confirmData.leftHelpText = sdl2w::L10n::trans(
            LOCSTR("This will replace your current weapon."));
      }
    }

    Dispatch::get().addAction(
        std::make_unique<HoverConfirmButton>(stateRef.ui.cursorInds.confirm));
  }

public:
  int roomInd;
  ConfirmType type;

  SetInputModeConfirm(int roomInd, ConfirmType type)
      : roomInd(roomInd), type(type) {}
};

} // namespace actions
} // namespace scoundrel