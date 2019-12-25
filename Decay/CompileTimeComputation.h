#pragma once

// The `constexpr` specifier enables compile-time computations in a cleaner
// and more readable way than compile-time computation with recursive template
// metaprogramming.

// C++ Standards:
// * C++11=> Introduced constexpr, however it only allows constexpr values
//   and recursive constexpr functions without local variable and loops.
// * C++14=> Constexpr function supports local variable and loops.

// 1. compile-time `factorial computation` using recursive template
// metaprogramming(old C++).

using BigInt = unsigned int;

template <BigInt N>
struct FactorialA {
  enum { value = N * FactorialA<N - 1>::value };
};

template <>
struct FactorialA<0> {
  enum { value = 1 };
};

// 2. C++11 constexpr function for compile-time factorial computation(Version
// A):

constexpr BigInt FactorialB(BigInt n) {
  return n == 1 ? 1 : n * FactorialB(n - 1);
}

// 3. C++14 constexpr function for compile-time factorial computation(Version
// B):

constexpr BigInt FactorialC(BigInt n) {
  BigInt acc = 1;
  for (size_t i = 1; i <= n; ++i)
    acc *= i;
  return acc;
}

// 4. C++14 constexpr function for compile-time factorial computation(Version
// C):

// Note: factorial(6) will not is not computed at compile-time.
// Instead, it is be computed at runtime, and the compiler
// generates an ordinary function named factorialB

constexpr auto FactorialD(BigInt n) -> BigInt {
  BigInt acc = 1;
  for (size_t i = 1; i <= n; ++i)
    acc *= i;
  return acc;
}

TEST(CompileTimeComputation, CompileTimeComputation) {
  std::cout << " ======== Old C++ ============= " << "\n";

  std::cout << "Factorial(4) = " << FactorialA<4>::value << "\n";
  std::cout << "Factorial(5) = " << FactorialA<5>::value << "\n";

  static_assert(FactorialA<4>::value == 24,
                "Error: result supposed to be equal to 24");
  static_assert(FactorialA<5>::value == 120, "");
  static_assert(FactorialA<6>::value == 720, "");

  std::cout << " ======== New C++ >= C++11 ============= " << "\n";

  static_assert(FactorialB(4) == 24, "");
  static_assert(FactorialB(5) == 120, "");
  static_assert(FactorialC(4) == 24, "");
  static_assert(FactorialC(5) == 120, "");
  static_assert(FactorialD(4) == 24, "");
  static_assert(FactorialD(5) == 120, "");

  // Note: factorial(6) will not is not computed at compile-time.
  // Instead, it is be computed at runtime, and the compiler
  // generates an ordinary function named FactorialB
  std::cout << "factorialB(6) = " << FactorialB(6) << "\n";

  // Generates compilation error when false
  static_assert(FactorialA<4>::value == 24, "");
  static_assert(FactorialA<5>::value == 120, "");
  static_assert(FactorialB(4) == 24, "");
  static_assert(FactorialB(5) == 120, "");
  static_assert(FactorialC(6) == 720, "");
  static_assert(FactorialC(7) == 7 * 720, "");
  static_assert(FactorialD(8) == 8 * 7 * 720, "");
  static_assert(FactorialD(9) == 9 * 8 * 7 * 720, "");
}
