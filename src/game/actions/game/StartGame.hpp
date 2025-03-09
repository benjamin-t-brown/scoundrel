#pragma once

#include "LibHTML.h"
#include "game/Dispatch.h"
#include "game/actions/AbstractAction.h"
#include "game/actions/depictions/DrawCardsForNewRoom.hpp"
#include "game/actions/ui/HoverCardInRoom.hpp"
#include "game/actions/ui/SetInputModeRoom.hpp"

namespace scoundrel {
namespace actions {

class StartGame : public AbstractAction {
protected:
  void act() override {
    *state = {};
    State& stateRef = *state;

    std::vector<Card> localDeck;
    for (int i = 2; i <= 14; i++) {
      if (i < 11) {
        localDeck.push_back({i, HEARTS});
        localDeck.push_back({i, DIAMONDS});
      }
      localDeck.push_back({i, SPADES});
      localDeck.push_back({i, CLUBS});
    }

    while (localDeck.size() > 0) {
      const int randI = rand() % static_cast<int>(localDeck.size());
      stateRef.stack.push_back({localDeck[randI]});
      localDeck.erase(localDeck.begin() + randI);
      auto& stackCard = stateRef.stack.back();
      stackCard.faceDown = true;
      transform::setAt(stackCard.pos, CARD_STACK_POS);
    }

    stateRef.ui.soundsToPlay.push_back("start_game");

    Dispatch::get().addAction(std::make_unique<DrawCardsForNewRoom>());
    Dispatch::get().addAction(std::make_unique<actions::SetInputModeRoom>());
    Dispatch::get().addAction(std::make_unique<actions::HoverCardInRoom>(0));

    libhtml::notifyGameStarted();
  }
};

} // namespace actions
} // namespace scoundrel