#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <arabica/driver/keypad.hpp>
#include <arabica/driver/display.hpp>
#include <arabica/driver/sound.hpp>
#include <arabica/driver/delay.hpp>
#include <fmt/core.h>

namespace arabica {

class Emulator {
public:
  Emulator()
    : cycle(0)
    , is_enable_log(false)
    , cpu(memory) {
  }

  bool init();
  bool load(const std::string& rom);
  void single_step();
  void execute();

  int  fps                    = 60;         // 60 FPS = 60 (Frames Per Second) = 60 (frames) / 1 (second)
  int  milliseconds_per_frame = 1000 / fps; // (FPS)^{-1} = 1 (s) / 60 (frames) = 1000 (milliseconds) / 60 (frames)
  int  cycle                  = 0;
  bool is_enable_log          = false;

  CPU     cpu;
  Memory  memory;
  Keypad  keypad;
  Display display;
  Sound   sound;
  Delay   delay;

private:
  template<typename... Args>
  inline void log_info(const char* const format, const Args&... args) {
    if (is_enable_log) {
      fmt::print("[emulator log] ");
      fmt::print(format, args...);
    }
  }
};

} // namespace arabica