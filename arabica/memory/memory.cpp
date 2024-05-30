#include <arabica/memory/memory.hpp>

uint8_t memory[MEMORY_SIZE];

void write_memory(address_t address, uint8_t value) {
  memory[address] = value;
}