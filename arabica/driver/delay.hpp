#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class delay : public noncopyable {
public:
  delay(const int r)
    : rate{r} {
  }

  ~delay() = default;

  delay(delay&&)            = default;
  delay& operator=(delay&&) = default;

  int rate = 0;
};

} // namespace arabica