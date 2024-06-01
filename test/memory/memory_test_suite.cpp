#include <test/memory/memory_test_suite.hpp>

arabica::CPU memory_test_suite::cpu;
arabica::Memory memory_test_suite::memory;
arabica::Memory::address_t memory_test_suite::address;
arabica::Memory::value_t memory_test_suite::value;

void memory_test_suite::write_and_assert(const arabica::Memory::address_t addr, const arabica::Memory::value_t val) {
  address = addr;
  value   = val;
  memory.write(address, value);
  ASSERT_EQ(memory.read(address), value);
}

TEST_F(memory_test_suite, write_case_001) {
  memory_test_suite::write_and_assert(memory_test_suite::cpu.pc, 0x61);
}

TEST_F(memory_test_suite, write_case_002) {
  memory_test_suite::write_and_assert(memory_test_suite::cpu.pc, 0x00);
}

TEST_F(memory_test_suite, write_case_003) {
  memory_test_suite::write_and_assert(0x300, 0x11);
}
