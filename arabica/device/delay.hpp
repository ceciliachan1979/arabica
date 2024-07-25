#pragma once

namespace arabica {

class Delay {
public:
  Delay(const int r = 0)
    : rate{r} {
  }

  void set(const uint8_t value) {
    rate = value;
  }

  uint8_t get() const {
    return rate;
  }

  void tick() {
    if (rate > 0) {
      --rate;
    }
  }

  int rate = 0;
};

} // namespace arabica