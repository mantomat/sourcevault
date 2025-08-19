# Coding standards

## Tools

Always follow the current clang-format configuration. You can find it at `.clang-format`.

Clang-tidy is configured in this project.  You can find the configuration at `.clang-tidy`. Follow its warnings and errors religiously.

## Style Conventions

You will find a lot of conventions in the codebase. Most are enforced by clang-tidy, but some are not. Always check if your coding style is compatible with the existing code.

For example, this project never uses `=` for assignments, but uniform-initialization instead. Example:

```cpp
int a = 4;

// becomes
int a{4};
```

## Exception-Free Design philosophy

SourceVault follows an original exception-free design approach. The reasons behind this decision are numerous:

1. I can't really tell when a situation is exceptional and when it is not. Can you?
2. The user is forced to check for unexpected conditions right when they could happen. This improves security and stability of the software.

This philosophy also brings some drawbacks. The C++ type system doesn't like this decision too much. Thus, code can easily become verbose and noisy. For example, the Static Factory Method Pattern is widespread in the project. This is because it's the only way to ensure data-oriented classes' invariants during creation: you can't just throw an exception in the constructor.

## Parameters and Return Types

Follow this simple rules:

- If you are writing a sink function (e.g. a function that always makes a copy of the parameter eventually), prefer accepting parameters by value. Even though this approach is not always optimal, it has several benefits:
    1. It gives flexibility to the user: they are free to choose to either move or copy the object.
    2. It's simple: you don't have to deal with overloads, rvalue references, perfect forwarding.

- If your function must take ownership of a dynamically allocated resource, use `std::unique_ptr`.

- If your function doesn't need to make copies of the object, nor to take ownership, use a const reference.

- **Always return by value**. RVO is magic.