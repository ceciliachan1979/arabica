#pragma once

#include <arabica/type/noncopyable.hpp>

namespace arabica {

class display : public noncopyable {
public:
  display(display&&)            = default;
  display& operator=(display&&) = default;
};

} // namespace arabica