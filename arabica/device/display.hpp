#pragma once

#include <vector>

namespace arabica {

class Display {
public:
  ~Display() {
    if (pixels != nullptr) {
      delete[] pixels;
    }
  }

  void init(const int w, const int h, const int s) {
    scale             = s;
    window_width      = w;
    window_height     = h;
    resolution        = w * h;
    chip8_width       = w / s;
    chip8_height      = h / s;
    pixels            = new uint32_t[resolution];
    horizontal_offset = 0;
    vertical_offset   = 0;
    reset();
  }

  void set(const int x, const int y, const uint32_t color) {
    if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
      pixels[y * window_width + x] = color;
    }
  }

  uint32_t get(const int x, const int y) const {
    if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
      return pixels[y * window_width + x];
    }
    return 0;
  }

  void reset() {
    for (int i = 0; i < resolution; ++i) {
      pixels[i] = 0;
    }
  }

  int update(const int reg_vx, const int reg_vy, const std::vector<uint8_t>& sprite_data) {
    bool collision = false;
    for (int y = 0; y < sprite_data.size(); ++y) {
      const uint8_t sprite_row = sprite_data[y];
      for (int x = 0; x < 8; ++x) {
        if (const uint8_t pixel = (sprite_row >> (7 - x)) & 0x01; pixel == 1) {
          const int device_x = (reg_vx + x) % chip8_width;
          const int device_y = (reg_vy + y) % chip8_height;
          const int window_x = device_x * scale + horizontal_offset;
          const int window_y = device_y * scale + vertical_offset;

          for (int dy = 0; dy < scale; ++dy) {
            for (int dx = 0; dx < scale; ++dx) {
              const auto window_dx = window_x + dx;
              const auto window_dy = window_y + dy;
              const auto pre_pixel = get(window_dx, window_dy);
              if (pre_pixel != 0) {
                collision = true;
              }
              set(window_dx, window_dy, pre_pixel ^ 0xFF0000FF);
            }
          }
        }
      }
    }
    return collision ? 1 : 0;
  }

  uint32_t* pixels{nullptr};
  int       chip8_width{0};
  int       chip8_height{0};
  int       scale{1};
  int       window_width{0};
  int       window_height{0};
  int       resolution{0};
  bool      is_refresh{false};
  int       horizontal_offset{0};
  int       vertical_offset{0};
};

} // namespace arabica