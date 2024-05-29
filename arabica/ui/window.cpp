#include <arabica/ui/window.hpp>
#include <fmt/core.h>

namespace arabica {

Window::Window(const std::string& title, const int width, const int height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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

} // namespace arabica
