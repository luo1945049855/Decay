# std::enable_if

```cpp
template <bool B, class T = void>
struct enable_if;
```

if B is `true`, `std::enable_if` has a public member typedef type, equal to T; otherwise, there is no member typedef.
This metafunction is a convenient way to leverage `SFINAE`(substitution failure is not an error) to conditionally remove functions from overload resolution based on type trait and to provide separate funtion overloads and specializations for different type traits. `std::enable_if` can be used as an `additional function argument`(not applicable to operator overloads), as a `return type`(not applicable to constructors and destructors), or as a `class template or function template parameter`.

## Possible implementation

```cpp
template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> { typedef T type; }
```

## Notes

A common mistake is to deduce two function templates that differ only in their default template arguments. This does not work because the declarations are treated as redeclarations of the same function template (default template arguments are not accounted for in function template equivalence).

```cpp
// Wrong
struct T {
    enum { int_t, float_t } m_type;

    template <typename Integer,
              typename = std::enable_if_t<std::is_integral<Integer>::value>>
    T (Integer) : m_type(int_t) {}

    template <typename Floating,
              typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
    T (Floating) : m_type(float_t) {} // error: treated as redefinition.
};
// Right
struct T {
    enum { int_t, float_t } m_type;

    template <typename Integer,
              std::enable_if_t<std::is_integral<Integer>::value>>
    T (Integer) : m_type(int_t) {}

    template <typename Floating,
              std::enable_if_t<std::is_floating_point<Floating>::value>>
    T (Floating) : m_type(float_t) {} // OK.
};
```
