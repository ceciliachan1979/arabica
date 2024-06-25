#include <arabica/emulator/emulator.hpp>
#include <iostream>
#include <fmt/core.h>

namespace arabica {

bool Emulator::load(const std::string& rom) {
  return memory.load(rom);
}

// int cycle = 0;

void Emulator::run() {
  // fmt::print("The current cycle is {}\n", cycle);
  // cycle++;
  cpu.run(memory, keypad);
}

} // namespace arabica