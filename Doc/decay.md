# std::decay

Applies `lvalue-to-rvalue`, `array to pointer`, and `function-to-pointer` implicit conversion to the type T, `remove cv-qualifiers`, and defines the resulting type as the member typedef type.

## Possible implementation

```cpp
template <class T>
struct decay
{
private:
    typedef typename std::remove_reference<T>::type U;
public:
    typedef typename
        std::conditional<
            std::is_array<U>::value, typename std::remove_extent<U>::type*,                             // array to pointer.
                std::conditional<
                    std::is_function<U>::value,
                    typename std::add_pointer<U>::type,                                                 // function to pointer.
                    typename std::remove_cv<U>::type                                                    // remove cv-qualifiers.
                >::type
        >::type type;
};
```

## Example

```cpp
#include <iostream>
#include <type_traits>

template <typename T, typename U>
struct decay_equiv :
    std::is_same<typename std::decay<T>::type, U>::type
{};

int main()
{
    std::cout << std::boolalpha
              << decay_equiv<int, int>::value << '\n'                   //
              << decay_equiv<int&, int>::value << '\n'                  // remove cv-qulifiers.
              << decay_equiv<int&&, int>::value << '\n'                 //  
              << decay_equiv<const int&, int>::value << '\n'            // remove cv-qualifiers.
              << decay_equiv<int[2], int*>::value << '\n'               // array to pointer.
              << decay_equiv<int(int), int(*)(int)>::value << '\n';     // function to pointer.
}
```

```cpp
template<typename... Ts>
void funka( std::tuple<Ts...>&& v ) {
}

template<typename T>
void funkb( T&& v ) {
    funka( std::forward<T>( v ) );
    // compile error: lvalue reference to rvalue reference. two solution:
    // when std::forward is invoked with a non-reference/rvalue-reference as its template parameter, it will move v.
    // 1. funka(std::forward<std::decay<T>::type>(v));
    // 2. funkb(std::move(tup)); // v is rvalue-reference.
}

void funk() {
    auto tup = std::tuple<int,int>( 1, 2 );
    funkb( tup );
}
```

## Reference

1. [cppreference.com](https://en.cppreference.com/w/cpp/types/remove_cv)
2. [cplusplus.com](http://www.cplusplus.com/reference/type_traits/decay/)
3. [Need decay in perfect forwarding](https://stackoverflow.com/questions/52038203/need-decay-in-perfect-forwarding)
