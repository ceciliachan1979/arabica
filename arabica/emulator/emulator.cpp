#include <arabica/emulator/emulator.hpp>

namespace arabica {

void Emulator::run() {
  memory.write(cpu.pc, 0xF0);
  memory.write(cpu.pc + 1, 0x0A);
  cpu.run(memory, keypad);
  cpu.reset();
}

} // namespace arabica