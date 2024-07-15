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
    width           = w;
    height          = h;
    scale           = s;
    scale_width     = scale * width;
    scale_height    = scale * height;
    pixels          = new uint32_t[scale_width * scale_height];
    vertical_offset = scale_height / 5;
    reset();
  }

  void set(const int x, const int y, const uint32_t color) {
    if (x >= 0 && x < scale_width && y >= 0 && y < scale_height) {
      pixels[y * scale_width + x] = color;
    }
  }

  uint32_t get(const int x, const int y) const {
    if (x >= 0 && x < scale_width && y >= 0 && y < scale_height) {
      return pixels[y * scale_width + x];
    }
    return 0;
  }

  void reset() {
    for (int i = 0; i < scale_width * scale_height; ++i) {
      pixels[i] = 0;
    }
  }

  int update(const int reg_vx, const int reg_vy, const std::vector<uint8_t>& sprite_data) {
    bool collision = false;
    for (int y = 0; y < sprite_data.size(); ++y) {
      const uint8_t sprite_row = sprite_data[y];
      for (int x = 0; x < 8; ++x) {
        if (const uint8_t pixel = (sprite_row >> (7 - x)) & 0x01; pixel == 1) {
          const int screen_x = (reg_vx + x) % width;
          const int screen_y = (reg_vy + y) % height;
          const int scaled_x = screen_x * scale;
          const int scaled_y = screen_y * scale + vertical_offset;

          for (int dy = 0; dy < scale; ++dy) {
            for (int dx = 0; dx < scale; ++dx) {
              const auto scaled_dx = scaled_x + dx;
              const auto scaled_dy = scaled_y + dy;
              const auto pre_pixel = get(scaled_dx, scaled_dy);
              if (pre_pixel != 0) {
                collision = true;
              }
              set(scaled_dx, scaled_dy, pre_pixel ^ 0xFF0000FF);
            }
          }
        }
      }
    }
    return collision ? 1 : 0;
  }

  uint32_t* pixels{nullptr};
  int       width{0};
  int       height{0};
  int       scale{1};
  int       scale_width{0};
  int       scale_height{0};
  bool      is_refresh{false};
  int       vertical_offset{0};
};

} // namespace arabica