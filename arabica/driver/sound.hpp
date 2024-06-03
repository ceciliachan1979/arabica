#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class sound : public noncopyable {
public:
  sound(const int r)
    : rate{r} {
  }

  ~sound() = default;

  sound(sound&&)            = default;
  sound& operator=(sound&&) = default;

  int rate = 0;
};

} // namespace arabica