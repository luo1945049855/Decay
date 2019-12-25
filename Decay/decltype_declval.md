# decltype and declval

## std::decltype

decltype(entity)
decltype(expression)

  * ���������δ�����ŵı��ʽ��`decltyp` �õ����ʽ�����������͡�
  * �����������������ΪT�ı��ʽ�������¼������
    * ���ʽ��ֵ����Ϊxvalue����decltype�õ�T&&��
    * ���ʽ��ֵ����Ϊlvalue����decltype�õ�T&��
    * ���ʽ��ֵ����Ϊprvalue����decltype�õ�T��
  * ע�⣺��������Ǽ������ŵģ���������Ϊƽ����lvalue���ʽ����`decltype(x)`��
    `decltype((x))`���ǲ�ͬ�����͡�

Example:

```cpp
struct A { double x; }
const A* a;
decltype(a->x) y;                       // type of y is double;

template <typename T, typename U>
auto add(T t, U u) -> decltype(t+u)     // return type depends on template parameters
{
    return t+u;
}

int main()
{
    auto f = [](int a, int b)->int {
        return a * b;
    };

    // the type of a lambda function is unique and unnamed.
    decltype(f) g = f;                  
}
```

## std::declval

```cpp
template <class T>
typename std::add_rvalue_reference<T>::type declval() noexcept;
```

Converts any type T to a reference type, 
making it possible to use member functions in
`decltype` expressions without the need to go through constructors.

`declval` is commonly used in templates where acceptable template parameters
may have no constructor in common, but have the same member function
whose return type is needed.

Example:

```cpp
struct Default { int foo() const {return 1;}}
struct NoDefault {
    NoDefault(const NoDefault&) {}
    int fool() const { return 2; }
}

int main()
{
    decltype(Default().foo()) n1 = 1;                   // type of n1 is int.
    decltype(NoDefault().foo()) n2 = n1;                // error: no default constructor.
    decltype(std::declval<NoDefault>().foo()) n2 = n1;  // type of n2 is int.
    return 0;
}
```
