#include <arabica/memory/memory.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <fmt/core.h>

namespace arabica {

Memory::Memory() {
  clear_cell();
}

Memory::~Memory() {
  clear_cell();
}

Memory::value_t& Memory::read(const address_t address) {
  return _cell[address];
}

const Memory::value_t& Memory::read(const address_t address) const {
  return _cell[address];
}

void Memory::write(const address_t address, const value_t value) {
  is_valid(address);
  _cell[address] = value;
}

Memory::value_t& Memory::operator[](const address_t address) {
  return read(address);
}

const Memory::value_t& Memory::operator[](const address_t address) const {
  return read(address);
}

void Memory::clear_cell() {
  _cell.fill(0);
}

void Memory::is_valid(const address_t address) const {
  if (!(address >= RESERVED && address < SIZE)) {
    throw std::out_of_range("Invalid memory address accessed");
  }
}

bool Memory::load(const std::string& rom) {
  std::ifstream file(rom, std::ios::binary | std::ios::ate);
  if (!file) {
    fmt::print("Failed to open the file.");
    return false;
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  if (!file.read(reinterpret_cast<char*>(_cell.data() + RESERVED), size)) {
    file.close();
    return false;
  }
  file.close();
  return true;
}

void Memory::init_fonts() {
  // spec: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4
  constexpr std::array<value_t, 80> fonts = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  for (int i = 0; i < fonts.size(); ++i) {
    _cell[i] = fonts[i];
  }
}

} // namespace arabica