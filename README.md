# How to use

Simply place the header files in your project!
Note that before including you must define \<NAME\>_IMPLEMENTATION in your source file, for example:

```
#define STACK_IMPLEMENTATION
#include "stack.h"
```

This is done to prevent multiple inclusions of the implementation code, sparing the size of your binary.
