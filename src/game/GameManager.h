#pragma once

#include "client/Render.h"
#include "game/state.h"
#include "lib/sdl2wrapper/Window.h"

namespace program {

class GameManager {
  State state;
  sdl2w::Window& window;
  Render r;

public:
  GameManager(sdl2w::Window& windowA);
  ~GameManager();

  void load();
  void start();
  void handleKeyPress(const std::string& key);
  void update(int dt);
  void render();
};

} // namespace program
