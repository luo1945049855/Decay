#pragma once

#include <deque>

// Variadic Templates（可变参数模板）
// Variadic Templates: sizeof...(args)
// Variadic Templates arguments expansion.

// ###############################################################################

template <typename T>
void PrintTypes(const T& x) {
  std::wcout << std::left << std::setw(15) << x << std::setw(10) << std::right
             << L" size = " << std::setw(2) << sizeof(x) << std::endl;
}

template <typename T, typename... Types>
void PrintTypes(const T& x, const Types... args) {
  std::wcout << std::left << std::setw(15) << x << std::setw(10) << std::right
             << L" size = " << std::setw(2) << sizeof(x) << std::endl;
  PrintTypes(args...);
}

// ###############################################################################

template <typename T, typename R, typename... Args>
std::function<R(T& obj)> BindFunction(R (T::*pMemFn)(Args...), Args... args) {
  return [=](T& obj) { return (obj.*pMemFn)(args...); };
}

class MemObj {
 public:
  bool MemFunc(bool, int, float, double) {
    std::wcout << L"MemObj::MemFunc invoked" << std::endl;
    return true;
  }
};

// ###############################################################################

// sizeof...

template <typename... Args>
struct CountArgs {
  static constexpr int ArgsCount = sizeof...(Args);
};

// ###############################################################################

// variadic template arguments expansion.

template <size_t... Numbers>
auto GetNumberParameters() -> std::vector<size_t> {
  return std::vector<size_t>{Numbers...};
}

template <typename... Types>
auto GetTypesSize() -> std::deque<size_t> {
  return std::deque<size_t> { sizeof(Types)...; }
}

// ###############################################################################

struct TypeInfoDes {
  TypeInfoDes(const std::string& name, unsigned int hash_code, size_t size)
      : name(name), hash_code(hash_code), size(size) {}
  std::string name;
  unsigned long hash_code;
  size_t size;
};

template <typename... Types>
void PrintTypesInfo() {
  std::vector<TypeInfoDes> info_list = std::vector<TypeInfoDes>{TypeInfoDes{
      typeid(Types).name(), typeid(Types).hash_code(), sizeof(Types)}...};
  std::cout << std::setw(5) << "Name" << std::setw(5) << "Size"
             << std::setw(15) << "Hash" << "\n";
  std::stringstream ss;
  for (const auto& x : info_list) {
    ss.str("");
    ss.clear();
    ss << "0x" << std::hex << x.hash_code;
    std::cout << std::right << std::setw(5) << x.name << std::setw(5) << x.size
               << std::setw(15) << ss.str() << "\n";
  }
}

// ###############################################################################

TEST(VaridicTemplate, VaridicTemplate) {
  PrintTypes(L"hello world", true, 1, 1.0f, 2.0);

  MemObj mem_obj;
  std::function<bool(MemObj&)> mem_func_bind =
      BindFunction(&MemObj::MemFunc, true, 1, 1.0f, 1.0);
  mem_func_bind(mem_obj);

  static_assert(CountArgs<bool, int, float, double>::ArgsCount == 4,
                L"sizeof... operator");

  PrintTypesInfo<bool, int, float, double>();
}
