#pragma once

#include <cstdint>
#include <arabica/memory/memory.hpp>

namespace arabica {

class CPU {
public:
  enum class OP_CODE : uint16_t {
    JP_addr   = 0x1000,
    CALL_addr = 0x2000,
  };

  constexpr static uint16_t REGISTER_COUNT = 16;
  constexpr static uint16_t PC_START       = 0x200;

  CPU();
  ~CPU();

  void run(const Memory& memory);

  uint16_t pc;
  uint8_t registers[REGISTER_COUNT];
  uint16_t opcode;
  OP_CODE instruction;

private:
  void reset();
};

} // namespace arabica