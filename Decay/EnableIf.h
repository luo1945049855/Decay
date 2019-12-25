#pragma once

// std::enable_if
// std::enable_if_t = std::enable_if<bool, type>::type

// 1. in function return type

template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type ClassifyTypeInReturn(
    T value) {
  std::wcout << L"Type = integral;" << " 3 * value = " << 3 * value
             << std::endl;
}

// 2. in trailing return type

template <typename T>
auto ClassifyTypeInTrailingReturn(T value) ->
    typename std::enable_if<std::is_integral<T>::value, void>::type {
  std::wcout << L"Type = integral;" << " 3 * value = " << 3 * value
             << std::endl;
}

// 3. in parameter

template <typename T>
void ClassifyTypeInParameter(T value,
                  typename std::enable_if<std::is_integral<T>::value,
                                          void>::type* = nullptr) {
  std::wcout << L"Type = integral;" << " 3 * value = " << 3 * value
             << std::endl;
}

// 4. template type parameter

template <
    typename T,
    typename std::enable_if<std::is_integral<T>::value, void>::type* = nullptr>
    void ClassifyTypeInFunction(T value) {
  std::wcout << L"Type = integral;" << " 3 * value = " << 3 * value
             << std::endl;
}
