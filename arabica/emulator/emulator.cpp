#include <arabica/emulator/emulator.hpp>
#include <cstdint>
#include <vector>

namespace arabica {

bool Emulator::init() {
  return sound.init();
}

bool Emulator::load(const std::string& rom) {
  return memory.load(rom);
}

void Emulator::execute() {
  is_enable_log = false;

  log_info("PC = {:x}\n", cpu.pc);

  // 500 Hz / 60 FPS = 500 (Instructions / Second) / 60 (Frames / Second) = 500 / 60 (Instructions / Frame)
  const int instructions_pre_frames = cpu.clock_speed / fps;
  for (int i = 0; i < instructions_pre_frames; ++i) {
    single_step();
  }

  log_info("The current cycle is {}\n", cycle);
  cycle++;

  delay.tick();
  sound.tick();
}

void Emulator::single_step() {
  cpu.instruction = memory[cpu.pc] << 8 | memory[cpu.pc + 1];
  uint16_t prefix = cpu.instruction & 0xF000;
  cpu.opcode      = static_cast<OP_CODE>(prefix);

  log_info("cpu.instruction is {0:x}\n", cpu.instruction);

  switch (prefix) {
    case 0x0: {
      switch (cpu.instruction & 0x00FF) {
        case 0xE0: cpu.opcode = OP_CODE::CLS; break;
        case 0xEE: cpu.opcode = OP_CODE::RET; break;
        default: break;
      }
    } break;
    case 0x8000: {
      switch (cpu.instruction & 0x000F) {
        case 0x0: cpu.opcode = OP_CODE::LD_Vx_Vy; break;
        case 0x1: cpu.opcode = OP_CODE::OR_Vx_Vy; break;
        case 0x2: cpu.opcode = OP_CODE::AND_Vx_Vy; break;
        case 0x3: cpu.opcode = OP_CODE::XOR_Vx_Vy; break;
        case 0x4: cpu.opcode = OP_CODE::ADD_Vx_Vy; break;
        case 0x5: cpu.opcode = OP_CODE::SUB_Vx_Vy; break;
        case 0x6: cpu.opcode = OP_CODE::SHR_Vx; break;
        case 0x7: cpu.opcode = OP_CODE::SUBN_Vx_Vy; break;
        case 0xE: cpu.opcode = OP_CODE::SHL_Vx; break;
        default: break;
      }
    } break;
    case 0xE000: {
      switch (cpu.instruction & 0x00FF) {
        case 0x9E: cpu.opcode = OP_CODE::SKP_Vx; break;
        case 0xA1: cpu.opcode = OP_CODE::SKNP_Vx; break;
        default: break;
      }
    }
    case 0xF000: {
      switch (cpu.instruction & 0x00FF) {
        case 0x07: cpu.opcode = OP_CODE::LD_Vx_DT; break;
        case 0x0A: cpu.opcode = OP_CODE::LD_Vx_K; break;
        case 0x15: cpu.opcode = OP_CODE::LD_DT_Vx; break;
        case 0x18: cpu.opcode = OP_CODE::LD_ST_Vx; break;
        case 0x1E: cpu.opcode = OP_CODE::ADD_I_Vx; break;
        case 0x29: cpu.opcode = OP_CODE::LD_F_Vx; break;
        case 0x33: cpu.opcode = OP_CODE::LD_B_Vx; break;
        case 0x55: cpu.opcode = OP_CODE::LD_I_Vx; break;
        case 0x65: cpu.opcode = OP_CODE::LD_Vx_I; break;
        default: break;
      }
    } break;
    default: break;
  }

  // The following comments are mostly taken from the Cowgod's Chip-8 Technical Reference v1.0.
  // link: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
  switch (cpu.opcode) {
    // 0nnn - SYS addr
    //
    // Jump to a machine code routine at nnn.
    //
    // This instruction is only used on the old computers on which Chip-8 was
    // originally implemented. It is ignored by modern interpreters.
    case OP_CODE::SYS_addr: {
      const uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc                = target;
    } break;
    // 00E0 - CLS
    //
    // Clear the display.
    case OP_CODE::CLS: {
      display.reset();
      display.is_refresh = true;
      cpu.advance_pc();
    } break;
    // 1nnn - JP addr
    //
    // Jump to location nnn.
    //
    // The interpreter sets the program counter to nnn.
    case OP_CODE::JP_addr: {
      const uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc                = target;
    } break;
    // 2nnn - CALL addr
    //
    // Call subroutine at nnn.
    //
    // The interpreter increments the stack pointer,
    // then puts the current PC on the top of the stack.
    // The PC is then set to nnn.
    case OP_CODE::CALL_addr: {
      cpu.stack.push(cpu.pc + 2);
      const uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc                = target;
    } break;
    // 00EE - RET
    //
    // Return from a subroutine.
    //
    // The interpreter sets the program counter to the address at the top of the stack,
    // then subtracts 1 from the stack pointer.
    case OP_CODE::RET: {
      if (!cpu.stack.empty()) {
        cpu.pc = cpu.stack.top();
        cpu.stack.pop();
      }
    } break;
    // 3xkk - SE Vx, byte
    //
    // Skip next instruction if Vx = kk.
    //
    // The interpreter compares register Vx to kk,
    // and if they are equal, increments the program counter by 2.
    case OP_CODE::SE_Vx_byte: {
      const uint8_t x       = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t kk_byte = cpu.instruction & 0x00FF;

      if (cpu.registers[x] == kk_byte) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // 4xkk - SNE Vx, byte
    //
    // Skip next instruction if Vx != kk.
    //
    // The interpreter compares register Vx to kk,
    // and if they are not equal, increments the program counter by 2.
    case OP_CODE::SNE_Vx_byte: {
      const uint8_t x       = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t kk_byte = cpu.instruction & 0x00FF;

      if (cpu.registers[x] != kk_byte) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // 5xy0 - SE Vx, Vy
    //
    // Skip next instruction if Vx = Vy.
    //
    // The interpreter compares register Vx to register Vy,
    // and if they are equal, increments the program counter by 2.
    case OP_CODE::SE_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      if (cpu.registers[x] == cpu.registers[y]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // 6xkk - LD Vx, byte
    //
    // Set Vx = kk.
    //
    // The interpreter puts the value kk into register Vx.
    case OP_CODE::LD_Vx_byte: {
      const uint8_t x       = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t kk_byte = cpu.instruction & 0x00FF;

      cpu.registers[x] = kk_byte;
      cpu.advance_pc();
    } break;
    // 7xkk - ADD Vx, byte
    //
    // Set Vx = Vx + kk.
    //
    // Adds the value kk to the value of register Vx,
    // then stores the result in Vx.
    case OP_CODE::ADD_Vx_byte: {
      const uint8_t x       = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t kk_byte = cpu.instruction & 0x00FF;

      cpu.registers[x] = cpu.registers[x] + kk_byte; // it will be wrapped if overflow occurs
      cpu.advance_pc();
    } break;
    // 8xy0 - LD Vx, Vy
    //
    // Set Vx = Vy.
    //
    // Stores the value of register Vy in register Vx.
    case OP_CODE::LD_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] = cpu.registers[y];
      cpu.advance_pc();
    } break;
    // 8xy1 - OR Vx, Vy
    //
    // Set Vx = Vx OR Vy.
    //
    // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
    // A bitwise OR compares the corrseponding bits from two values,
    // and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
    case OP_CODE::OR_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] |= cpu.registers[y];
      cpu.advance_pc();
    } break;
    // 8xy2 - AND Vx, Vy
    //
    // Set Vx = Vx AND Vy.
    //
    // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
    // A bitwise AND compares the corrseponding bits from two values, and if both bits are 1,
    // then the same bit in the result is also 1. Otherwise, it is 0.
    case OP_CODE::AND_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] &= cpu.registers[y];
      cpu.advance_pc();
    } break;
    // 8xy3 - XOR Vx, Vy
    //
    // Set Vx = Vx XOR Vy.
    //
    // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
    // An exclusive OR compares the corrseponding bits from two values,
    // and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
    case OP_CODE::XOR_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] ^= cpu.registers[y];
      cpu.advance_pc();
    } break;
    // 8xy4 - ADD Vx, Vy
    //
    // Set Vx = Vx + Vy, set VF = carry.
    //
    // The values of Vx and Vy are added together.
    // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
    // Only the lowest 8 bits of the result are kept, and stored in Vx.
    case OP_CODE::ADD_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      const uint16_t sum = cpu.registers[x] + cpu.registers[y];
      cpu.registers[0xF] = sum > 255;
      cpu.registers[x]   = sum & 0xFF;

      cpu.advance_pc();
    } break;
    // 8xy5 - SUB Vx, Vy
    //
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    //
    // If Vx > Vy, then VF is set to 1, otherwise 0.
    // Then Vy is subtracted from Vx, and the results stored in Vx.
    case OP_CODE::SUB_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[0xF] = cpu.registers[x] > cpu.registers[y];
      cpu.registers[x]   = cpu.registers[x] - cpu.registers[y];

      cpu.advance_pc();
    } break;
    // 8xy6 - SHR Vx {, Vy}
    //
    // Set Vx = Vx SHR 1.
    //
    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
    case OP_CODE::SHR_Vx: {
      // Remark: historically, the semantics is "right shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      cpu.registers[0xF] = cpu.registers[x] & 1;
      cpu.registers[x]   = cpu.registers[x] >> 1;

      cpu.advance_pc();
    } break;
    // 8xy7 - SUBN Vx, Vy
    //
    // Set Vx = Vy - Vx, set VF = NOT borrow.
    //
    // If Vy > Vx, then VF is set to 1, otherwise 0.
    // Then Vx is subtracted from Vy, and the results stored in Vx.
    case OP_CODE::SUBN_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[0xF] = cpu.registers[y] > cpu.registers[x];
      cpu.registers[x]   = cpu.registers[y] - cpu.registers[x];

      cpu.advance_pc();
    } break;
    // 8xyE - SHL Vx {, Vy}
    //
    // Set Vx = Vx SHL 1.
    //
    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
    case OP_CODE::SHL_Vx: {
      // Remark: historically, the semantics is "left shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      cpu.registers[0xF] = (cpu.registers[x] >> 7) & 1;
      cpu.registers[x]   = cpu.registers[x] << 1;

      cpu.advance_pc();
    } break;
    // 9xy0 - SNE Vx, Vy
    //
    // Skip next instruction if Vx != Vy.
    //
    // The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
    case OP_CODE::SNE_Vx_Vy: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      if (cpu.registers[x] != cpu.registers[y]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // Annn - LD I, addr
    //
    // Set I = nnn.
    //
    // The value of register I is set to nnn.
    case OP_CODE::LD_I_addr: {
      const uint16_t data = (cpu.instruction & 0x0FFF);
      cpu.reg_I           = data;
      cpu.advance_pc();
    } break;
    // Bnnn - JP V0, addr
    //
    // Jump to location nnn + V0.
    //
    // The program counter is set to nnn plus the value of V0.
    case OP_CODE::JP_V0_addr: {
      const uint16_t base_address = (cpu.instruction & 0x0FFF);
      cpu.pc                      = base_address;
      cpu.advance_pc(cpu.registers[0]);
    } break;
    // Cxkk - RND Vx, byte
    //
    // Set Vx = random byte AND kk.
    //
    // The interpreter generates a random number from 0 to 255,
    // which is then ANDed with the value kk. The results are stored in Vx.
    case OP_CODE::RND_Vx_byte: {
      const uint8_t x         = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t kk_byte   = (cpu.instruction & 0x00FF);
      const uint8_t rand_byte = random(0, 255);

      cpu.registers[x] = rand_byte & kk_byte;

      cpu.advance_pc();
    } break;
    // Fx07 - LD Vx, DT
    //
    // Set Vx = delay timer value.
    //
    // The value of DT is placed into Vx.
    case OP_CODE::LD_Vx_DT: {
      const uint8_t x  = (cpu.instruction & 0x0F00) >> 8;
      cpu.registers[x] = delay.get();
      cpu.advance_pc();
    } break;
    // Fx15 - LD DT, Vx
    //
    // Set delay timer = Vx.
    //
    // DT is set equal to the value of Vx.
    case OP_CODE::LD_DT_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      delay.set(cpu.registers[x]);
      cpu.advance_pc();
    } break;
    // Fx18 - LD ST, Vx
    //
    // Set sound timer = Vx.
    //
    // ST is set equal to the value of Vx.
    case OP_CODE::LD_ST_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_sound   = cpu.registers[x];
      sound.frequency = cpu.reg_sound;
      if (sound.frequency > 0) {
        sound.start_beep();
      }
      cpu.advance_pc();
    } break;
    // Fx1E - ADD I, Vx
    //
    // Set I = I + Vx.
    //
    // The values of I and Vx are added, and the results are stored in I.
    case OP_CODE::ADD_I_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_I       = cpu.reg_I + cpu.registers[x];
      cpu.advance_pc();
    } break;
    // Fx0A - LD Vx, K
    //
    // Wait for a key press, store the value of the key in Vx.
    //
    // All execution stops until a key is pressed, then the value of that key is stored in Vx.
    case OP_CODE::LD_Vx_K: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      const auto keycode = keypad.get_last_keypressed_code();
      if (keycode != -1) {
        cpu.registers[x] = keycode;
        cpu.advance_pc();
      }
    } break;
    // Ex9E - SKP Vx
    //
    // Skip next instruction if key with the value of Vx is pressed.
    //
    // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is
    // increased by 2.
    case OP_CODE::SKP_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      if (keypad.is_keypressed(cpu.registers[x])) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // ExA1 - SKNP Vx
    //
    // Skip next instruction if key with the value of Vx is not pressed.
    //
    // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is
    // increased by 2.
    case OP_CODE::SKNP_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      if (!keypad.is_keypressed(cpu.registers[x])) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    // Dxyn - DRW Vx, Vy, nibble
    //
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    //
    // The interpreter reads n bytes from memory, starting at the address stored in I.
    // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
    // Sprites are XORed onto the existing screen.
    // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
    // If the sprite is positioned so part of it is outside the coordinates of the display,
    // it wraps around to the opposite side of the screen.
    case OP_CODE::DRW_Vx_Vy_nibble: {
      const uint8_t x      = (cpu.instruction & 0x0F00) >> 8;
      const uint8_t y      = (cpu.instruction & 0x00F0) >> 4;
      const uint8_t nibble = cpu.instruction & 0x000F;

      std::vector<uint8_t> sprite_data;
      for (int i = 0; i < nibble; ++i) {
        sprite_data.push_back(memory.read(cpu.reg_I + i));
      }
      cpu.registers[0xF] = display.update(cpu.registers[x], cpu.registers[y], sprite_data);
      display.is_refresh = true;
      cpu.advance_pc();
    } break;
    // Fx29 - LD F, Vx
    //
    // Set I = location of sprite for digit Vx.
    //
    // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
    case OP_CODE::LD_F_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_I       = cpu.registers[x] * 0x5;
      cpu.advance_pc();
    } break;
    // Fx33 - LD B, Vx
    //
    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
    //
    // The interpreter takes the decimal value of Vx, and
    // places the hundreds digit in memory at location in I,
    // the tens digit at location I + 1,
    // and the ones digit at location I + 2.
    case OP_CODE::LD_B_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      const auto rx         = cpu.registers[x];
      memory[cpu.reg_I + 0] = (rx % 1000) / 100;
      memory[cpu.reg_I + 1] = (rx % 100) / 10;
      memory[cpu.reg_I + 2] = (rx % 10);
      cpu.advance_pc();
    } break;
    // Fx55 - LD [I], Vx
    //
    // Store registers V0 through Vx in memory starting at location I.
    //
    // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
    case OP_CODE::LD_I_Vx: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      for (int i = 0; i <= x; i++) {
        memory[cpu.reg_I + i] = cpu.registers[i];
      }
      cpu.advance_pc();
    } break;
    // Fx65 - LD Vx, [I]
    //
    // Read registers V0 through Vx from memory starting at location I.
    //
    // The interpreter reads values from memory starting at location I into registers V0 through Vx.
    case OP_CODE::LD_Vx_I: {
      const uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      for (int i = 0; i <= x; i++) {
        cpu.registers[i] = memory[cpu.reg_I + i];
      }
      cpu.advance_pc();
    } break;
    default: {
      log_info("Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(cpu.opcode));
    } break;
  }
}

} // namespace arabica
