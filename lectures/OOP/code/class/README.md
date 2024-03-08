https://en.wikipedia.org/wiki/Name_mangling

C++ name mangling is not standardized
Originally C++ was translated to C - 

_ZN1A1fEi:

All mangled symbols begin with _Z (note that an identifier beginning with an underscore followed by a capital letter is a reserved identifier in C, so conflict with user identifiers is avoided); for nested names (including both namespaces and classes), this is followed by N, then a series of <length, id> pairs (the length being the length of the next identifier), and finally E. For example, wikipedia::article::format becomes:

E indicates the end
i - function takes a single int

Demangle use c++filt -n

Name encoding - see 7.2 ARM
- overloaded functions
  - same name with different arguments ... argument types added to the name
- type-safe linkage



class layouts

https://stackoverflow.com/questions/51005713/dumping-memory-layout-of-c-object-does-not-work-in-clang

https://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/:


clang -cc1 -fdump-record-layouts -emit-llvm AB.cpp
clang -cc1 -fdump-vtable-layouts -emit-llvm AB.cpp

-fdump-record-layouts-simple

