#include "Dispatch.h"
#include "game/Dispatch.h"
#include "game/state.h"

using sdl2w::Logger;
using sdl2w::LogType;

namespace program {

Dispatch Dispatch::singleton = Dispatch();

Dispatch& Dispatch::get() { return Dispatch::singleton; }

void Dispatch::init(State* state) {
  if (state == nullptr) {
    Logger().get(LogType::ERROR)
        << "Dispatch::init window is nullptr" << Logger::endl;
    return;
  }
  Dispatch::singleton.state = state;
}

void Dispatch::addAction(std::unique_ptr<actions::AbstractAction> action) {
  // logger::debug(("[ACTION] Adding action: " + action->getName()).c_str());
  actions.push_back(std::move(action));
}

void Dispatch::update() {
  if (!actions.empty()) {
    // prevents infinite loop if an action adds another action
    std::vector<std::unique_ptr<actions::AbstractAction>> actionsCopy;
    actionsCopy.swap(actions);
    for (auto& actionPtr : actionsCopy) {
      auto& action = *actionPtr;
      // logger::debug(("[ACTION] Invoking action: " +
      // action.getName()).c_str());
      action.execute(state);
    }
  }
}

} // namespace program