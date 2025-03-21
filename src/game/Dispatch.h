#pragma once

#include "game/state.h"
#include "game/actions/AbstractAction.h"
#include <memory>

#define DISPATCH_ACTION(action, ...)                                           \
  program::Dispatch::get().addAction(std::make_unique<action>(__VA_ARGS__))

namespace program {

class Dispatch {
  State* state; // intended unsafe pointer for singleton

  static Dispatch singleton;

public:
  std::vector<std::unique_ptr<actions::AbstractAction>> actions;
  static Dispatch& get();
  static void init(State* state);

  void addAction(std::unique_ptr<actions::AbstractAction> action);

  void update();
};

} // namespace program