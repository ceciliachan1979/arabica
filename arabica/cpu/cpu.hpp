#pragma once

#include <arabica/cpu/op_code.hpp>
#include <arabica/memory/memory.hpp>
#include <arabica/driver/keypad.hpp>
#include <cstdint>
#include <stack>

namespace arabica {

class CPU {
public:
  constexpr static uint16_t REGISTER_COUNT  = 16;
  constexpr static uint16_t PC_START        = 0x0200;
  constexpr static uint8_t  DEFAULT_RATE_HZ = 60;

  void run(const Memory& memory); // workaround, code refactoring later
  void run(const Memory& memory, const Keypad& keypad);

  void reset();

  uint8_t              registers[REGISTER_COUNT] = {0};
  uint16_t             reg_I{0x0000};
  uint8_t              reg_delay{DEFAULT_RATE_HZ};
  uint8_t              reg_sound{DEFAULT_RATE_HZ};
  uint16_t             pc{PC_START};
  std::stack<uint16_t> stack;
  uint16_t             instruction{0x0000};
  OP_CODE              opcode{OP_CODE::CLS};
};

} // namespace arabica
