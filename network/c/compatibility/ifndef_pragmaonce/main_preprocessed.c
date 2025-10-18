# 0 "main.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "main.c"
# 1 "test_ifndef.h" 1



int function_ifndef()
{
    return 42;
}
# 2 "main.c" 2
# 1 "test_pragma.h" 1
       

int function_pragma()
{
    return 24;
}
# 3 "main.c" 2



int main()
{
    return function_ifndef() + function_pragma();
}
