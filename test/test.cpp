#include <test/cpu/cpu_test_suite.hpp>
#include <test/memory/memory_test_suite.hpp>
#include <test/driver/keypad_test_suite.hpp>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
