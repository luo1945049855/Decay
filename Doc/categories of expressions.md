# Categories of expressions

![categories of expressions](./imgs/categories_of_expressions.png)

## xvalue

-An xvalue(an **"eXpiring"** value) also refer to an object, usually near the **end of its lifetime**(so that its resources may be moved, for example). An xvalue is the result of certain kinds of expressions involving rvalue references. [Example: The result of calling a function whose return type is an rvalue reference is an xvalue. --end example]

## lvalue

-An lvalue(so called, historically, because lvalues could appear on the left-hand side of an assignment expression.) designates a function or an object.
[Example: if E is an expression of pointer type, the *E is an lvalue expression referring to the object or function to which E points. As another example, the result of calling a function whose return type is **an lvalue reference** is a lvalue].

## glvalue

-A glvalue("generalized" lvalue) is an lvalue or an xvalue.

## rvalue

-An rvalue(so called, historically, because rvalues could appear on the right-hand side of an assignment expressions) is an xvalue. a **temporary object or subobject thereof**, or a **value that is not associated with an object**.

## prvalue

-A prvalue("pure" rvalue) is rvalue that is not an xvalue.[Example: The result of calling a function whose return type is not a reference is a prvalue. The value of a **literal** such as 12, 7.3e5, or ture is also a prvalue --end example].

## Reference

1. [What are rvalues, lvalues, xvalues, glvalues, and prvalues?](https://stackoverflow.com/questions/3601602/what-are-rvalues-lvalues-xvalues-glvalues-and-prvalues)
