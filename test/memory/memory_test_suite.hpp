#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <gtest/gtest.h>

void write_and_assert(const arabica::Memory::address_t address, const arabica::Memory::value_t value) {
  arabica::Memory memory;
  memory.write(address, value);
  ASSERT_EQ(memory.read(address), value);
}

#define arabica_memory_test(test_case, address, value) \
  TEST(memory_test_suite, test_case) {                 \
    arabica::CPU cpu;                                  \
    write_and_assert(address, value);                  \
  }
// clang-format off

arabica_memory_test(pc_test_case_001, cpu.pc, 0x61);
arabica_memory_test(pc_test_case_002, cpu.pc, 0x00);
arabica_memory_test(write_test_case_001, 0x300, 0x11);