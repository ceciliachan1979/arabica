#include <test/cpu/cpu_test_suite.hpp>

void cpu_test_suite::test_jump(void) {
  arabica::CPU cpu;
  arabica::Memory memory;

  memory.write(0x200, 0x16);
  memory.write(0x201, 0x00);

  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x600);
}

TEST_F(cpu_test_suite, run_case_001) {
  cpu_test_suite::test_jump();
}
