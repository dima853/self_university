# 🧠 Pointers in C: A complete guide with examples

Pointers are one of the most powerful and complex features of the C language. Let's look at them in detail.

## 🔍 Basics of pointers

A pointer is a variable that stores the **memory address** of another variable.

```c
int x = 10;
int *p = &x; // p stores the address of x
```

### Basic operations:
- `&` is the address capture operator
- `*` - dereference operator (access to the value at the address)

## 📝 Example with getint

The 'getint` function uses a pointer to return two values.:
1. Through the return value, the operation status
2. Through the pointer, the number itself

```c
int getint(int *pn) {
// ...
*pn = 10; // Writing the value to
    return 1; // Successful execution
}

// Usage:
int num;
if (getint(&num) == 1) {
    printf("Got a number: %d", num);
}
```

## 🔄 Pointers and arrays

In C, pointers and arrays are closely related:

```c
int a[10];
int *pa = a; // Equivalent to &a[0]
``

### Equivalent entries:
``c
a[i]   ≡ *(a+i)
&a[i]  ≡ a+i
```

## 📊 Pointer arithmetic

Pointers support special arithmetic operations:

```c
int arr[5] = {1, 2, 3, 4, 5};
int *p = arr;

p++; // Now points to arr[1]
p += 2; // Now it points to arr[3]
int diff = p - arr; // diff = 3 (difference in positions)
``

## 🏗 Example with alloc and afree

Implementation of a simple memory allocator:

```c
#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    }
    return NULL;
}

void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}
```

# Important rules for working with pointers

1. Always initialize pointers
   ```c
   int *p = NULL; // Good
   int *p; // Bad (can point anywhere)
``

2. Check the pointers before using
   ```c
   if (p != NULL) {
       *p = 10;
   }
   ```

3. Do not go beyond the boundaries of arrays
   ```c
   int arr[5];
   int *p = arr + 10; // Danger!
   ```

## 🛠 Practical application

###1. Working with strings
```c
int strlen(char *s) {
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;
}
```

### 2. Passing arrays to functions
```c
void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
}
```

### 3. Dynamic memory allocation (malloc analog)
``c
char *alloc(int n) {
// ... (implementation above)
}
``

## 🎯 Withdrawal

Pointers in C give:
- Direct memory access
- Efficient work with arrays
- Ability to create complex data structures
- Flexibility in memory management