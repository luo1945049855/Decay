# SFINAE (Substitution Failure Is Not An Error)

When substituting the `explicitly specified` or `deduced` type for the template paramter fails, 
the speccialization is discarded from the overload set instead of causing a compiler error.

This feature is used in template metaprogramming.

The point of `SFINAE` is to `deactivate a piece of template code for certain types`.

The SFINAE technique is peformed by adding new overload functions
(function with same name and different type signatures) for preveting
compilation error during the template parameter type substitution.
When a substition failure happens,
the compiler looks for a the next function overload,
if the substition fails again and there are no more candidates, 
the compiler will generate an error.
