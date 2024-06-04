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

arabica_cpu_test(test_ret, 
  memory.write(0x200, 0x26);
  memory.write(0x201, 0x00);
  cpu.run(memory);
 
  memory.write(0x600, 0x00);
  memory.write(0x601, 0xEE);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.stack.size(), 0);
)

arabica_cpu_test(test_ld_vx_byte, 
  for(uint8_t i = 0, offset = 0; i < 15; i++)
  {
    memory.write(0x200 + offset, 0x60 + i);
    memory.write(0x200 + offset + 1, i);
    cpu.run(memory);

    ASSERT_EQ(cpu.registers[i], i);
  }
)

arabica_cpu_test(test_add_vx_byte,
  for(uint8_t i = 0, offset = 0; i < 15; i++)
  {
    memory.write(0x200 + offset, 0x60 + i);
    memory.write(0x200 + offset + 1, i);
    cpu.run(memory);
    
    ASSERT_EQ(cpu.registers[i], i);
  }

  for(uint8_t i = 0, offset = 0; i < 15; i++)
  {
    memory.write(0x200 + offset, 0x70 + i);
    memory.write(0x200 + offset + 1, 1);
    cpu.run(memory);
    
    ASSERT_EQ(cpu.registers[i], i + 1);
  }
)

arabica_cpu_test(test_ld_vx_vy,

  // set register V[0..6] to i, for i = 0..6
  for(uint8_t i = 0, offset = 0; i < 7; i++)
  {
    memory.write(0x200 + offset, 0x60 + i);
    memory.write(0x200 + offset + 1, i);
    cpu.run(memory);
    
    ASSERT_EQ(cpu.registers[i], i);
  }

  for(uint8_t i = 0, j = 7, offset = 0; j < 15; j++)
  {
    // load the value of register V[0..6] to V[7..14] 
    memory.write(0x200 + offset, 0x80 + j);
    memory.write(0x200 + offset + 1, i << 4);
    cpu.run(memory);
    ASSERT_EQ(cpu.registers[i], cpu.registers[j]);

    i++;
  }
)

arabica_cpu_test(test_add_vx_vy,
  // LD V[0], 0x1 
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD V[1], 0x1
  memory.write(0x200, 0x61);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.registers[1], 0x1);

  // ADD V[0], V[1], expected V[0] = 2
  memory.write(0x200, 0x80);
  memory.write(0x201, 0x14);
  cpu.run(memory);
  ASSERT_EQ(cpu.registers[0], 0x2);

  // LD V[1], 0xFF
  memory.write(0x200, 0x61);
  memory.write(0x201, 0xFF);
  cpu.run(memory);
  ASSERT_EQ(cpu.registers[1], 0xFF);

  // ADD V[0], V[1], expected V[0] = 1, V[0xF] = 1, overflow occur
  memory.write(0x200, 0x80);
  memory.write(0x201, 0x14);
  cpu.run(memory);
  ASSERT_EQ(cpu.registers[0], 0x1);
  ASSERT_EQ(cpu.registers[0xF], 1);
)
