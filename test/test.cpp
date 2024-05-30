#include <fmt/core.h>
#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>

int main(void) {
  init_cpu();
  write_memory(0x200, 0x61);
  write_memory(0x200, 0x00);
  run_cpu();
  // TODO: Verify something
  return 0;
}