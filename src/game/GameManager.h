#pragma once

#include "client/Render.h"
#include "game/state.h"
#include "lib/sdl2wrapper/Window.h"

namespace scoundrel {

class GameManager {
  State state;
  SDL2Wrapper::Window& window;
  Render r;

public:
  GameManager(SDL2Wrapper::Window& windowA);
  ~GameManager();

  void load();
  void start();
  void handleKeyPress(const std::string& key);
  void update(int dt);
  void render();
};

} // namespace scoundrel
