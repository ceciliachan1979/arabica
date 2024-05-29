#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace arabica {

class Window {

public:
  Window(const std::string& title, const int width, const int height);
  ~Window();

  void execute();

private:
  void on_keyboard(const SDL_Keycode keycode);
  void on_render();

  bool _running = false;

  SDL_Event _event;
  SDL_Window* _window     = nullptr;
  SDL_Renderer* _renderer = nullptr;
};

} // namespace arabica
