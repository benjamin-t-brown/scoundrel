#include "game/GameManager.h"
#include "game/state.h"
#include "lib/sdl2wrapper/Localization.h"
#include "lib/sdl2wrapper/Store.h"
#include "lib/sdl2wrapper/Window.h"
#include "logger.h"

void renderLoadingScreen(SDL2Wrapper::Window& window) {
  const std::string text = SDL2Wrapper::Localization::trans(LOCSTR("Loading"));
  auto windowWidth = window.renderWidth;
  auto windowHeight = window.renderHeight;
  const float x = (windowWidth / 2.f);
  const float y = (windowHeight / 2.f);
  window.drawTextCentered(text, x, y, {255, 255, 255});
}

int main() {
  logger::info("Start program");
  srand(time(NULL));
  try {
    SDL2Wrapper::Window window(
        "Scoundrel", scoundrel::WINDOW_WIDTH, scoundrel::WINDOW_HEIGHT, 25, 50);
    window.setBackgroundColor({0, 0, 0});

    SDL2Wrapper::Store::createFont("default", "assets/monofonto.ttf");
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

    // SDL2Wrapper::Store::logSprites();

    auto& events = window.getEvents();
    events.setKeyboardEvent(
        "keypress", [&](const std::string& key) { game.handleKeyPress(key); });

    logger::info("Start loop");
    game.start();
    window.startRenderLoop([&]() {
      window.setBackgroundColor({10, 10, 10});
      window.setCurrentFont("default", 18);

      game.update(std::min(window.getDeltaTimeMs(), 100));
      game.render();
      return true;
    });

  } catch (const std::string& e) {
    Logger().get(LogType::ERROR) << e << Logger::endl;
  }

  logger::info("End program");
}