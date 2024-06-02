#include <test/cpu/cpu_test_suite.hpp>

void cpu_test_suite::test_jump(void) {
  arabica::CPU cpu;
  arabica::Memory memory;

  memory.write(0x200, 0x16);
  memory.write(0x201, 0x00);

  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x600);
}

void cpu_test_suite::test_call(void) {
  arabica::CPU cpu;
  arabica::Memory memory;

  memory.write(0x200, 0x26);
  memory.write(0x201, 0x00);

  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x600);
  ASSERT_EQ(cpu.stack.size(), 1);
  ASSERT_EQ(cpu.stack.top(), 0x202);
}

TEST_F(cpu_test_suite, test_jump) {
  cpu_test_suite::test_jump();
}

TEST_F(cpu_test_suite, test_call) {
  cpu_test_suite::test_call();
}
