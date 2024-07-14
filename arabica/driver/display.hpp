#pragma once

namespace arabica {

class Display {
public:
  ~Display() {
    if (pixels != nullptr) {
      delete[] pixels;
    }
  }

  void init(const int w, const int h, const int s) {
    width  = w;
    height = h;
    scale  = s;
    pixels = new uint32_t[(width * scale) * (height * scale)];
    reset_pixel();
  }

  void set_pixel(const int x, const int y, const uint32_t color) {
    if (x >= 0 && x < width * scale && y >= 0 && y < height * scale) {
      pixels[y * width * scale + x] = color;
    }
  }

  uint32_t get_pixel(const int x, const int y) const {
    if (x >= 0 && x < width * scale && y >= 0 && y < height * scale) {
      return pixels[y * width * scale + x];
    }
    return 0;
  }

  void reset_pixel() {
    for (int i = 0; i < width * scale * height * scale; ++i) {
      pixels[i] = 0;
    }
  }

  uint32_t* pixels{nullptr};
  int       width{0};
  int       height{0};
  int       scale{1};
  bool      flag{false};
};

} // namespace arabica