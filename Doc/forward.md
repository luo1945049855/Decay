# std::forward

what `std::forward` does according to standard:

```cpp
return static_cast<T&&>(t);
```

-----------------------------------------------------

Basically, given the expression E(a,b,c,...,n), we want the expression f(a,b,c,...,n) to equivalent. In C++03, this is impossible. There are many attempts, but they all fail in some regard.

-----------------------------------------------------

The simplest is to use lvalue reference:

```cpp
template <typename A, typename B, typename C>
void f(A& a, B& b, C& c)
{
    E(a, b, c);
}
```

But this fails to handle temporary values: `f(1, 2, 3);`, as those cannot be bound to an lvalue-reference.

The next attempt might be:

```cpp
template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c)
{
    E(a, b, c);
}
```

Which fixed the above problem, but flips flops. It now fails to allow E to have non-const arguments.

```cpp
int i = 1, j = 2, k = 3;
void E(int&, int&, int&);
f(i, j, k); // oops! E cannot modify these.
```

The third attempt accepts const-references, but then `const_cast` the const away:

```cpp
template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c)
{
    E(cosnt_cast<A&>(a), const_cast<B&>(b), const_cast<C&>(c));
}
```

This accepts all values, can pass on all values, but potentially leads to undefined behavior:

```cpp
const int i = 1, j = 2, k = 3;
E(int&, int&, int&);
f(i, j, k); // oops! E can modify a const object!
```

A final solution handles everything correctly...at the cost of being impossible to maintain. You provide overload of `f`, with *all* combinations of const and non-const:

```cpp
template <typename A, typename B, typename C>
void f(A& a, B& b, C& c);

template <typename A, typename B, typename C>
void f(const A& a, B& b, C& c);

template <typename A, typename B, typename C>
void f(A& a, const B& b, C& c);

template <typename A, typename B, typename C>
void f(A& a, B& b, const C& c);

template <typename A, typename B, typename C>
void f(const A& a, const B& b, C& c);

template <typename A, typename B, typename C>
void f(const A& a, B& b, const C& c);

template <typename A, typename B, typename C>
void f(A& a, const B& b, const C& c);

template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c);
```

N arguments require 2^N combinations, a nightmare. We'd like to do this automatically.

(This is effectively what we get the compiler to for us in C++11.)

-----------------------------------------------------

The solution is to instead use the newly added **rvalue-references**; we can introduce new rules when deducing rvalue-reference types and create any desired result. After all, we cannot possibly break code now.

If given a reference to a reference(note reference is an encompassing term meaning both `T&` and `T&&`), we use the following rule to figure out the resulting type:

|  TR  |  R   |      |
|------|------|------|
|  T&  |  &   |  T&  |
|  T&  |  &&  |  T&  |
|  T&& |  &   |  T&  |
|  T&& |  &&  |  T&& |

Next, with template argument deduction: if an argument is an lvalue A, we supply the template argument with a lvalue reference to A. Otherwise, we deduce normally. This gives so-called **universal references** (the term *forwarding reference* is now the official one).

Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was a lvalue, we have a lvalue-reference parameter, otherwise we have a rvalue-reference.

In code:

```cpp
template<typename T>
void deduce(T&& x);

int i;
deduce(i); // deduce<int&>(int& &&) -> deduce<int&>(int&)
deduce(1); // deduce<int>(int&&)
```

The last thing is to "forward" the value category of variable. Keep in mind, once inside the function the parameter could be passed as a **lvalue to anything**:

```cpp
void foo(int&);

template <typename T>
void deduce(T&& x)
{
    foo(x); // what foo gets is a lvalue. this isn't what we want.
}

deduce(1); // ok. deduce<int>(int&& x)
```

That's no good. foo needs to get the same kind of value-category that we got! The solution is:

```cpp
static_cast<T&&>(x);
```

Consider we're inside the `deduce` function:

1. we've been passed an lvalue. This means `T` is a `A&`, and so the target type for the static cast is `A& &&`, or just `A&`. Since `x` is already an `A&`, we do nothing and left with an lvalue reference.
2. we've been passed an rvalue. This means `T` is a `A`, so the target type for the static cast is `A&&`. The cast results in an rvalue expression, which can no longer be passed to an lvalue reference. We've maintained the value category of the parameter.

Putting these together gives us "perfect forwarding":

```cpp
template <typename T>
void f(T&& t)
{
    E(static_cast<T&&>(t));
}
```

When f receives a lvalue, E gets an lvalue. When f receives a rvalue, E gets a rvalue.

-----------------------------------------------------

```cpp
std::forward<T>(t);
// is same as
static_cast<T&&>(t);
```

## Reference

[Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward)
