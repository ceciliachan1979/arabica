#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <gtest/gtest.h>

#define arabica_cpu_test(test_case_name, test_case_body) \
  TEST(cpu_test_suite, test_case_name) {                 \
    arabica::CPU    cpu;                                 \
    arabica::Memory memory;                              \
    test_case_body                                       \
  }
// clang-format off

arabica_cpu_test(test_jump,  
  memory.write(0x200, 0x16); 
  memory.write(0x201, 0x00);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x600);
)

arabica_cpu_test(test_call,
  memory.write(0x200, 0x26);
  memory.write(0x201, 0x00);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x600);
  ASSERT_EQ(cpu.stack.size(), 1);
  ASSERT_EQ(cpu.stack.top(), 0x202);
)
