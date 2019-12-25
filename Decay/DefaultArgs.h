#pragma once

// Template with default arguments.

// Array allocated on stack with size 10.

template <typename ElementType, size_t Size = 10>
class Array {
 public:

 private:
  ElementType m_date[Size];
};
