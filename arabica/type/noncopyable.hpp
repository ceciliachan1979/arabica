#pragma once

namespace arabica {

class noncopyable {
protected:
  noncopyable() = default;

  noncopyable(const noncopyable&)    = delete;
  void operator=(const noncopyable&) = delete;

  noncopyable(noncopyable&&)            = default;
  noncopyable& operator=(noncopyable&&) = default;
};

} // namespace arabica
