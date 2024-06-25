#pragma once

#include <arabica/emulator/emulator.hpp>
#include <SDL2/SDL.h>
#include <string>

namespace arabica {

class Window {
public:
  Window(const std::string& title, const int width, const int height, const std::string& rom);
  ~Window();

  void execute();

  Emulator emulator;

private:
  void   on_keyboard(const SDL_Keycode keycode);
  void   on_render();
  Uint32 ontick(Uint32 interval, void* userdata);

  bool _running = false;

  SDL_TimerID   _timer_id;
  SDL_Event     _event{0};
  SDL_Window*   _window   = nullptr;
  SDL_Renderer* _renderer = nullptr;

  friend Uint32 _ontick(Uint32 interval, void* userdata);
};

} // namespace arabica
