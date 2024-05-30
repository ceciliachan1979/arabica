#include <test/cpu/cpu_test_suite.hpp>

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>

#include <gtest/gtest.h>

void test_cpu(void) {
  init_cpu();
  write_memory(0x200, 0x61);
  write_memory(0x200, 0x00);
  run_cpu();
  // TODO: Verify something
}

TEST(test_cpu, AssertionTrue) {
  test_cpu();
  ASSERT_TRUE(true);
}
