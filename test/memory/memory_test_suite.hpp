#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <gtest/gtest.h>

class memory_test_suite : public ::testing::Test {
public:
  static void write_and_assert(const arabica::Memory::address_t addr, const arabica::Memory::value_t val);

  static arabica::CPU cpu;
  static arabica::Memory memory;
  static arabica::Memory::address_t address;
  static arabica::Memory::value_t value;
};
