#pragma once

namespace arabica {

class Delay {
public:
  Delay()
    : Delay{0} {
  }

  Delay(const int r)
    : rate{r} {
  }

  int rate = 0;
};

} // namespace arabica