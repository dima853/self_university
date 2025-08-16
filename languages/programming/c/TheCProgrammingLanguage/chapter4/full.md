# 🎯 **4.6 static variables**

Static variables in C have special properties:

## 🔒 **External static variables**
``c
static char buf[BUFSIZE]; // Visible only in this file
static int bufp = 0; // Does not conflict with other files
``
**Features:**
- Limit the viewport to the current file
- Useful for hiding internal variables from other parts of the program
- Save the value between function calls

## 🏠 **Internal static variables**
```c
void func() {
static int counter = 0; // Saves the value between calls
    counter++;
}
```
**Behaviour:**
- Initialized once
- Save the value between function calls
- Visible only inside the function

# ⚡ **4.7 register variables**

We recommend that the compiler store the variable in a register:
``c
register int x; // Optimization attempt
``
**Limitations:**
- Only for automatic variables and parameters
- It is impossible to get the address (&) of a register variable
- Modern compilers often ignore

# 🧱 **4.8 Block structure**

In C, you can create blocks with local variables:
``c
if (n > 0) {
    int i; // Local variable of the block
    for (i = 0; i < n; i++) {...}
}
``
** Features:**
- Variables exist only inside the block
- Can hide variables from external areas

# 🏁 **4.9 Initializing variables**

**Initialization rules:**
- Global and static variables: automatically reset to zero
- Automatic variables: contain "garbage" if not initialized

**Examples:**
``c
int x = 1; // Simple initialization
int days[] = {31,28,31}; // Initialization of the array
char str[] = "Hello"; // String initialization
```

# 🔄 **4.10 Recursion**

A recursive function calls itself. Example - printing a number:
``c
void printd(int n) {
    if (n < 0) { putchar('-'); n = -n; }
    if (n / 10) printd(n / 10);
    putchar(n % 10 + '0');
}
```

**Quicksort:**
``c
void qsort(int v[], int left, int right) {
    if (left >= right) return;
    int last = left;
    for (int i = left+1; i <= right; i++)
        if (v[i] < v[left]) swap(v, ++last, i);
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}
```

# 🛠 **4.11 C Preprocessor**

## 📂 **4.11.1 Enabling Files**
```c
#include <stdio.h> // System headers
#include "myheader.h" // Custom headers
``

## 🔀 **4.11.2 Macro Substitutions**
```c
#define MAX(a,b) ((a) > (b) ? (a) : (b)) // Macro with parameters
#define forever for(;;) // Infinite loop
```

**Special features:**
```c
#define fingerprint(expr) printf(#expr " =%g\n", expr) // String conversion
#define paste(front, back) front ## back // Concatenation
```

## ❓ **4.11.3 Conditional compilation**
```c
#if SYSTEM == LINUX
    #define HDR "linux.h"
#elif SYSTEM == WINDOWS
    #define HDR "windows.h"
#endif

#ifndef HEADER_H // Protection against re-activation
#define HEADER_H
/* header content */
#endif
```

**Key points:**
- `#ifdef` checks whether the macro is defined
- '#ifndef` protects against repeated activation
- Conditional compilation helps to create cross-platform code

These mechanisms provide powerful tools for code organization, scope management, and optimization of C programs.

 Detailed analysis of the C preprocessor (4.11.1-4.11.3)

The C preprocessor is a powerful tool that processes source code before compilation begins. Let's take a closer look at each of the key features.

## 4.11.1 File inclusion (#include)

### 🧩 Basic forms:
```c
#include <standard_title.h> // For system libraries
#include "user_file.h" // For your headlines
```

### 🔎 How it works:
1. **File search**:
- For `<...>`: the compiler searches in system directories
   - For `"..."`: first in the current directory, then in the system

2. **Mechanism of action**:
```c
// In main.c
#include "utils.h"
```
   The preprocessor literally *inserts the contents of*utils.h into main.c before compilation.

## Re-enable protection:
Problem: If the header is enabled multiple times, it may cause errors.
Solution:
```c
// In myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

/* header content */

#endif
```

### 💡 Practical application:
- Interface separation (.h) and implementations (.c)
- Creating common constants and declarations
- Connecting libraries

## 4.11.2 Macro substitutions (#define)

### 🧩 Basic macros:
```c
#define PI 3.14159 // Simple replacement
#define CIRCLE_AREA(r) (PI*(r)*(r)) // Macro with parameters
```

## Dangerous moments (and how to avoid them):
Problematic code:
```c
#define SQUARE(x) x*x
// For SQUARE(2+3) we get 2+3*2+3 = 11 instead of 25!
```

Corrected version:
```c
#define SQUARE(x) ((x)*(x)) // Always enclose parameters in parentheses!
```

### � Special operators:
1. **Operator #** (string conversion):
```c
   #define DEBUG_PRINT(x) printf(#x " = %d\n", x)
   // DEBUG_PRINT(count) → printf("count" " = %d\n", count)
   ```

2. **Operator ##** (token concatenation):
   ```c
   #define MAKE_VAR(n) var_##n
   // MAKE_VAR(1) → var_1
   ```

### 🏗 Advanced techniques:
Multi-line macros:
```c
#define SWAP(t,x,y) { t temp = x; \
                      x = y;      \
                      y = temp; }
```

## 4.11.3 Conditional compilation

### 🧩 Main directives:
```c
#if, #elif, #else, #endif // Conditions based on constants
#ifdef, #ifndef // Checking the definition of macros
#define, #undef // Defining/canceling macros
```

### 💻 Practical scenarios:

1. **Debugging code**:
```c
   #define DEBUG 1

   #if DEBUG
   printf("Debug: x=%d\n", x);
   #endif
   ```

2. **Cross-platform development**:
```c
   #if defined(_WIN32)
// Windows-specific code
#elif defined(__linux__)
// Linux-specific code
#endif
   ```

3. **Temporary code deactivation**:
```c
   #if 0
   /* This code will not be compiled */
   #endif
   ```

### 🔍 Checking definitions:
```c
#if defined(MAX_SIZE) // Is equivalent to #ifdef MAX_SIZE
// ...
#endif
```

### 📊 Directive comparison table:

| Directive | Testable condition              | Usage example          |
| --------- | ------------------------------- | ---------------------- |
| `#if`     | Value of the integer expression | `#if VERSION > 2`      |
| `#ifdef`  | Is the macro                    | `#ifdef DEBUG` defined |
| `#ifndef` | The macro is not defined        | `#ifndef HEADER_H`     |
| `#elif`   | Alternative condition           | `#elif SYSTEM == 1`    |
| `#else`   | Default branch                  | `#else`                |

# Best practices for working with the preprocessor

1. **Header files**:
- Always use include guard
   - Connect only the necessary headers
   - Avoid cyclical dependencies

2. **Macros**:
- The names of macros in the TOP_REGISTER
   - Enclose the parameters and the entire expression in parentheses
   - Avoid complex macros with side effects

3. **Conditional compilation**:
- Minimize platform-dependent code
- Comment on the reasons for conditional compilation
   - Avoid deeply nested #if's

An example of well-organized code:
```c
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#define APP_VERSION "1.2.3"
#define MAX_USERS 100

#ifdef DEBUG
#define LOG(msg) printf("[DEBUG] %s\n", msg)
#else
#define LOG(msg)
#endif

#endif
```

The C preprocessor is a powerful tool that, when used correctly, greatly simplifies the development of complex projects, but requires careful handling to avoid subtle errors.

 Macros and the preprocessor in C: a simple explanation with usage examples

A preprocessor is a "preprocessor" of your code that runs before compilation. Let's take it all in order.

## 1. What are macros?

Macros are a way to create "code templates" that will be automatically inserted before compilation.

### 🔹 Simple macros (constants)
```c
#define PI 3.14159
#define MAX_SIZE 100
```
**When to use**:
- When you need to set constants that may change
- To avoid "magic numbers" in the code

**Example**:
```c
double circle_area = PI * radius * radius;
if (count > MAX_SIZE) { ... }
```

### 🔹 Macros with parameters (as functions)
```c
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE(x) ((x)*(x))
```
**When to use**:
- For simple operations where no type checking is needed
- When speed is important (macros run faster than functions)

**Example**:
```c
int smallest = MIN(10, 20); // Will become ((10) < (20) ? (10) : (20))
int x_squared = SQUARE(5); // Will become ((5)*(5))
```

## 2. Basic preprocessor directives

### 🔹 #include — connecting files
```c
#include <stdio.h> // System Libraries
#include "my_lib.h" // Your own files
```
**When to use**:
- Whenever you need to use functions from other files
- To split the code into logical modules

## Conditional compilation
```c
#define DEBUG 1

#if DEBUG
printf("Debug info: %d\n", value); // Will be in the code only if DEBUG ==1
#endif
```
**When to use**:
- For debugging code
- For platform-dependent code
- To temporarily disable a part of the code

## 3. Practical examples when it is useful

## Example 1: Logging in debugging
```c
#define DEBUG 1

#ifdef DEBUG
    #define LOG(msg) printf("DEBUG: %s\n", msg)
#else
    #define LOG(msg)
#endif

int main() {
LOG("The program is running");
    // ... your code ...
}
```
**What gives**:
- In DEBUG mode, we see the messages
- Logs disappear automatically in the final version.

## Example 2: Cross-platform code
```c
#if defined(_WIN32)
    #define CLEAR "cls"
#elif defined(__linux__)
    #define CLEAR "clear"
#endif

system(CLEAR); // Will work on both Windows and Linux
```

## Example 3: Protection against re-activation
```c
// In myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

// Your header code

#endif
```
*Why**:
- To avoid errors when repeatedly including a single file

##4. When NOT to use macros

1. **Complex logic** — it is better to use functions
2. **Long calculations** — macros can inflate the code
3. **When type safety is important** — macros do not check types

Bad example:
``c
#define MULTIPLY(a, b) a * b
// Dangerous: MULTIPLY(2+3, 4+5) → 2+3*4+5 = 19 ( not 45)
``

A good alternative is:
``c
inline int multiply(int a, int b) { return a * b; }
```

## 5. The main rules of use

1. Write the names of macros IN UPPERCASE (`#define CONSTANT`)
2. Always enclose parameters in parentheses: `#define SQUARE(x) ((x)*(x))
3. Avoid side effects in arguments:
   ```c
   int x = 5;
   SQUARE(x++); // Dangerous: will increase x twice
   ```
4. For complex logic, use functions instead of macros.

# Conclusion: when to use what

| Situation                        | What to use          | Example                              |
| -------------------------------- | -------------------- | ------------------------------------ |
| Simple constants                 | #define              | `#define MAX 100`                    |
| Short frequent operations        | Parameterized macros | `#define MIN(a,b) ((a)<(b)?(a):(b))` |
| Debugging code                   | #ifdef DEBUG         | `#ifdef DEBUG printf(...)`           |
| Different versions for platforms | #if defined(...)     | `#if _WIN32 ... #endif`              |
| Complex logic                    | Functions            | `int min(int a, int b) { ... }`      |

The preprocessor is a powerful tool, but use it wisely! First, use simple #define for constants, and learn more complex features gradually.