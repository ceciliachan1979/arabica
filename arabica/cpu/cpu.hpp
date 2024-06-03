#pragma once

#include <cstdint>
#include <stack>
#include <arabica/type/noncopyable.hpp>
#include <arabica/cpu/op_code.hpp>
#include <arabica/memory/memory.hpp>

namespace arabica {

class CPU : public noncopyable {
public:
  constexpr static uint16_t REGISTER_COUNT  = 16;
  constexpr static uint16_t PC_START        = 0x0200;
  constexpr static uint16_t DEFAULT_RATE_HZ = 60;

  CPU()  = default;
  ~CPU() = default;

  CPU(CPU&&)            = default;
  CPU& operator=(CPU&&) = default;

  void run(const Memory& memory);

  uint8_t              registers[REGISTER_COUNT] = {0};
  uint16_t             reg_I{0x0000};
  uint8_t              reg_delay{DEFAULT_RATE_HZ};
  uint8_t              reg_sound{DEFAULT_RATE_HZ};
  uint16_t             pc{PC_START};
  uint8_t              sp{0x00};
  std::stack<uint16_t> stack;
  uint16_t             instruction{0x0000};
  OP_CODE              opcode{OP_CODE::CLS};
};

} // namespace arabica