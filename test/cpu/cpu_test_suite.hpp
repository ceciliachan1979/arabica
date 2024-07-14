#pragma once

#include <arabica/emulator/emulator.hpp>
#include <gtest/gtest.h>
#include <cstdint>
#include <algorithm>
#include <array>

#define arabica_cpu_test(test_case_name, test_case_body) \
  TEST(cpu_test_suite, test_case_name) {                 \
    arabica::Emulator emulator;                          \
    test_case_body                                       \
  }

// clang-format off

arabica_cpu_test(test_jump,  
  emulator.memory.write(0x200, 0x16); 
  emulator.memory.write(0x201, 0x00);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x600);
)

arabica_cpu_test(test_call,
  emulator.memory.write(0x200, 0x26);
  emulator.memory.write(0x201, 0x00);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x600);
  ASSERT_EQ(emulator.cpu.stack.size(), 1);
  ASSERT_EQ(emulator.cpu.stack.top(), 0x202);
)

arabica_cpu_test(test_ret, 
  emulator.memory.write(0x200, 0x26);
  emulator.memory.write(0x201, 0x00);
  emulator.single_step();
 
  emulator.memory.write(0x600, 0x00);
  emulator.memory.write(0x601, 0xEE);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.stack.size(), 0);
)

arabica_cpu_test(test_se_vx_byte, 
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // SE V[0], 0x12
  emulator.memory.write(0x202, 0x30);
  emulator.memory.write(0x203, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
)

arabica_cpu_test(test_sne_vx_byte,
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // SNE V[0], 0x01
  emulator.memory.write(0x202, 0x40);
  emulator.memory.write(0x203, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
)

arabica_cpu_test(test_se_vx_vy,
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // LD V[0], 0x12
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // SE V[0], V[1]
  emulator.memory.write(0x204, 0x50);
  emulator.memory.write(0x205, 0x10);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
)

arabica_cpu_test(test_add_vx_byte,
  uint16_t address_offset = 0;
  for(uint8_t i = 0; i < 15; i++)
  {
    emulator.memory.write(0x200 + 2 * address_offset , 0x60 + i);
    emulator.memory.write(0x200 + 2 * address_offset + 1, i);
    emulator.single_step();

    ASSERT_EQ(emulator.cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(emulator.cpu.registers[i], address_offset);

    address_offset++;
  }

  for(uint8_t i = 0, offset = 0; i < 15; i++)
  {
    emulator.memory.write(0x200 + 2 * address_offset, 0x70 + i);
    emulator.memory.write(0x200 + 2 * address_offset + 1, 1);
    emulator.single_step();
    
    ASSERT_EQ(emulator.cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(emulator.cpu.registers[i], i + 1);
    address_offset++;
  }
)

arabica_cpu_test(test_ld_vx_vy,
  uint16_t address_offset = 0;
  
  // set register V[0..6] to i, for i = 0..6
  for(uint8_t i = 0, offset = 0; i < 7; i++)
  {
    emulator.memory.write(0x200 + 2 * address_offset,  0x60 + i);
    emulator.memory.write(0x200 + 2 * address_offset + 1, i);
    emulator.single_step();
    
    ASSERT_EQ(emulator.cpu.pc, 0x200 + 2 * address_offset + 2);
    ASSERT_EQ(emulator.cpu.registers[i], i);

    address_offset++;
  }

  for(uint8_t i = 0, j = 7, offset = 0; j < 15; j++)
  {
    // load the value of register V[0..6] to V[7..14] 
    emulator.memory.write(0x200 + 2 * address_offset, 0x80 + j);
    emulator.memory.write(0x200 + 2 * address_offset + 1, i << 4);
    emulator.single_step();
    ASSERT_EQ(emulator.cpu.registers[i], emulator.cpu.registers[j]);

    i++;
    address_offset++;
  }
)

arabica_cpu_test(test_or_vx_vy,
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // LD V[1], 0x34
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x34);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x34);

  // OR V[0], V[1]
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x11);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x36);
)

arabica_cpu_test(test_and_vx_vy,
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // LD V[1], 0x34
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x34);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x34);

  // AND V[0], V[1]
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x10);
)

arabica_cpu_test(test_xor_vx_vy,
  // LD V[0], 0x12
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x12);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x12);

  // LD V[1], 0x34
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x34);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x34);

  // XOR V[0], V[1]
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x13);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x26);
)

arabica_cpu_test(test_add_vx_vy,
  // LD V[0], 0x1 
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD V[1], 0x1
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x1);

  // ADD V[0], V[1], expected V[0] = 2
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x14);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x2);

  // LD V[1], 0xFF
  emulator.memory.write(0x206, 0x61);
  emulator.memory.write(0x207, 0xFF);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
  ASSERT_EQ(emulator.cpu.registers[1], 0xFF);

  // ADD V[0], V[1], expected V[0] = 1, V[0xF] = 1, overflow occur
  emulator.memory.write(0x208, 0x80);
  emulator.memory.write(0x209, 0x14);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20A);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);
)

arabica_cpu_test(test_sub_vx_vy,
  // LD V[0], 0x1 
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD V[1], 0x2
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x02);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x2);

  // SUB V[0], V[1], expected V[0] = 0xFF
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x15);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0xff);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // LD V[0], 0x2
  emulator.memory.write(0x206, 0x60);
  emulator.memory.write(0x207, 0x02);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
  ASSERT_EQ(emulator.cpu.registers[0], 0x2);

  // LD V[1], 0x1
  emulator.memory.write(0x208, 0x61);
  emulator.memory.write(0x209, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20A);
  ASSERT_EQ(emulator.cpu.registers[1], 0x1);

  // SUB V[0], V[1], expected V[0] = 0x00
  emulator.memory.write(0x20A, 0x80);
  emulator.memory.write(0x20B, 0x15);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20C);
  ASSERT_EQ(emulator.cpu.registers[0], 0x01);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);
)

arabica_cpu_test(test_shr_vx,
  // LD V[0], 0xF
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x0f);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0xF);

  // SHR V[0], expected V[0] = 0x7
  emulator.memory.write(0x202, 0x80);
  emulator.memory.write(0x203, 0x06);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[0], 0x7);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x3
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x06);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x3);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x1
  emulator.memory.write(0x206, 0x80);
  emulator.memory.write(0x207, 0x06);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x0
  emulator.memory.write(0x208, 0x80);
  emulator.memory.write(0x209, 0x06);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20A);
  ASSERT_EQ(emulator.cpu.registers[0], 0x0);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);

  // SHR V[0], expected V[0] = 0x0
  emulator.memory.write(0x20A, 0x80);
  emulator.memory.write(0x20B, 0x06);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20C);
  ASSERT_EQ(emulator.cpu.registers[0], 0x0);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);
)

arabica_cpu_test(test_subn_vx_vy,
  // LD V[0], 0x2
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x02);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x2);

  // LD V[1], 0x1
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x1);

  // SUBN V[0], V[1], expected V[0] = 0xFF
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x17);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0xff);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // LD V[0], 0x1
  emulator.memory.write(0x206, 0x60);
  emulator.memory.write(0x207, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD V[1], 0x2
  emulator.memory.write(0x208, 0x61);
  emulator.memory.write(0x209, 0x02);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20A);
  ASSERT_EQ(emulator.cpu.registers[1], 0x2);

  // SUBN V[0], V[1], expected V[0] = 0x01
  emulator.memory.write(0x20A, 0x80);
  emulator.memory.write(0x20B, 0x17);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20C);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);
)

arabica_cpu_test(test_shl_vx,
  // LD V[0], 0xF
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x0f);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0xF);

  // SHL V[0], expected V[0] = 0x1E
  emulator.memory.write(0x202, 0x80);
  emulator.memory.write(0x203, 0x0E);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1E);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0x3C
  emulator.memory.write(0x204, 0x80);
  emulator.memory.write(0x205, 0x0E);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x206);
  ASSERT_EQ(emulator.cpu.registers[0], 0x3C);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0x78
  emulator.memory.write(0x206, 0x80);
  emulator.memory.write(0x207, 0x0E);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
  ASSERT_EQ(emulator.cpu.registers[0], 0x78);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0xF0
  emulator.memory.write(0x208, 0x80);
  emulator.memory.write(0x209, 0x0E);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20A);
  ASSERT_EQ(emulator.cpu.registers[0], 0xF0);
  ASSERT_EQ(emulator.cpu.registers[0xF], 0);

  // SHL V[0], expected V[0] = 0xE0
  emulator.memory.write(0x20A, 0x80);
  emulator.memory.write(0x20B, 0x0E);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x20C);
  ASSERT_EQ(emulator.cpu.registers[0], 0xE0);
  ASSERT_EQ(emulator.cpu.registers[0xF], 1);
)

arabica_cpu_test(test_sne_vx_vy,
  // LD V[0], 0x1
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD V[0], 0x2
  emulator.memory.write(0x202, 0x61);
  emulator.memory.write(0x203, 0x02);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.registers[1], 0x2);

  // SNE V[0], V[1], expected the pc = 0x208
  emulator.memory.write(0x204, 0x90);
  emulator.memory.write(0x205, 0x10);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x208);
)

arabica_cpu_test(test_ld_i_addr,
  // LD I, 0x123
  emulator.memory.write(0x200, 0xA1);
  emulator.memory.write(0x201, 0x23);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.reg_I, 0x0123);
)

arabica_cpu_test(test_jp_v0_addr,
  // LD V[0], 0x1
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // JP V[0], 0x300
  emulator.memory.write(0x202, 0xB3);
  emulator.memory.write(0x203, 0x00);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x301);
)

arabica_cpu_test(test_rnd_vx_byte,
  uint8_t key = 0xff;
  bool tested[256]{false};
  std::size_t N = 256 * 100;

  for(int i = 0; i < N; i++)
  {
    emulator.memory.write(0x200, 0xC0);
    emulator.memory.write(0x201, 0xff);
    emulator.single_step();
    ASSERT_TRUE(emulator.cpu.registers[0] >= 0 && emulator.cpu.registers[0] <= 255);

    tested[emulator.cpu.registers[0]] = true;
    emulator.cpu.pc = 0x200;
  }

  ASSERT_TRUE(std::all_of(std::begin(tested), std::end(tested), [](bool f) { return f;}));
)

arabica_cpu_test(test_ld_vx_dt,
  // LD V[0], 0x1
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD V[0], DT
  emulator.memory.write(0x202, 0xF0);
  emulator.memory.write(0x203, 0x07);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  // ASSERT_EQ(emulator.cpu.registers[0], arabica::CPU::DEFAULT_RATE_HZ);
)

arabica_cpu_test(test_ld_dt_vx,
  // LD V[0], 0x1
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD DT, V[0]
  emulator.memory.write(0x202, 0xF0);
  emulator.memory.write(0x203, 0x15);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  // ASSERT_EQ(emulator.reg_delay, 0x1);
)

arabica_cpu_test(test_ld_st_vx,
  // LD V[0], 0x1
  emulator.memory.write(0x200, 0x60);
  emulator.memory.write(0x201, 0x01);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x202);
  ASSERT_EQ(emulator.cpu.registers[0], 0x1);

  // LD ST, V[0]
  emulator.memory.write(0x202, 0xF0);
  emulator.memory.write(0x203, 0x18);
  emulator.single_step();
  ASSERT_EQ(emulator.cpu.pc, 0x204);
  ASSERT_EQ(emulator.cpu.reg_sound, 0x1);
)
