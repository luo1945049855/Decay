# Callback

```cpp
// -----------------------------------------------------------------------------
// Usage documentation
// -----------------------------------------------------------------------------
//
// Overview:
// A callback is similar in concept to a function pointer: it wraps a runnable
// object such as a function, method, lambda, or even another callback, allowing
// the runnable object to be invoked later via the callback object.
//
// Unlike function pointers, callbacks are created with base::BindOnce() or
// base::BindRepeating() and support partial function application.
//
// A base::OnceCallback may be Run() at most once; a base::RepeatingCallback may
// be Run() any number of times. |is_null()| is guaranteed to return true for a
// moved-from callback.
//
//   // The lambda takes two arguments, but the first argument |x| is bound at
//   // callback creation.
//   base::OnceCallback<int(int)> cb = base::BindOnce([] (int x, int y) {
//     return x + y;
//   }, 1);
//   // Run() only needs the remaining unbound argument |y|.
//   printf("1 + 2 = %d\n", std::move(cb).Run(2));  // Prints 3
//   printf("cb is null? %s\n", cb ? "true" : "false");  // Prints true
//   std::move(cb).Run(2);  // Crashes since |cb| has already run.
//
// Callbacks also support cancellation. A common use is binding the receiver
// object as a WeakPtr<T>. If that weak pointer is invalidated, calling Run()
// will be a no-op. Note that |is_cancelled()| and |is_null()| are distinct:
// simply cancelling a callback will not also make it null.
//
// base::Callback is currently a type alias for base::RepeatingCallback. In the
// future, we expect to flip this to default to base::OnceCallback.
//
```
