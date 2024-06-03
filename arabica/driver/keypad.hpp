#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class keypad : public noncopyable {
public:
  keypad(keypad&&)            = default;
  keypad& operator=(keypad&&) = default;
};

} // namespace arabica