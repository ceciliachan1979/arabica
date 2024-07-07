#include <arabica/ui/window.hpp>
#include <arabica/driver/keypad.hpp>
#include <fmt/core.h>

namespace arabica {

Uint32 _ontick(Uint32 interval, void* userdata) {
  return (static_cast<Window*>(userdata))->ontick(interval, userdata);
}

Window::Window(const std::string& title, const int width, const int height, const std::string& rom) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    fmt::print("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  if (!emulator.load(rom)) {
    fmt::print("Failed to load rom");
    std::exit(1);
  }

  _width  = width;
  _height = height;

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

  emulator.display.init(_width, _height);

  _texture = SDL_CreateTexture(_renderer,                //
                               SDL_PIXELFORMAT_ARGB8888, //
                               SDL_TEXTUREACCESS_STATIC, //
                               _width,                   //
                               _height);                 //

  const int delay = 2;
  // const int delay = 1000;
  _timer_id = SDL_AddTimer(delay, _ontick, this);
}

Window::~Window() {
  SDL_DestroyTexture(_texture);
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::execute() {
  _running = true;
  while (_running) {
    while (SDL_PollEvent(&_event)) {
      switch (_event.type) {
        case SDL_QUIT: _running = false; break;
        case SDL_WINDOWEVENT: {
          if (_event.window.event == SDL_WINDOWEVENT_CLOSE) {
            if (SDL_FALSE == SDL_RemoveTimer(_timer_id)) {
              fmt::print("Failed to remove timer! SDL_Error: {}\n", SDL_GetError());
              std::exit(1);
            }
          }
        } break;
        case SDL_KEYDOWN: on_keyboard(_event.key.keysym.sym); break;
        default: break;
      }
    }
    on_render();
  }
}

void Window::on_keyboard(const SDL_Keycode keycode) {
  switch (keycode) {
    case SDLK_0: emulator.keypad.keydown_code = 0x0; break;
    case SDLK_1: emulator.keypad.keydown_code = 0x1; break;
    case SDLK_2: emulator.keypad.keydown_code = 0x2; break;
    case SDLK_3: emulator.keypad.keydown_code = 0x3; break;
    case SDLK_4: emulator.keypad.keydown_code = 0x4; break;
    case SDLK_5: emulator.keypad.keydown_code = 0x5; break;
    case SDLK_6: emulator.keypad.keydown_code = 0x6; break;
    case SDLK_7: emulator.keypad.keydown_code = 0x7; break;
    case SDLK_8: emulator.keypad.keydown_code = 0x8; break;
    case SDLK_9: emulator.keypad.keydown_code = 0x9; break;
    case SDLK_a: emulator.keypad.keydown_code = 0xA; break;
    case SDLK_b: emulator.keypad.keydown_code = 0xB; break;
    case SDLK_c: emulator.keypad.keydown_code = 0xC; break;
    case SDLK_d: emulator.keypad.keydown_code = 0xD; break;
    case SDLK_e: emulator.keypad.keydown_code = 0xE; break;
    case SDLK_f: emulator.keypad.keydown_code = 0xF; break;
    default: break;
  }
}

void Window::on_render() {
  SDL_RenderClear(_renderer);
  // SDL_SetRenderDrawColor(_renderer, 0x33, 0x99, 0x66, 0xFF);
  //
  // static auto color = 0x339966FF;
  // if (emulator.display.flag) {
  //   fmt::print("[emulator log] render n-byte sprite\n");
  //   color                 = (color + 120) % 256;
  //   emulator.display.flag = false;
  // }
  // for (int i = 0; i < _width * _height; ++i) {
  //   emulator.display.pixels[i] = color;
  // }
  if (emulator.display.flag) {
    SDL_UpdateTexture(_texture, NULL, emulator.display.pixels, _width * sizeof(uint32_t));
    emulator.display.flag = false;
  }
  SDL_RenderCopy(_renderer, _texture, NULL, NULL);
  SDL_RenderPresent(_renderer);
}

Uint32 Window::ontick(Uint32 interval, void* userdata) {
  emulator.run();
  // TODO: Check if there are update to display, if so, tell SDL to redraw (how - remember this is a separate thread)
  return interval;
}

} // namespace arabica
