#include <arabica/thread/thread.hpp>

namespace arabica {

Thread::Thread(int (*func)(void*), const std::string& name, void* data)
  : _func(func)
  , _name(name)
  , _data(data)
  , _thread(nullptr) {
}

Thread::~Thread() {
  if (_thread != nullptr) {
    SDL_DetachThread(_thread);
  }
}

void Thread::start() {
  _thread = SDL_CreateThread(_func, _name.c_str(), _data);
  if (_thread == nullptr) {
    throw std::runtime_error("SDL_CreateThread failed: " + std::string(SDL_GetError()));
  }
}

void Thread::wait() {
  if (_thread != nullptr) {
    int threadReturnValue{-1};
    SDL_WaitThread(_thread, &threadReturnValue);
  }
}

} // namespace arabica