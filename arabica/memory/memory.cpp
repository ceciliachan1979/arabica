#include <iostream>
#include <fstream>
#include <arabica/memory/memory.hpp>
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
  is_valid(address);
  return _cell[address];
}

const Memory::value_t& Memory::read(const address_t address) const {
  is_valid(address);
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

} // namespace arabica