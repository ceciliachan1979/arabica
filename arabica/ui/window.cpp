#include <arabica/ui/window.hpp>
#include <fmt/core.h>

namespace arabica {

Uint32 _ontick(Uint32 interval, void* userdata) {
  return (static_cast<Window*>(userdata))->ontick(interval, userdata);
}

Window::Window(const std::string& title, const int width, const int height) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    fmt::print("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _window = SDL_CreateWindow(title.c_str(),           //
                             SDL_WINDOWPOS_UNDEFINED, //
                             SDL_WINDOWPOS_UNDEFINED, //
                             width,                   //
                             height,                  //
                             SDL_WINDOW_SHOWN);       //
  if (_window == nullptr) {
    fmt::print("Window could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (_renderer == nullptr) {
    fmt::print("Renderer could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  SDL_AddTimer(2, _ontick, this);
}

Window::~Window() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::execute() {
  _running = true;
  while (_running) {
    while (SDL_PollEvent(&_event)) {
      if (_event.type == SDL_QUIT) {
        _running = false;
      } else if (_event.type == SDL_KEYDOWN) {
        on_keyboard(_event.key.keysym.sym);
      }
    }
    on_render();
  }
}

void Window::on_keyboard(const SDL_Keycode keycode) {
  switch (keycode) {
    case SDLK_w: {
      fmt::print("{}\n", "w key pressed");
    } break;
    case SDLK_s: {
      fmt::print("{}\n", "s key pressed");
    } break;
    case SDLK_a: {
      fmt::print("{}\n", "a key pressed");
    } break;
    case SDLK_d: {
      fmt::print("{}\n", "d key pressed");
    } break;
    default: break;
  }
}

void Window::on_render() {
  SDL_RenderClear(_renderer);
  SDL_SetRenderDrawColor(_renderer, 0x33, 0x99, 0x66, 0xFF);
  SDL_RenderPresent(_renderer);
}

Uint32 Window::ontick(Uint32 interval, void* userdata) {
  //
  // TODO: Implementation
  //
  // Note, this happens on a separate thread
  //
  // 1. Check if there were any keystroke (which could happen on the UI thread at the same time, synchronization
  //    required), if so, tell the CPU that's the case
  // 2. Execute a batch of CPU instructions (Potentially just one)
  // 3. Check if there are update to display, if so, tell SDL to redraw (how - remember this is a separate thread)
  // 4. Handles any other CPU/Window interactions.
  //
  // This design should work even if this is executed on the UI thread, but this is not the case for SDL.
  //
  return interval;
}

} // namespace arabica
