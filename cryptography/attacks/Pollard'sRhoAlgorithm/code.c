#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Function for computing nodes (Euclid's algorithm)
long long gcd(long long a, long long b) {
while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Pseudorandom function (x^2+c) mod n
long long f(long long x, long long c, long long n) {
return ((x*x) %n + c) %n;
}

// Pollard's Rho factorization algorithm
long long pollards_rho(long long n) {
// Checking for trivial divisors
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    if (n % 5 == 0) return 5;

    long long c, x, y, d;

    while (1) {
        // Choose a random c (usually from 1 to n-1)
c = rand() % (n - 1) + 1;

        // Initialization of "turtle" (x) and "hare" (y)
x = rand() % (n - 1) + 1;
        y = x;

        do {
            // The turtle takes one step
            x = f(x, c, n);

            // The hare takes two steps
            y = f(y, c, n);
            y = f(y, c, n);

            // Calculating the difference and the NODE
            d = gcd(llabs(x - y), n);

            // If you have found a divisor
            if (d > 1 && d < n) {
                return d;
            }
        } while (x != y); // No collision has been found yet

        // If d == n, try another c
    }
}

int main() {
    long long n, factor;

    printf("Enter the number to factorize: ");
    scanf("%lld", &n);

    if (n == 1) {
        printf("1 has no divisors except itself.\n");
        return 0;
    }

    printf("Divisor %lld: ", n);
    factor = pollards_rho(n);
    printf("%lld\n", factor);

    return 0;
}