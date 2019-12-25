# How could you get the count of an Array in C++

## usual method

``` cpp
#define countof(array) (sizeof(array)) / (sizeof(array[0]))
```

This macro will give a wrong answer when you supply a raw pointer `int * p` or any object of a class that has a member function `operator []`. For example, suppose you write

``` cpp

class IntArray {
private:
    int * p;
    size_t size;

public:
    int & operator [] (size_t i);
} x;

```

## using compiler to detect a array

if we want the compiler to ensure that the parameter to *countof* is always an array, we have to **find a context where only an array is allowed**. The same context should reject any non-array expression.

### first try

``` cpp

template <typename T, size_t N>
size_t countof (T array[N]) {
    return N;
}

```

Unfortunately, this cannot pass compiling. because **C++ treats an array parameter the same as a pointer parameter**, ie. the above definition is equivalent to:

``` cpp

template <typename T, size_t N>
sizeof countof (T * array) {
    return N;
}

```

There is no way it can know what *N* is.

### second try

However, if a function expects an array *reference*, then the compiler does make sure that the size of the actual parameter matches the declaration. This means we can definition first try work with a minor modification:

``` cpp

template <typename T, size_t N>
sizeof countof (T (&array) [N]) {
    return N;
}

```

This *countof* works very well. However, it's a function, not a macro. This means you can only use it *during runtime* but not *compile time*. In particular, you cannot write something like:

```cpp

int x[10];
int y[2 * countof(x)]; //twice as big as x.

```

### third try

if we want to get *N* during compile time, we have to move it to *type info*. Following method move *N* to *return type*.

To be precise, We have to make the function return an *array reference*, as C++ does not allow you to return an array directly.

``` cpp

template <typename T, size_t N>
char (&_ArraySizeHelper( T (&array) [N] )) [N];

#define countof (array) (sizeof( _ArraySizeHelper( array )))

```

First, the top-level stuff

``` cpp

char ( &_ArraySizeHelper( ... )) [N];

```

says "_ArraySizeHelper is a function that return a reference(note the &) to a char array of *N* elements".

Next, the function parameter is

``` cpp

T (&array) [N]

```

which is a reference to a T array of N elements.

With this new definition,

``` cpp

int x[10];
int y[2 * countof(x)];//twice as big as x

```

becomes valid.

## Reference

[How Would You Get the Count of an Array in C++?](http://blogs.msdn.com/b/the1/archive/2004/05/07/128242.aspx)
