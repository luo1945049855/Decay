// Decay.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

#include "ExtractReturnAndArgs.h"
#include "Specialization.h"
#include "ArrayInTemplate.h"
#include "MetaFunctionAndTypeTraits.h"
#include "VariadicTemplate.h"
#include "EnableIf.h"
#include "CompileTimeComputation.h"

namespace NS_Function {

template <typename T>
using do_func = std::function<T>;

template <typename T>
void Invoke_do_func(do_func<T> func) {
  func();
}

void InvokeFuncTest() {
  std::wcout << L"InvokeFuncTest is invoked!" << std::endl;
}

void Test() {
  Invoke_do_func<void()>(InvokeFuncTest);
}

}  // namespace NS_Function

namespace NSExtractReturnAndArgs {

int Functor(int, int, double, double) {
  return 1;
}

}  // namespace NSExtractReturnAndArgs

int main(int argc, char* argv[]) {

  testing::InitGoogleTest(&argc, argv);

  NS_CArrayInArgs::Test();
  NS_MetaFunctionAndTypeTraits::Test();

  return RUN_ALL_TESTS();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
