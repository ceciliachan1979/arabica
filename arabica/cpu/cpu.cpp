#include <arabica/cpu/cpu.hpp>
#include <fmt/core.h>

namespace arabica {

void CPU::run(const Memory& memory) {
  instruction     = memory[pc] << 8 | memory[pc + 1];
  uint16_t prefix = instruction & 0xF000;
  uint16_t suffix = instruction & 0x00FF;
  opcode          = static_cast<OP_CODE>(prefix);

  switch (prefix) {
    case 0x0: {
      switch (suffix) {
        case 0xEE: opcode = OP_CODE::RET; break;
        default:; break;
      }
    } break;
    default: break;
  }

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
    case OP_CODE::RET: {
      if (!stack.empty()) {
        pc = stack.top();
        stack.pop();
      } else {
        // ToDo: raise interrupt? how to test the failed case?
      };
    } break;
    default: {
      fmt::print("Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(opcode));
    } break;
  }
}

} // namespace arabica
