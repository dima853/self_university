#include "test_ifndef.h"
#include "test_pragma.h"
#include "test_ifndef.h" // Двойное включение
#include "test_pragma.h" // Двойное включение

int main()
{
    return function_ifndef() + function_pragma();
}