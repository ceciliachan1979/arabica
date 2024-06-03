#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class rom : public noncopyable {
public:
  rom(rom&&)            = default;
  rom& operator=(rom&&) = default;
};

} // namespace arabica