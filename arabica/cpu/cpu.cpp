#include <arabica/cpu/cpu.hpp>

namespace arabica {

CPU::CPU() {
  reset();
}

CPU::~CPU() {
  reset();
}

void CPU::reset() {
  pc = PC_START;
  for (int i = 0; i < REGISTER_COUNT; i++) {
    registers[i] = 0;
  }
}

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
      // ToDo
    } break;
  }
}

} // namespace arabica