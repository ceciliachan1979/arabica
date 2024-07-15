#pragma once

#include <SDL2/SDL.h>

namespace arabica {

class Keypad {
public:
  void on_keydown(const uint8_t keycode) {
    keypressed_status[keycode] = true;
    last_keypressed_code       = keycode;
  }

  void on_keyup(const uint8_t keycode) {
    keypressed_status[keycode] = false;
  }

  bool is_keypressed(const uint8_t keycode) const {
    return keypressed_status[keycode];
  }

  int get_last_keypressed_code() const {
    return last_keypressed_code;
  }

  int last_keypressed_code = -1;

private:
  std::array<bool, 16> keypressed_status{};
};

} // namespace arabica