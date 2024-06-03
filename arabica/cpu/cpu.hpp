#pragma once

#include <cstdint>
#include <stack>
#include <arabica/type/noncopyable.hpp>
#include <arabica/cpu/op_code.hpp>
#include <arabica/memory/memory.hpp>

namespace arabica {

class CPU : public noncopyable {
public:
  constexpr static uint16_t REGISTER_COUNT = 16;
  constexpr static uint16_t PC_START       = 0x200;

  CPU()  = default;
  ~CPU() = default;

  CPU(CPU&&)            = default;
  CPU& operator=(CPU&&) = default;

  void run(const Memory& memory);

  uint16_t             pc{PC_START};
  uint8_t              registers[REGISTER_COUNT] = {0};
  uint16_t             instruction{0};
  OP_CODE              opcode{OP_CODE::CLS};
  std::stack<uint16_t> stack;
};

} // namespace arabica