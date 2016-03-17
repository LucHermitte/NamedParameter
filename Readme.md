# Named parameters for C++14 [![Build Status](https://secure.travis-ci.org/LucHermitte/NamedParameter.png?branch=master)]

Here is another proof of concept to implement
[named parameters](https://en.wikipedia.org/wiki/Named_parameter) in C++.

__Discl.:__ This experiment requires C++14 and support of
[n3599](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3599.html)
(gcc and clang support this as a non standard extension).
[Tests are passing with g++5.0+ and clang 3.6+](https://travis-ci.org/LucHermitte/NamedParameter).

In a few words, this header only library permits to write:

```c++
// service
template <typename ...Args>
void f(Args&& ...args)
{
    using namespace na::literals;
    // non mandatory
    auto && a = na::get("a"_na = 1    , std::forward<Args>(args)...);
    auto && c = na::get("c"_na = "bar", std::forward<Args>(args)...);
    // mandatory
    auto && b = na::get("b"_na        , std::forward<Args>(args)...); // a reference
    auto    d = na::get("d"_na        , std::forward<Args>(args)...); // a copy
    auto && e = na::get("e"_na        , std::forward<Args>(args)...); // a const-reference
    auto && i = na::get("i"_na        , std::forward<Args>(args)...); // a reference

    // do stuff with the parameters
    ++i;
}

// client
using namespace na::literals;
auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
auto const x = std::vector<int>{ 11, 12, 13, 14, 15};
auto       i = 0;
// in no particular order
f( "d"_na=12.5, "a"_na=42, "c"_na="foo", "b"_na=v, "e"_na=x, "i"_na = i);
assert(i==1);

// without calling the optional parameters
f("d"_na=12.5, "b"_na=v, "e"_na=x, "i"_na = i);
assert(i==2);
```

## Objectives

- Provide a library/framework for named parameters in C++
- No overhead:
  - Everything is meant to be done at compile time
  - No dynamic allocation is done
  - Rely on perfect forwarding to move or reference things
- No macros
- Simple to use:
  - The end-user doesn't need to manually define a type for each possible
    argument, this is done automagically thanks to C++11 user-defined literals
  - Only one function to fetch argument values, with or without default values
  - Header-only
- Support
  - References
  - Non copyable types
  - Moved stuff
  - Default values
- Errors are detected at compile time
  - Multiple instantiations of a same parameter (the current implementation
    does not detect this error with g++ 4.9)
  - Missing mandatory parameters
  - Default values not compatible with parameters
- As close to C++11 as possible
  - Unfortunately that wasn't possible, for simplicity reasons, this is a
    C++14 code, that relies on a non-standard extension of gcc and clang.

## Design considerations

### one parameter == one type
I went (as similar libraries do) to one parameter == one very specific type.
This way, the variadic and generic list of parameters can be analysed at
compile time.

In order to define those types, we could ask the end-user to explicitly define
one type for each possible parameters for all the functions were he wants named
parameters. While it's a good exercise to play with variadic template, this
solution is not acceptable for the end-user.

Some libraries use macros to define those types on-the-fly. I went for C++11
user-define literals. Well. Not exactly. The standard permits to have the
literal either cooked for us, or still in their raw form. In order to define a
type that can be used with templates, the raw form is necessary.
Unfortunately, the standard does not offer raw literals on string. This is a
[gcc extension](https://gcc.gnu.org/ml/gcc-patches/2013-04/msg00998.html),
that clang also supports (I haven't checked VC++ nor ICC, and I'm not sure
whether it'll be integrated in future versions of the language).

### Passing the value

In order to pass the value from the call site to a local variable acting as an
argument, the string literal type builds and returns a proxy object we assign a
value to. The proxy object stores a reference to the parameter value.

In order to avoid undesired copies, parameters are perfect-forwarded.

If we want to get a reference, a const reference, or a copy, we have to just
say so:

```c++
auto        copy           = na::get(....);
auto      & ref            = na::get(....);
auto const& constref       = na::get(....);
auto const nonmutablecopy  = na::get(....);
auto    && ref_or_constref = ... // see literature on perfect forwarding
```

NB: it's not necessary to extract all the parameters passed.

### Fetching parameters
This operation is done through a recursive analysis of the parameter types
generated on the fly. The analysis is done on types at compile time.

We may require a `na::is_set()` function for optional parameters.

### Default values
While it'll be easy to have dedicated functions, like `get()` and
`get_default()`, or `mandatory()` and `defaulted()`, ..., I choose to have only
one function in order to simplify user code.

Unlike usual C++ code, the default value cannot be declared in function
prototypes, it has to be declared in function definitions where local values
are bound to the arguments. I choose to reuse the `"argument"_na = value`
syntax (and thus the proxy object). This way, we can know a compile time
whether an argument in mandatory or not, and thus generate compile time errors
on misuses.


### Compile time errors

A few techniques have been used to provide compilation error messages as clean
as possible:
- `static_assert` + a `wrong_t` that depends on a parameter in order to raise
  the error only when the function is actually called.  This have been used for
  missing mandatory parameters
- tags dispatching has been used to call the right function depending on
  whether the first parameters in the one searched or not.

The library currently detects:
- a missing mandatory parameter
- a returned parameter not compatible with the default value provided
- a same parameter kind used multiple times in a same function call.

## When to use this library

According you're working in C++14 and with a compiler that provides the
required compiler-extension.

This library can be used with any function. This means all functions would
become template functions. This is not something we wish to systematized as
it'll make all functions inline.

The best is to reserve it to functions that are already template and that have
many parameters we don't wish to mix up.

Or also to constructors.  Named parameters are a neat alternative to setters.
It's easy to forget to call a setter: the compiler will never tell us about it.
With named parameters, we cannot mix up arguments and parameters nor forget to
set them to pertinent values.

## Running the tests

Unit tests are provided in the `test/` directory. They require boost and
CMake+CTest.

In order to execute them, you'll have to run from your console:

```bash
mkdir build && cd build
# With your default compiler
cmake -DCMAKE_BUILD_TYPE=Release path/to/project/root && make && ctest
# To force the use of clang++:
CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release path/to/project/root && make && ctest
```

## TO DO

- permit to force the result type in get<>
- compare assembler to the equivalent code
- check with lambdas, static arrays, function pointers, ...
- positional parameters for the unnamed parameters
- add some constexpr?
- doxygen

## See also
- https://isocpp.org/wiki/faq/ctors#named-parameter-idiom
- https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter
- http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4172.htm
- https://marcoarena.wordpress.com/2014/12/16/bring-named-parameters-in-modern-cpp/
- http://www.boost.org/doc/libs/1_60_0/libs/parameter/doc/html/index.html
- http://justinvh.blogspot.fr/2012/10/c11-named-parameters-using-operator.html
- https://bitbucket.org/p2rkw/namedarguments/overview
- https://github.com/cheshirekow/kwargs


## Copyright

(c) 2016, Luc Hermitte, Boost Sofware License
