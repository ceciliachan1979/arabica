#pragma once

#include <cstdint>
#include <arabica/type/noncopyable.hpp>
#include <arabica/memory/memory.hpp>

namespace arabica {

class CPU : public noncopyable {
public:
  enum class OP_CODE : uint16_t {
    NONE      = 0x0000,
    JP_addr   = 0x1000,
    CALL_addr = 0x2000,
  };

  constexpr static uint16_t REGISTER_COUNT = 16;
  constexpr static uint16_t PC_START       = 0x200;

  CPU()  = default;
  ~CPU() = default;

  CPU(CPU&&)            = default;
  CPU& operator=(CPU&&) = default;

  void run(const Memory& memory);

  uint16_t pc{PC_START};
  uint8_t registers[REGISTER_COUNT] = {0};
  uint16_t instruction{0};
  OP_CODE opcode{OP_CODE::NONE};
};

} // namespace arabica