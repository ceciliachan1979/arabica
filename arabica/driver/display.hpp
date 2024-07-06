#pragma once

namespace arabica {

class Display {
public:
  ~Display() {
    if (pixels != nullptr) {
      delete[] pixels;
    }
  }

  void init(int w, int h) {
    width  = w;
    height = h;
    pixels = new uint32_t[width * height];
    clear();
  }

  void set_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
      pixels[y * width + x] = color;
    }
  }

  uint32_t get_pixel(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
      return pixels[y * width + x];
    }
    return 0;
  }

  void clear() {
    for (int i = 0; i < width * height; ++i) {
      pixels[i] = 0;
    }
  }

  uint32_t* pixels{nullptr};
  int       width{0};
  int       height{0};
  bool      flag{false};
};

} // namespace arabica