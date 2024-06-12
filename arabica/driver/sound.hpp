#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class Sound : public noncopyable {
public:
  Sound()
    : Sound(0) {
  }

  Sound(const int r)
    : rate{r} {
  }

  ~Sound() = default;

  Sound(Sound&&)            = default;
  Sound& operator=(Sound&&) = default;

  int rate = 0;
};

} // namespace arabica