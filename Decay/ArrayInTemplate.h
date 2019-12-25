#pragma once

// Templates with C-arrays.

namespace NS_CArrayInArgs {

// Pass array by reference: T (&arr) [N]

template <typename T, size_t N>
void Array_Info(const wchar_t* name, T (&arr)[N]) {
  std::wcout << L"Size of array \"" << name << L"\" is: " << N << std::endl;
}

template <size_t N>
void Array_Info(const wchar_t* name, wchar_t (&arr)[N]) {
  std::wcout << L"Size[1] of char-array \"" << name << L"\" is: " << N
             << std::endl;
  std::wcout << L"Size[2] of char-array \"" << name << L"\" is: " << std::size(arr)
             << std::endl;
}

void Test() {
  std::wcout << std::fixed << std::setprecision(3);

  double double_arry[] = {3.14, 6.28, 9.42, 12.56};
  Array_Info(L"double_array", double_arry);

  wchar_t wchar_array[] = {L'a', L'b', L'c'};
  Array_Info(L"wchar_array", wchar_array);  

  const wchar_t const_wchar_array[] = {L'a', L'b', L'c'};
  Array_Info(L"const_wchar_array", const_wchar_array);
}

}  // namespace NS_CArrayInArgs
