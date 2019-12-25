#pragma once

// Template Specialization.				// 特化
// Template Partial Specialization.		// 偏特化

// Experiment: Check wether number is floating point.

template <typename T>
bool IsFloatNumber(const T&) {
  return false;
}

template <>
bool IsFloatNumber<float>(const float&) {
  return true;
}

template <>
bool IsFloatNumber<double>(const double&) {
  return true;
}

// Experiment: Runtime type identification.

template <typename type>
const wchar_t* TypeName() {
  return L"unknown";
}

#define REGINSTER_TYPE(type)        \
  template <>                       \
  const wchar_t* TypeName<type>() { \
    return L## #type;               \
  }

REGINSTER_TYPE(int)
REGINSTER_TYPE(bool)

// Experiment: int specialization.

template <int>
const wchar_t* GetNumName() {
  return L"unknown";
}

template <>
const wchar_t* GetNumName<1>() {
  return L"one";
}

// Experiment: true/false type.

template <typename Type, Type v>
struct Integral_Type {
  static constexpr Type value = v;
};

template <bool v>
using bool_integral = Integral_Type<bool, v>;

using true_type = bool_integral<true>;
using false_type = bool_integral<false>;

// Experiment: Check wether type is equal.
// Partial specialization.

template <class A, class B>
struct type_equal {
  static constexpr bool value = false;
};

template <class A>
struct type_equal<A, A> {
  static constexpr bool value = true;
};

TEST(Specialization, TemplateSpecialization) {

  ASSERT_FALSE(IsFloatNumber(0));
  ASSERT_TRUE(IsFloatNumber(0.0f));
  ASSERT_TRUE(IsFloatNumber(0.0));

  ASSERT_STREQ(TypeName<int>(), L"int");
  ASSERT_STREQ(TypeName<bool>(), L"bool");
  ASSERT_STREQ(TypeName<long>(), L"unknown");

  ASSERT_STREQ(GetNumName<1>(), L"one");
  ASSERT_STREQ(GetNumName<2>(), L"unknown");

  ASSERT_TRUE(true_type::value);
  ASSERT_FALSE(false_type::value);

  ASSERT_FALSE((type_equal<float, int>::value));
  ASSERT_TRUE((type_equal<bool, bool>::value));
}
