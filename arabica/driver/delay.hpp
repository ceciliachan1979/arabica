#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class delay : public noncopyable {
public:
  delay(delay&&)            = default;
  delay& operator=(delay&&) = default;
};

} // namespace arabica