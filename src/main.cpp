#include "game/GameManager.h"
#include "game/state.h"
#include "lib/sdl2wrapper/Localization.h"
#include "lib/sdl2wrapper/Logger.h"
#include "lib/sdl2wrapper/Store.h"
#include "lib/sdl2wrapper/Window.h"

using sdl2w::Logger;
using sdl2w::LogType;

void renderLoadingScreen(sdl2w::Window& window) {
  const std::string text = sdl2w::L10n::trans(LOCSTR("Loadingg"));
  auto windowWidth = window.renderWidth;
  auto windowHeight = window.renderHeight;
  const float x = (windowWidth / 2.f);
  const float y = (windowHeight / 2.f);
  window.drawTextCentered(text, x, y, {255, 255, 255});
}

int main() {
#ifndef __EMSCRIPTEN__
  Logger::setLogToFile(true);
#endif

  Logger().get(LogType::INFO) << "Start program" << Logger::endl;
  srand(time(NULL));
  try {
    sdl2w::Window window(
        "Scoundrel", scoundrel::WINDOW_WIDTH, scoundrel::WINDOW_HEIGHT, 25, 50);
    window.setBackgroundColor({0, 0, 0});

    sdl2w::Store::createFont("default", "assets/monofonto.ttf");
    window.setCurrentFont("default", 18);

#ifdef __EMSCRIPTEN__
    renderLoadingScreen(window);
#else
    int ctr = 0;
    window.startRenderLoop([&]() {
      renderLoadingScreen(window);
      if (ctr == 2) {
        return false;
      }
      ctr++;
      return true;
    });
#endif

    scoundrel::GameManager game(window);
    game.load();

    // sdl2w::Store::logSprites();

    auto& events = window.getEvents();
    events.setKeyboardEvent(
        "keypress", [&](const std::string& key) { game.handleKeyPress(key); });

    Logger().get(LogType::INFO) << "Start loop" << Logger::endl;
    game.start();
    window.startRenderLoop([&]() {
      window.setBackgroundColor({10, 10, 10});
      window.setCurrentFont("default", 18);

      if (events.isKeyPressed("Escape")) {
        return false;
      }

      game.update(std::min(window.getDeltaTimeMs(), 100));
      game.render();
      return true;
    });

  } catch (const std::string& e) {
    Logger().get(LogType::ERROR) << e << Logger::endl;
  }

  Logger().get(LogType::INFO) << "End program" << Logger::endl;
}