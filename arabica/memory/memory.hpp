#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <arabica/type/noncopyable.hpp>

namespace arabica {

// spec: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#memmap
//
// +---------------+= 0xFFF (4095) End of Chip-8 RAM
// |               |
// |               |
// |               |
// |               |
// |               |
// | 0x200 to 0xFFF|
// |     Chip-8    |
// | Program / Data|
// |     Space     |
// |               |
// |               |
// |               |
// |               |
// |               |
// |               |
// |               |
// +---------------+= 0x200 (512) Start of most Chip-8 programs
// | 0x000 to 0x1FF|
// | Reserved for  |
// |  interpreter  |
// +---------------+= 0x000 (0) Start of Chip-8 RAM

class Memory : public noncopyable {
public:
  using address_t = uint16_t;
  using value_t   = uint8_t;

  constexpr static uint16_t SIZE     = 4096;
  constexpr static uint16_t RESERVED = (0x1FF - 0x000) + 1;

  Memory();
  ~Memory();

  Memory(Memory&&)            = default;
  Memory& operator=(Memory&&) = default;

  value_t& read(const address_t address);
  const value_t& read(const address_t address) const;

  void write(const address_t address, const value_t value);

  value_t& operator[](const address_t address);
  const value_t& operator[](const address_t address) const;

private:
  void clear_cell();
  void is_valid(const address_t address) const;

  std::array<value_t, SIZE> _cell;
};

} // namespace arabica
