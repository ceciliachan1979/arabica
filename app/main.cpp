#include <arabica/ui/window.hpp>
#include <fmt/core.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
    fmt::print("Usage: ./arabica.out rom-file");
  }
  arabica::Window window("Arabica Emulator", 640, 320, argv[1]);
  window.execute();
  return 0;
}
