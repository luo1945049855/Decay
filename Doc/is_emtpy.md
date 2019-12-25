# std::is_empty

If T is an empty type, provides the member constant value equal `true`, for any other type, value is `false`.
If T is a non-union class type, T shall be a complete type; otherwise, the behavior is undefined.

1. non-union
2. with no non-static data members other than bit-fields of size 0
3. no virtual functions
4. no virual base classes
5. no non-empty base classes
