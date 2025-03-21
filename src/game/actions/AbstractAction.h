#pragma once

#include "lib/sdl2wrapper/Logger.h"
#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace scoundrel {

struct State;

namespace actions {
class AbstractAction {
protected:
  State* state = nullptr;

  virtual void act() {
    sdl2w::Logger().get(sdl2w::WARN) << "AbstractAction::act() called noop";
  };

public:
  virtual std::string getName() const {
#ifdef __GNUG__
    int status;
    char* realname = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);
    std::string name = (status == 0) ? realname : typeid(*this).name();
    free(realname);
    return name;
#else
    return typeid(*this).name();
#endif
  }

  void setState(State* state) { this->state = state; }

  void execute(State* state) {
    this->state = state;
    act();
  }
};
} // namespace actions

} // namespace scoundrel