#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <arabica/type/noncopyable.hpp>

namespace arabica {

class Window : public noncopyable {
public:
  Window(const std::string& title, const int width, const int height);
  ~Window();

  Window(Window&&)            = default;
  Window& operator=(Window&&) = default;

  void execute();

private:
  void   on_keyboard(const SDL_Keycode keycode);
  void   on_render();
  Uint32 ontick(Uint32 interval, void* userdata);

  bool _running = false;

  SDL_Event     _event;
  SDL_Window*   _window   = nullptr;
  SDL_Renderer* _renderer = nullptr;

  friend Uint32 _ontick(Uint32 interval, void* userdata);
};

} // namespace arabica
