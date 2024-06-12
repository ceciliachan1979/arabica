#pragma once

#define arabica_thread(func, data) arabica::Thread(func, #func, data)

#include <arabica/type/noncopyable.hpp>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <string>

namespace arabica {

class Thread : public noncopyable {
public:
  Thread(int (*func)(void*), const std::string& name, void* data);
  ~Thread();

  Thread(Thread&&)            = default;
  Thread& operator=(Thread&&) = default;

  void start();
  void wait();

private:
  int (*_func)(void*){nullptr};
  std::string _name{""};
  void*       _data{nullptr};
  SDL_Thread* _thread{nullptr};
};

} // namespace arabica
