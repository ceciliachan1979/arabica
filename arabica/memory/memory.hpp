#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <arabica/type/noncopyable.hpp>

namespace arabica {

class Memory : public noncopyable {
public:
  using address_t = uint16_t;
  using value_t   = uint8_t;

  constexpr static uint16_t SIZE     = 4096;
  constexpr static uint16_t RESERVED = (0x1FF - 0x000) + 1;

  Memory();
  ~Memory();

  value_t read(const address_t address) const;
  void write(const address_t address, value_t value);

  value_t& operator[](const address_t address);
  const value_t& operator[](const address_t address) const;

private:
  void clear_cell();
  bool is_valid(const address_t address) const;

  std::array<value_t, SIZE> _cell;
};

} // namespace arabica
