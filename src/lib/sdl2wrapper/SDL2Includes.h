#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct Mix_Chunk;

// Forward declarations matching SDL's typedef patterns
// Emscripten SDL uses _TTF_Font and _Mix_Music, while system SDL uses TTF_Font and Mix_Music
#ifdef __EMSCRIPTEN__
typedef struct _TTF_Font TTF_Font;
typedef struct _Mix_Music Mix_Music;
#else
typedef struct TTF_Font TTF_Font;
typedef struct Mix_Music Mix_Music;
#endif
typedef struct _SDL_Joystick SDL_Joystick;

namespace sdl2w {
struct SDL_Deleter {
  void operator()(SDL_Window* p) const;
  void operator()(SDL_Renderer* p) const;
  void operator()(SDL_Texture* p) const;
  void operator()(Mix_Chunk* p) const;
  void operator()(TTF_Font* p) const;
  void operator()(Mix_Music* p) const;
  void operator()(SDL_Joystick* p) const;
};
} // namespace sdl2w
