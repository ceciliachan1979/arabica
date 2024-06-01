#include <arabica/cpu/cpu.hpp>
#include <fmt/core.h>

namespace arabica {

void CPU::run(const Memory& memory) {
  opcode      = memory[pc] << 8 | memory[pc + 1];
  instruction = static_cast<OP_CODE>(opcode & 0xF000);

  switch (instruction) {
    case OP_CODE::JP_addr: {
      // ToDo
    } break;
    case OP_CODE::CALL_addr: {
      // ToDo
    } break;
    default: {
      fmt::print("Unknown opcode: 0x{:X}\n", opcode);
    } break;
  }
}

} // namespace arabica
