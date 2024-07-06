#include <arabica/cpu/cpu.hpp>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <random>
#include <fmt/core.h>

namespace arabica {

inline static constexpr void advance_pc(uint16_t& pc, uint16_t offset = 2) {
  pc += offset;
}

template<typename T>
inline T random(T range_from, T range_to) {
  std::random_device               rand_dev;
  std::mt19937                     generator(rand_dev());
  std::uniform_int_distribution<T> distr(range_from, range_to);
  return distr(generator);
}

void CPU::reset() {
  pc = PC_START;
}

void CPU::run(Memory& memory) {
  Keypad keypad;
  run(memory, keypad);
}

void CPU::run(Memory& memory, Keypad& keypad) {
  Display display;
  run(memory, keypad, display);
}

void CPU::run(Memory& memory, Keypad& keypad, Display& display) {
  instruction     = memory[pc] << 8 | memory[pc + 1];
  uint16_t prefix = instruction & 0xF000;
  opcode          = static_cast<OP_CODE>(prefix);

  fmt::print("[cpu log] instruction is {0:x}\n", instruction);

  switch (prefix) {
    case 0x0: {
      switch (instruction & 0x00FF) {
        case 0xE0: opcode = OP_CODE::CLS; break;
        case 0xEE: opcode = OP_CODE::RET; break;
        default: break;
      }
    } break;
    case 0x8000: {
      switch (instruction & 0x000F) {
        case 0x0: opcode = OP_CODE::LD_Vx_Vy; break;
        case 0x1: opcode = OP_CODE::OR_Vx_Vy; break;
        case 0x2: opcode = OP_CODE::AND_Vx_Vy; break;
        case 0x3: opcode = OP_CODE::XOR_Vx_Vy; break;
        case 0x4: opcode = OP_CODE::ADD_Vx_Vy; break;
        case 0x5: opcode = OP_CODE::SUB_Vx_Vy; break;
        case 0x6: opcode = OP_CODE::SHR_Vx; break;
        case 0x7: opcode = OP_CODE::SUBN_Vx_Vy; break;
        case 0xE: opcode = OP_CODE::SHL_Vx; break;
        default: break;
      }
    } break;
    case 0xF000: {
      switch (instruction & 0x00FF) {
        case 0x07: opcode = OP_CODE::LD_Vx_DT; break;
        case 0x0A: opcode = OP_CODE::LD_Vx_K; break;
        case 0x15: opcode = OP_CODE::LD_DT_Vx; break;
        case 0x18: opcode = OP_CODE::LD_ST_Vx; break;
        case 0x1E: opcode = OP_CODE::ADD_I_Vx; break;
        case 0x29: opcode = OP_CODE::LD_F_Vx; break;
        case 0x33: opcode = OP_CODE::LD_B_Vx; break;
        case 0x55: opcode = OP_CODE::LD_I_Vx; break;
        case 0x65: opcode = OP_CODE::LD_Vx_I; break;
        default: break;
      }
    } break;
    default: break;
  }

  switch (opcode) {
    case OP_CODE::CLS: {
      // ToDo: actually clear the screen!
      advance_pc(pc);
    } break;
    case OP_CODE::JP_addr: {
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    case OP_CODE::CALL_addr: {
      stack.push(pc + 2);
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    case OP_CODE::RET: {
      if (!stack.empty()) {
        pc = stack.top();
        stack.pop();
      } else {
        // ToDo: raise interrupt? how to test the failed case?
      };
    } break;
    case OP_CODE::SE_Vx_byte: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      uint8_t byte_v = instruction & 0x00FF;
      if (registers[x] == byte_v)
        advance_pc(pc);

      advance_pc(pc);

    } break;
    case OP_CODE::SNE_Vx_byte: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      uint8_t byte_v = instruction & 0x00FF;

      if (registers[x] != byte_v)
        advance_pc(pc);

      advance_pc(pc);

    } break;
    case OP_CODE::SE_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;

      if (registers[x] == registers[y])
        advance_pc(pc);

      advance_pc(pc);
    } break;
    case OP_CODE::LD_Vx_byte: {
      uint8_t byte_v                         = instruction & 0x00FF;
      registers[(instruction & 0x0F00) >> 8] = byte_v;

      advance_pc(pc);
    } break;
    case OP_CODE::ADD_Vx_byte: {
      uint8_t byte_v = instruction & 0x00FF;
      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[x]   = registers[x] + byte_v; // it will be wrapped if overflow occurs

      advance_pc(pc);
    } break;
    case OP_CODE::LD_Vx_Vy: {
      uint8_t x    = (instruction & 0x0F00) >> 8;
      uint8_t y    = (instruction & 0x00F0) >> 4;
      registers[x] = registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::OR_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] |= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::AND_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] &= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::XOR_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] ^= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::ADD_Vx_Vy: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      uint8_t y      = (instruction & 0x00F0) >> 4;
      registers[x]   = registers[x] + registers[y];
      registers[0xF] = registers[y] > 255 - registers[x];

      advance_pc(pc);
    } break;
    case OP_CODE::SUB_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;

      registers[0xF] = registers[x] > registers[y];
      registers[x]   = registers[x] - registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::SHR_Vx: {
      // Remark: historically, the semantics is "right shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.

      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[0xF] = registers[x] & 1;
      registers[x]   = registers[x] >> 1;

      advance_pc(pc);
    } break;
    case OP_CODE::SUBN_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;

      registers[0xF] = registers[y] > registers[x];
      registers[x]   = registers[y] - registers[x];

      advance_pc(pc);
    } break;
    case OP_CODE::SHL_Vx: {
      // Remark: historically, the semantics is "left shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.
      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[0xF] = (registers[x] >> 7) & 1;
      registers[x]   = registers[x] << 1;

      advance_pc(pc);
    } break;
    case OP_CODE::SNE_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      if (registers[x] != registers[y])
        advance_pc(pc);

      advance_pc(pc);
    } break;
    case OP_CODE::LD_I_addr: {
      uint16_t data = (instruction & 0x0FFF);
      reg_I         = data;
      advance_pc(pc);
    } break;
    case OP_CODE::JP_V0_addr: {
      uint16_t base_address = (instruction & 0x0FFF);
      pc                    = base_address;
      advance_pc(pc, registers[0]);
    } break;
    case OP_CODE::RND_Vx_byte: {
      uint8_t x         = (instruction & 0x0F00) >> 8;
      uint8_t kk        = (instruction & 0x00FF);
      uint8_t rand_byte = random(0, 255);

      registers[x] = rand_byte & kk;

      advance_pc(pc);
    } break;
    case OP_CODE::LD_Vx_DT: {
      uint8_t x    = (instruction & 0x0F00) >> 8;
      registers[x] = reg_delay;
      advance_pc(pc);
    } break;
    case OP_CODE::LD_DT_Vx: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      reg_delay = registers[x];
      advance_pc(pc);
    } break;
    case OP_CODE::LD_ST_Vx: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      reg_sound = registers[x];
      advance_pc(pc);
    } break;
    case OP_CODE::ADD_I_Vx: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      reg_I     = reg_I + registers[x];
      advance_pc(pc);
    } break;
    case OP_CODE::LD_Vx_K: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      if (keypad.keydown_code != -1) {
        fmt::print("[cpu log] pressed key is {}\n", keypad.keydown_code);
        registers[x]        = keypad.keydown_code;
        keypad.keydown_code = -1;
        fmt::print("[cpu log] reg{} is {}\n", x, registers[x]);
        advance_pc(pc);
      }
    } break;
    case OP_CODE::DRW_Vx_Vy_nibble: {
      uint8_t vx     = (instruction & 0x0F00) >> 8;
      uint8_t vy     = (instruction & 0x00F0) >> 4;
      uint8_t nibble = instruction & 0x000F;

      std::vector<uint8_t> sprite_data;
      for (int i = 0; i < nibble; ++i) {
        sprite_data.push_back(memory.read(reg_I + i));
      }

      for (int y = 0; y < nibble; ++y) {
        uint8_t sprite_row = sprite_data[y];
        for (int x = 0; x < 8; ++x) {
          uint8_t pixel_value = (sprite_row >> (7 - x)) & 0x01;
          int     screen_x    = (registers[vx] + x) % display.width;
          int     screen_y    = (registers[vy] + y) % display.height;
          if (pixel_value == 1) {
            if (display.get_pixel(screen_x, screen_y)) {
              registers[0xF] = 1;
            }
            // display.set_pixel(screen_x, screen_y, !display.get_pixel(screen_x, screen_y));
            display.set_pixel(screen_x, screen_y, 255);
          }
        }
      }
      display.flag = true;
      fmt::print("[cpu log] display flag = {}.\n", display.flag);
      advance_pc(pc);
    } break;
    default: {
      fmt::print("Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(opcode));
    } break;
  }
}

} // namespace arabica
