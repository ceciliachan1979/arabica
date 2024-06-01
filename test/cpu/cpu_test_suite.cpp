#include <test/cpu/cpu_test_suite.hpp>

void cpu_test_suite::test_run(void) {
  arabica::CPU cpu;
  arabica::Memory memory;

  memory.write(0x200, 0x61);
  memory.write(0x200, 0x00);

  cpu.run(memory);

  // TODO: Verify something

  ASSERT_TRUE(true);
}

TEST_F(cpu_test_suite, run_case_001) {
  cpu_test_suite::test_run();
}
