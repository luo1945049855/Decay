#pragma once

// Meta function which return type.
// Meta function which return value.

template <typename T1, typename... T2>
struct MetaFunctionReturnType {
  using Type = T1;
};

template <typename T1, typename... T2>
struct MetaFunctionReturnValue {
  static constexpr bool value = true;
};

// Example

template <typename T>
struct IsPointer {
  static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*> {
  static constexpr bool value = true;
};

// Example: TypeInfo

template <typename T>
struct TypeInfo {
  static constexpr const wchar_t* name = L"unknown";
  static constexpr size_t size = sizeof(T);
  static constexpr bool is_number = false;
  static constexpr bool is_pointer = IsPointer<T>::value;
  static constexpr bool is_const = std::is_const<T>::value;
};

#define REGISTER_TYPE_INFO(type, is_number_arg) \
template <> \
struct TypeInfo<type> { \
  static constexpr const wchar_t* name = L ## #type; \
  static constexpr size_t size = sizeof(type); \
  static constexpr bool is_number = is_number_arg; \
  static constexpr bool is_pointer = IsPointer<type>::value; \
  static constexpr bool is_const = std::is_const<type>::value; \
};

REGISTER_TYPE_INFO(bool, false)
REGISTER_TYPE_INFO(int, true)

// Pre-defined type traits in <type_traits>
#include <type_traits>

namespace NS_MetaFunctionAndTypeTraits {

void Test() {
  std::wcout << L"########## Test For NS_MetaFunctionAndTypeTraits ##########" << std::endl;
  std::wcout << L"IsPointer<int>::value: " << IsPointer<int>::value
             << std::endl;
  std::wcout << L"IsPointer<int*>::value: " << IsPointer<int*>::value
             << std::endl;

  std::wcout << L"TypeInfo for " << TypeInfo<bool>::name << L" size = "
             << TypeInfo<bool>::size << L" is_number: "
             << TypeInfo<bool>::is_number << L" is_pointer: "
             << TypeInfo<bool>::is_pointer << L" is_const: "
             << TypeInfo<bool>::is_const << std::endl;
  std::wcout << L"TypeInfo for " << TypeInfo<const bool>::name << L" size = "
             << TypeInfo<const bool>::size << L" is_number: "
             << TypeInfo<const bool>::is_number << L" is_pointer: "
             << TypeInfo<const bool>::is_pointer << L" is_const: "
             << TypeInfo<const bool>::is_const << std::endl;

  static_assert(std::is_void<void>::value,
                L"std::is_void<void>::value is true");
  static_assert(std::is_floating_point<float>::value,
                L"float is floating point number");
  static_assert(std::is_integral<int>::value, L"int is integral number");
  static_assert(std::is_const<const int>::value, L"std::is_const");
  static_assert(std::is_reference<int&>::value, L"std::is_reference");
  static_assert(!std::is_same<int, int&>::value, L"std::is_same");
  static_assert(!std::is_base_of<int, int>::value, L"std::is_base::of");
}

}  // namespace NS_MetaFunctionAndTypeTraits
