# 静态断言

* sizeof is a compile time operator.
* A_FLOAT may not appear in a constant expression.

```cpp
template<bool> struct StaticAssert;
template<> struct StaticAssert<true> {};

#define STATIC_ASSERT(condition) do { StaticAssert<(condition)>(); } while(0)

static const int   AN_INT  = 1000;
static const float A_FLOAT = 1000.0f;

int main()
{
   STATIC_ASSERT(AN_INT > 0);     // OK
   STATIC_ASSERT(A_FLOAT > 0.0f); // Error
}
```

## char s[] 与 char * s区别

The difference here is that`char *s = "Hello world"`will place "Hello world" in the read-only parts of the memory, and making s a pointer to that so makes any writing operation on this memory illegal.

While doing:`char s[] = "Hello world"`puts the literal string in read-only memory and copies the string to newly allocated memory on the stack. Thus making`s[0] = 'J'` is legal.
