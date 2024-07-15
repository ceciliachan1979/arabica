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

  void   on_keyboard(const SDL_Keycode keycode, const bool is_pressed);
  void   on_render();
  Uint32 on_tick(Uint32 interval, void* userdata);

  Emulator emulator;

private:
  bool _running{false};
  int  _width{100};
  int  _height{100};

  SDL_TimerID   _timer_id;
  SDL_Event     _event{0};
  SDL_Window*   _window{nullptr};
  SDL_Renderer* _renderer{nullptr};
  SDL_Texture*  _texture{nullptr};

  friend Uint32 _on_tick(Uint32 interval, void* userdata);
};

} // namespace arabica
