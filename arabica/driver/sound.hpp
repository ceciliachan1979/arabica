#pragma once

namespace arabica {

class Sound {
public:
  Sound()
    : Sound(0) {
  }

  Sound(const int r)
    : rate{r} {
  }

  int rate = 0;
};

} // namespace arabica