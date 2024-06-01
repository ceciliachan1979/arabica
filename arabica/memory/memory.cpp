#include <arabica/memory/memory.hpp>

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
  } else {
    // ToDo: raise interrupt?
  }
  return 0;
}

void Memory::write(const address_t address, value_t value) {
  if (is_valid(address)) {
    _cell[address] = value;
  } else {
    // ToDo: raise interrupt?
  }
}

Memory::value_t& Memory::operator[](const address_t address) {
  if (is_valid(address)) {
    return _cell[address];
  } else {
    // ToDo: raise interrupt?
  }
  return _cell[0]; // ToDo: raise interrupt?
}

const Memory::value_t& Memory::operator[](const address_t address) const {
  if (is_valid(address)) {
    return _cell[address];
  } else {
    // ToDo: raise interrupt?
  }
  return _cell[0]; // ToDo: raise interrupt?
}

void Memory::clear_cell() {
  _cell.fill(0);
}

bool Memory::is_valid(const address_t address) const {
  return address >= RESERVED && address < SIZE;
}

} // namespace arabica