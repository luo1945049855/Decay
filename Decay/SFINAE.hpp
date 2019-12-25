#include <gtest/gtest.h>

#include <iostream>

/*

The SFINAE technique is peformed by adding new overload functions
(function with same name and different type signatures) for preveting
compilation error during the template parameter type substitution.

When a substition failure happens,
the compiler looks for a the next function overload,
if the substition fails again and there are no more candidates,
the compiler will generate an error.

*/

// SFINAE: Substitution Failure Is Not An Error.
// The point of `SFINAE` is to `deactivate a piece of template code for certain
// type`.

// The compiler needs to know which of the two overloads of `f` we're
// talking about. So it's going to substitute `T` for the parameter we're
// passing, which is `A` here. Actually, the compiler probably does a way
// more sophisticated analysis, but in essence the substitution of the two
// overloads will look like this:

/*
  void f(A x, typename A::type y)
  {}

  void f(A x, typename A::other_type y)
  {}

  But `A` doesn't have a type called `othertype`. This substitution has
  failed. But this won't be an error, because `Substitution Failure Is Not
  An Error`.
*/

struct AStruct {
  using ASubtype = int;
};

class AStructB {
 public:
  using ASubtype = const char*;
};

template <typename T>
void AfunctionTemplate(typename T::ASubtype value) {
  std::cout << " Type contains ASubtype = TRUE " << std::endl;
  std::cout << " Value = " << value << std::endl;
}

void AfunctionTemplate(int value) {
  std::cout << " Overload 2 => [INT] Type contains ASubtype = FALSE "
            << std::endl;
  std::cout << " Value = " << value << std::endl;
}

// Note: This overload function is not a function template, just a free function
void AfunctionTemplate(const char* value) {
  std::cout << " Overload 3 => [CONST CHAR*] Type contains ASubtype = FALSE "
            << std::endl;
  std::cout << " Value = " << value << std::endl;
}

//============================ SFINAE typeintrospection ===================

// 1. check whether type has default constructor.

template <typename T>
struct HasDefaultConstructor {
 private:
  // If the expression decltype(X()) fails,
  // then, the type X has no default constructor.
  // and SFINAE happens and this function is
  // excluded from the overload resolution.

  template <typename X, typename = decltype(X())>
  static auto testConstructor(void*) -> char;

  // Fallback - template with dummy parameter.

  template <typename X>
  static auto testConstructor(...) -> long;

 public:
  // If the type T has default constructor,
  // the return type of testConstructor<T>(nullptr)
  // is char, otherwise it is long.

  // (nullptr) is used to solve ambiguous problem.

  static constexpr bool value =
      std::is_same<decltype(testConstructor<T>(nullptr)), char>::value;
};

struct Default {};

struct NoDefault {
  NoDefault(bool, int, float) {}
};

// 2. Check whether type has .end() member function

template <typename T>
struct HasEndMemberFunction {
 private:
  template <typename X, typename = decltype(std::declval<X>().end())>
  static auto check(void*) -> char;

  template <typename X>
  static auto check(...) -> long;

 public:
  static constexpr bool value =
      std::is_same<decltype(check<T>(nullptr)), char>::value;
};

// 3. Check whether type is printable(whether it can be printed with (<<)
// operator)

template <typename T>
struct IsPrintable {
 private:
  template <typename X, typename = decltype(std::cout << std::declval<X>())>
  static auto check(void*) -> char;

  template <typename X>
  static auto check(...) -> long;

 public:
  static constexpr bool value =
      std::is_same<decltype(check<T>(nullptr)), char>::value;
};

// 4. print type if it is printable.
// 4.1: uses std::enable_if as function-return type.
// 4.2: uses std::enable_if as trailing-return type.
// 4.3: uses std::enable_if as function parameter.
// 4.4: uses std::enable_if as function template type parameter.
// 4.5: uses C++17 `if constexpr` for eliminating SFINAE function overload boilerplate.

// 4.1

template <typename T>
typename std::enable_if<IsPrintable<T>::value, void>::type PrintIfPrintableA(
    const char* text,
    T const& value) {
  std::cout << "[PrintIfPrintableA] "
            << "Value of object of type < " << text << "> = " << value << "\n";
}

template <typename T>
typename std::enable_if<!IsPrintable<T>::value, void>::type PrintIfPrintableA(
    const char* text,
    T const& value) {
  std::cout << "[PrintIfPrintableA] "
            << "Value of object of type < " << text << "> = "
            << "[NOT PRINTABLE]"
            << "\n";
}

// 4.2

template <typename T>
auto PrintIfPrintableB(const char* text, T const& value) ->
    typename std::enable_if<IsPrintable<T>::value, void>::type {
  std::cout << "[PrintIfPrintableB] "
            << "Value of object of type < " << text << "> = " << value << "\n";
}

template <typename T>
auto PrintIfPrintableB(const char* text, T const& value) ->
    typename std::enable_if<!IsPrintable<T>::value, void>::type {
  std::cout << "[PrintIfPrintableB] "
            << "Value of object of type < " << text << "> = "
            << "[NOT PRINTABLE]"
            << "\n";
}

// 4.3

template <typename T>
void PrintIfPrintableC(
    const char* text,
    T const& value,
    typename std::enable_if<IsPrintable<T>::value, void>::type* = nullptr) {
   std::cout << "[PrintIfPrintableC] "
            << "Value of object of type < " << text << "> = " << value << "\n";   
}

template <typename T>
void PrintIfPrintableC(
    const char* text,
    T const& value,
    typename std::enable_if<!IsPrintable<T>::value, void>::type* = nullptr) {
  std::cout << "[PrintIfPrintableC] "
            << "Value of object of type < " << text << "> = "
            << "[NOT PRINTABLE]"
            << "\n";
}

// 4.4

template <typename T, typename std::enable_if<IsPrintable<T>::value, void>::type* = nullptr>
void PrintIfPrintableD(const char* text, T const& value) {
   std::cout << "[PrintIfPrintableD] "
            << "Value of object of type < " << text << "> = " << value << "\n";   
}

template <typename T, typename std::enable_if<!IsPrintable<T>::value, void>::type* = nullptr>
void PrintIfPrintableD(const char* text, T const& value) {
  std::cout << "[PrintIfPrintableD] "
            << "Value of object of type < " << text << "> = "
            << "[NOT PRINTABLE]"
            << "\n";
}

// 4.5

template <typename T>
void PrintIfPrintableE(const char* text, T const& value) {
  std::cout << "[PrintIfPrintableE] "
            << "Value of object of type <" << text << "> = ";
  if constexpr (IsPrintable<T>::value)
    std::cout << value << "\n";
  else
    std::cout << "[NOT PRINTABLE]"
              << "\n";
}

//=========================================================================

TEST(SFINAE, SFINAE) {
  AfunctionTemplate<AStruct>(200);
  AfunctionTemplate<AStructB>("PARAMETER");

  // without overload 2 and overload 3, there will be compile error:
  // error : no matching function for call to 'AfunctionTemplate'
  // note : candidate template ignored: couldn 't infer template argument ' T'
  AfunctionTemplate(600);
  AfunctionTemplate("Hello world");

  ASSERT_TRUE(HasDefaultConstructor<Default>::value);
  ASSERT_FALSE(HasDefaultConstructor<NoDefault>::value);

  ASSERT_TRUE(HasEndMemberFunction<std::vector<int>>::value);
  ASSERT_FALSE(HasEndMemberFunction<int>::value);

  ASSERT_TRUE(IsPrintable<int>::value);
  ASSERT_FALSE(IsPrintable<Default>::value);

  int printable_obj = 1;
  Default not_printable_obj;

  PrintIfPrintableA("int", printable_obj);
  PrintIfPrintableA("Default", not_printable_obj);
  PrintIfPrintableB("int", printable_obj);
  PrintIfPrintableB("Default", not_printable_obj);
  PrintIfPrintableC("int", printable_obj);
  PrintIfPrintableC("Default", not_printable_obj);
  PrintIfPrintableD("int", printable_obj);
  PrintIfPrintableD("Default", not_printable_obj);
  PrintIfPrintableE("int", printable_obj);
  PrintIfPrintableE("Default", not_printable_obj);
}
