#include <arabica/memory/memory.hpp>
#include <stdexcept>

namespace arabica {

Memory::Memory() {
  clear_cell();
}

Memory::~Memory() {
  clear_cell();
}

Memory::value_t Memory::read(const address_t address) const {
  if (is_valid(address)) {
    return _cell[address];
  }
  throw std::out_of_range("Invalid memory address accessed");
}

void Memory::write(const address_t address, value_t value) {
  if (is_valid(address)) {
    _cell[address] = value;
  } else {
    throw std::out_of_range("Invalid memory address accessed");
  }
}

Memory::value_t& Memory::operator[](const address_t address) {
  if (is_valid(address)) {
    return _cell[address];
  }
  throw std::out_of_range("Invalid memory address accessed");
}

const Memory::value_t& Memory::operator[](const address_t address) const {
  if (is_valid(address)) {
    return _cell[address];
  }
  throw std::out_of_range("Invalid memory address accessed");
}

void Memory::clear_cell() {
  _cell.fill(0);
}

bool Memory::is_valid(const address_t address) const {
  return address >= RESERVED && address < SIZE;
}

} // namespace arabica