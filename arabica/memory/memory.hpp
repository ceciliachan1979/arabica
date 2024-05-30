#pragma once

#include <stdint.h>

#define MEMORY_SIZE 2048

typedef uint16_t address_t;
/**
 *
 */
void write_memory(address_t address, uint8_t value);
