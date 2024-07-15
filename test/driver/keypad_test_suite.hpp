#pragma once

#include <arabica/emulator/emulator.hpp>
#include <gtest/gtest.h>

#define arabica_keypress_test_impl(test_case_name, test_case_body) \
  TEST(keypad_test_suite, test_case_name) {                        \
    arabica::Emulator emulator;                                    \
    test_case_body                                                 \
  }

#define arabica_keypress_test(i)                      \
  arabica_keypress_test_impl(test_key##i##_down, {    \
    emulator.keypad.last_keypressed_code = 0x##i;     \
    emulator.memory.write(emulator.cpu.pc + 0, 0xF0); \
    emulator.memory.write(emulator.cpu.pc + 1, 0x0A); \
    emulator.single_step();                           \
    ASSERT_EQ(emulator.cpu.registers[0], 0x##i);      \
  });

arabica_keypress_test(0);
arabica_keypress_test(1);
arabica_keypress_test(2);
arabica_keypress_test(3);
arabica_keypress_test(4);
arabica_keypress_test(5);
arabica_keypress_test(6);
arabica_keypress_test(7);
arabica_keypress_test(8);
arabica_keypress_test(9);
arabica_keypress_test(a);
arabica_keypress_test(b);
arabica_keypress_test(c);
arabica_keypress_test(d);
arabica_keypress_test(e);
arabica_keypress_test(f);