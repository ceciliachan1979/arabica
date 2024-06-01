#pragma once

namespace arabica {

class noncopyable {
public:
  noncopyable() = default;

  noncopyable(const noncopyable&)    = delete;
  void operator=(const noncopyable&) = delete;

  noncopyable(noncopyable&&)            = default;
  noncopyable& operator=(noncopyable&&) = default;
};

} // namespace arabica