#include <arabica/emulator/emulator.hpp>
#include <iostream>
#include <fmt/core.h>

namespace arabica {

bool Emulator::load(const std::string& rom) {
  return memory.load(rom);
}

int cycle = 0;

void Emulator::run() {
  // memory.write(cpu.pc + 0, 0xF0);
  // memory.write(cpu.pc + 1, 0x0A);
  // memory.write(cpu.pc + 2, 0xD0);
  // memory.write(cpu.pc + 3, 0x14);

  fmt::print("The current cycle is {}\n", cycle);
  cycle++;
  fmt::print("pc = {:x}\n", cpu.pc);
  cpu.run(memory, keypad, display);
  // cpu.run(memory, keypad, display);

  // cpu.reset();
}

} // namespace arabica
