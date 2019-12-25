# return type

`Callback<MakeUnboundRunType<Functor, Args...>>`

MakeUnboundRunType->BindTypeHelper->FunctorTraits::RunType

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

```cpp
template <typename Functor>
using MakeFunctorTraits = FunctorTraits<std::decay_t<Functor>>;
//  FunctorTraits<> -- Type traits used determine the correct RunType and
//                     RunnableType for a Functor.  This is where function
//                     signature adapters are applied.
//                     There are |ARITY| ForceVoidReturn types.
```

## CHROMIUM

```cpp
//--------------------------------------CHROMIUM-------------------------//

// For empty callable types.
// This specialization is intended to allow binding captureless lambdas by
// base::Bind(), based on the fact that captureless lambdas are empty while
// capturing lambdas are not. This also allows any functors as far as it's an
// empty class.
// Example:
//
//   // Captureless lambdas are allowed.
//   []() {return 42;};
//
//   // Capturing lambdas are *not* allowed.
//   int x;
//   [x]() {return x;};
//
//   // Any empty class with operator() is allowed.
//   struct Foo {
//     void operator()() const {}
//     // No non-static member variable and no virtual functions.
//   };
template <typename Functor>
struct FunctionTraits<Functor,
                      std::enable_if_t<IsCallableObject<Functor>::value &&
                      std::is_empty<Functor>::value>> {
    using RunType = ExtractCallableRunType<Functor>;
    static constexpr bool is_method = false;
    static constexpr bool is_nullable = false;

    template <typename RunFunctor, typename... RunArgs>
    static ExtractReturnType<RunType> Invoke(RunFunctor&& functor,
                                             RunArgs&&... args) {
        return std::forward<RunFunctor>(functor)(std::forward<RunArgs>(args)...);
    }
}

// For functions.
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)>
{
    using RunType = R(Args...);
    static constexpr bool is_method = false;
    static constexpr bool is_nullable = true;

    template <typename... RunArgs>
    static R Invoke(R (*function)(Args...), RunArgs&&... args) {
        return function(std::forward<RunArgs>(args)...);
    }
};

// For methods.
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...)> {
    using RunType = R (Receiver*, Args...);
    static constexpr bool is_method = true;
    static constexpr bool is_nullable = true;

    template <typename ReceiverPtr, typename... RunArgs>
    static R Invoke(R (Receiver::*method)(Args...),
                    ReceiverPtr&& receiver_ptr,
                    RunArgs&&... args) {
        return ((*receiver_ptr).*method)(std::forward<Args>(args)...);
    }
};

// For const methods.
template <typename R, typename Receiver, typename... Args>
stuct FunctorTraits<R (Receiver::*)(Args...) const> {
    using RunType = R (const Receiver*, Args...);
    static constexpr bool is_method = true;
    static constexpr bool is_nullable = true;

    template <typename ReceiverPtr, typename... RunArgs>
    static R Invoke(R (Receiver::*method)(Args...) const,
                    ReceiverPtr&& receiver_ptr,
                    RunArgs&&... args) {
        return ((*receiver_ptr).*method)(std::forward(args)...);
    }
};
```
