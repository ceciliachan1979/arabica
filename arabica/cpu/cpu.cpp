#include <arabica/cpu/cpu.hpp>
#include <fmt/core.h>

namespace arabica {

void CPU::run(const Memory& memory) {
  instruction = memory[pc] << 8 | memory[pc + 1];
  opcode      = static_cast<OP_CODE>(instruction & 0xF000);

  switch (opcode) {
    case OP_CODE::JP_addr: {
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    case OP_CODE::CALL_addr: {
      stack.push(pc + 2);
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    default: {
      fmt::print("Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(opcode));
    } break;
  }
}

} // namespace arabica
