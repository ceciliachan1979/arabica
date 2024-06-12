#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <gtest/gtest.h>
#include <cstdint>
#include <algorithm>
#include <array>

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

arabica_cpu_test(test_se_vx_byte, 
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // SE V[0], 0x12
  memory.write(0x202, 0x30);
  memory.write(0x203, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
)


arabica_cpu_test(test_sne_vx_byte,
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // SNE V[0], 0x01
  memory.write(0x202, 0x40);
  memory.write(0x203, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
)

arabica_cpu_test(test_se_vx_vy,
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // LD V[0], 0x12
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // SE V[0], V[1]
  memory.write(0x204, 0x50);
  memory.write(0x205, 0x10);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
)

arabica_cpu_test(test_add_vx_byte,
  uint16_t address_offset = 0;
  for(uint8_t i = 0; i < 15; i++)
  {
    memory.write(0x200 + 2 * address_offset , 0x60 + i);
    memory.write(0x200 + 2 * address_offset + 1, i);
    cpu.run(memory);

    ASSERT_EQ(cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(cpu.registers[i], address_offset);

    address_offset++;
  }

  for(uint8_t i = 0, offset = 0; i < 15; i++)
  {
    memory.write(0x200 + 2 * address_offset, 0x70 + i);
    memory.write(0x200 + 2 * address_offset + 1, 1);
    cpu.run(memory);
    
    ASSERT_EQ(cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(cpu.registers[i], i + 1);
    address_offset++;
  }
)

arabica_cpu_test(test_ld_vx_vy,
  uint16_t address_offset = 0;
  
  // set register V[0..6] to i, for i = 0..6
  for(uint8_t i = 0, offset = 0; i < 7; i++)
  {
    memory.write(0x200 + 2 * address_offset,  0x60 + i);
    memory.write(0x200 + 2 * address_offset + 1, i);
    cpu.run(memory);
    
    ASSERT_EQ(cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(cpu.registers[i], i);

    address_offset++;
  }

  for(uint8_t i = 0, j = 7, offset = 0; j < 15; j++)
  {
    // load the value of register V[0..6] to V[7..14] 
    memory.write(0x200 + 2 * address_offset, 0x80 + j);
    memory.write(0x200 + 2 * address_offset + 1, i << 4);
    cpu.run(memory);
    ASSERT_EQ(cpu.registers[i], cpu.registers[j]);

    i++;
    address_offset++;
  }
)

arabica_cpu_test(test_or_vx_vy,
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // LD V[1], 0x34
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x34);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x34);

  // OR V[0], V[1]
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x11);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x36);
)

arabica_cpu_test(test_and_vx_vy,
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // LD V[1], 0x34
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x34);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x34);

  // AND V[0], V[1]
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x10);
)

arabica_cpu_test(test_xor_vx_vy,
  // LD V[0], 0x12
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x12);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x12);

  // LD V[1], 0x34
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x34);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x34);

  // XOR V[0], V[1]
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x13);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x26);
)

arabica_cpu_test(test_add_vx_vy,
  // LD V[0], 0x1 
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD V[1], 0x1
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x1);

  // ADD V[0], V[1], expected V[0] = 2
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x14);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x2);

  // LD V[1], 0xFF
  memory.write(0x206, 0x61);
  memory.write(0x207, 0xFF);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
  ASSERT_EQ(cpu.registers[1], 0xFF);

  // ADD V[0], V[1], expected V[0] = 1, V[0xF] = 1, overflow occur
  memory.write(0x208, 0x80);
  memory.write(0x209, 0x14);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20A);
  ASSERT_EQ(cpu.registers[0], 0x1);
  ASSERT_EQ(cpu.registers[0xF], 1);
)

arabica_cpu_test(test_sub_vx_vy,
  // LD V[0], 0x1 
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD V[1], 0x2
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x02);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x2);

  // SUB V[0], V[1], expected V[0] = 0xFF
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x15);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0xff);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // LD V[0], 0x2
  memory.write(0x206, 0x60);
  memory.write(0x207, 0x02);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
  ASSERT_EQ(cpu.registers[0], 0x2);

  // LD V[1], 0x1
  memory.write(0x208, 0x61);
  memory.write(0x209, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20A);
  ASSERT_EQ(cpu.registers[1], 0x1);

  // SUB V[0], V[1], expected V[0] = 0x00
  memory.write(0x20A, 0x80);
  memory.write(0x20B, 0x15);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20C);
  ASSERT_EQ(cpu.registers[0], 0x01);
  ASSERT_EQ(cpu.registers[0xF], 1);
)

arabica_cpu_test(test_shr_vx,
  // LD V[0], 0xF
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x0f);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0xF);

  // SHR V[0], expected V[0] = 0x7
  memory.write(0x202, 0x80);
  memory.write(0x203, 0x06);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[0], 0x7);
  ASSERT_EQ(cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x3
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x06);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x3);
  ASSERT_EQ(cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x1
  memory.write(0x206, 0x80);
  memory.write(0x207, 0x06);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
  ASSERT_EQ(cpu.registers[0], 0x1);
  ASSERT_EQ(cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x0
  memory.write(0x208, 0x80);
  memory.write(0x209, 0x06);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20A);
  ASSERT_EQ(cpu.registers[0], 0x0);
  ASSERT_EQ(cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x0
  memory.write(0x20A, 0x80);
  memory.write(0x20B, 0x06);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20C);
  ASSERT_EQ(cpu.registers[0], 0x0);
  ASSERT_EQ(cpu.registers[0xF], 0);
)

arabica_cpu_test(test_subn_vx_vy,
  // LD V[0], 0x2
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x02);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x2);

  // LD V[1], 0x1
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x1);

  // SUBN V[0], V[1], expected V[0] = 0xFF
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x17);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0xff);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // LD V[0], 0x1
  memory.write(0x206, 0x60);
  memory.write(0x207, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD V[1], 0x2
  memory.write(0x208, 0x61);
  memory.write(0x209, 0x02);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20A);
  ASSERT_EQ(cpu.registers[1], 0x2);

  // SUBN V[0], V[1], expected V[0] = 0x01
  memory.write(0x20A, 0x80);
  memory.write(0x20B, 0x17);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20C);
  ASSERT_EQ(cpu.registers[0], 0x1);
  ASSERT_EQ(cpu.registers[0xF], 1);
)

arabica_cpu_test(test_shl_vx,
  // LD V[0], 0xF
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x0f);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0xF);

  // SHL V[0], expected V[0] = 0x1E
  memory.write(0x202, 0x80);
  memory.write(0x203, 0x0E);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[0], 0x1E);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0x3C
  memory.write(0x204, 0x80);
  memory.write(0x205, 0x0E);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x206);
  ASSERT_EQ(cpu.registers[0], 0x3C);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0x78
  memory.write(0x206, 0x80);
  memory.write(0x207, 0x0E);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
  ASSERT_EQ(cpu.registers[0], 0x78);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0xF0
  memory.write(0x208, 0x80);
  memory.write(0x209, 0x0E);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20A);
  ASSERT_EQ(cpu.registers[0], 0xF0);
  ASSERT_EQ(cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0xE0
  memory.write(0x20A, 0x80);
  memory.write(0x20B, 0x0E);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x20C);
  ASSERT_EQ(cpu.registers[0], 0xE0);
  ASSERT_EQ(cpu.registers[0xF], 1);
)

arabica_cpu_test(test_sne_vx_vy,
  // LD V[0], 0x1
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

 // LD V[0], 0x2
  memory.write(0x202, 0x61);
  memory.write(0x203, 0x02);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[1], 0x2);


  // SNE V[0], V[1], expected the pc = 0x208
  memory.write(0x204, 0x90);
  memory.write(0x205, 0x10);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x208);
)

arabica_cpu_test(test_ld_i_addr,
  // LD I, 0x123
  memory.write(0x200, 0xA1);
  memory.write(0x201, 0x23);
  cpu.run(memory);
  ASSERT_EQ(cpu.reg_I, 0x0123);
)

arabica_cpu_test(test_jp_v0_addr,
  // LD V[0], 0x1
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // JP V[0], 0x300
  memory.write(0x202, 0xB3);
  memory.write(0x203, 0x00);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x301);
)

arabica_cpu_test(test_rnd_vx_byte,
  uint8_t key = 0xff;
  bool tested[256]{false};
  std::size_t N = 256 * 100;

  for(int i = 0; i < N; i++)
  {
    memory.write(0x200, 0xC0);
    memory.write(0x201, 0xff);
    cpu.run(memory);
    ASSERT_TRUE(cpu.registers[0] >= 0 && cpu.registers[0] <= 255);

    tested[cpu.registers[0]] = true;
    cpu.pc = 0x200;
  }

  ASSERT_TRUE(std::all_of(std::begin(tested), std::end(tested), [](bool f) { return f;}));
)

arabica_cpu_test(test_ld_vx_dt,
  // LD V[0], 0x1
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD V[0], DT
  memory.write(0x202, 0xF0);
  memory.write(0x203, 0x07);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.registers[0], arabica::CPU::DEFAULT_RATE_HZ);
)

arabica_cpu_test(test_ld_dt_vx,
  // LD V[0], 0x1
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD DT, V[0]
  memory.write(0x202, 0xF0);
  memory.write(0x203, 0x15);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.reg_delay, 0x1);
)

arabica_cpu_test(test_ld_st_vx,
  // LD V[0], 0x1
  memory.write(0x200, 0x60);
  memory.write(0x201, 0x01);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x202);
  ASSERT_EQ(cpu.registers[0], 0x1);

  // LD ST, V[0]
  memory.write(0x202, 0xF0);
  memory.write(0x203, 0x18);
  cpu.run(memory);
  ASSERT_EQ(cpu.pc, 0x204);
  ASSERT_EQ(cpu.reg_sound, 0x1);
)
