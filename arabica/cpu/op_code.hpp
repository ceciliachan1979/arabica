#pragma once

#include <cstdint>

namespace arabica {

// spec: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
enum class OP_CODE : uint16_t {
  NONE             = 0x0000,
  CLS              = 0x00E0, // 00E0
  RET              = 0x00EE, // 00EE
  SYS_addr         = 0x0000, // 0nnn
  JP_addr          = 0x1000, // 1nnn
  CALL_addr        = 0x2000, // 2nnn
  SE_Vx_byte       = 0x3000, // 3xkk
  SNE_Vx_byte      = 0x4000, // 4xkk
  SE_Vx_Vy         = 0x5000, // 5xy0
  LD_Vx_byte       = 0x6000, // 6xkk
  ADD_Vx_byte      = 0x7000, // 7xkk
  LD_Vx_Vy         = 0x8000, // 8xy0
  OR_Vx_Vy         = 0x8001, // 8xy1
  AND_Vx_Vy        = 0x8002, // 8xy2
  XOR_Vx_Vy        = 0x8003, // 8xy3
  ADD_Vx_Vy        = 0x8004, // 8xy4
  SUB_Vx_Vy        = 0x8005, // 8xy5
  SHR_Vx           = 0x8006, // 8xy6
  SUBN_Vx_Vy       = 0x8007, // 8xy7
  SHL_Vx           = 0x800E, // 8xyE
  SNE_Vx_Vy        = 0x9000, // 9xy0
  LD_I_addr        = 0xA000, // Annn
  JP_V0_addr       = 0xB000, // Bnnn
  RND_Vx_byte      = 0xC000, // Cxkk
  DRW_Vx_Vy_nibble = 0xD000, // Dxyn
  SKP_Vx           = 0xE09E, // Ex9E
  SKNP_Vx          = 0xE0A1, // ExA1
  LD_Vx_DT         = 0xF007, // Fx07
  LD_Vx_K          = 0xF00A, // Fx0A
  LD_DT_Vx         = 0xF015, // Fx15
  LD_ST_Vx         = 0xF018, // Fx18
  ADD_I_Vx         = 0xF01E, // Fx1E
  LD_F_Vx          = 0xF029, // Fx29
  LD_B_Vx          = 0xF033, // Fx33
  LD_I_Vx          = 0xF055, // Fx55
  LD_Vx_I          = 0xF065, // Fx65
};

} // namespace arabica