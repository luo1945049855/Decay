# Chromium Bind

```cpp
template <typename Functor, typename... Args>
inline Callback<MakeUnboundRunType<Functor, Args...>>
Bind(Functor&& functor, Args&&... args) {
    return base::BindRepeating(std::forward<Functor>(functor),
                               std::forward<Args>(args)...));
}
```

## return type: `Callback<MakeUnboundRunType<Functor, Args...>>`

```cpp
// Returns a RunType of bound functor.
// E.g. MakeUnboundRunType<R(A, B, C), A, B> is evaluated to R(C).
template <typename Functor, typename... BoundArgs>
using MakeUnboundRunType =
    typename internal::BindTypeHelper<Functor, BoundArgs...>::UnboundRunType;

template <typename Functor, typename... BoundArgs>
struct BindTypeHelper
{
    static constexpr size_t num_bounds = sizeof...(BoundArgs);

    using FunctorTraits = MakeFunctorTraits<Functor>;
    using RunType = typename FunctorTraits::RunType;

};
```
