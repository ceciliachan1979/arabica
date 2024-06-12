#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class Delay : public noncopyable {
public:
  Delay()
    : Delay{0} {
  }

  Delay(const int r)
    : rate{r} {
  }

  ~Delay() = default;

  Delay(Delay&&)            = default;
  Delay& operator=(Delay&&) = default;

  int rate = 0;
};

} // namespace arabica