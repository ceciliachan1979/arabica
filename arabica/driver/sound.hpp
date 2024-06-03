#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class sound : public noncopyable {
public:
  sound(sound&&)            = default;
  sound& operator=(sound&&) = default;
};

} // namespace arabica