#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

/**
* Calculates the greatest common divisor (GCD) of two numbers
 * using Euclid's algorithm.
 *
* @param a The first number
 * @param b The second number
 * @return NODE(a, b)
*
* Algorithm:
* 1. While b is not equal to 0:
* - Save the current value of b in a temporary variable
 * - Update b as a mod b
 * - Assign the stored value b
* 2 to a. When b becomes 0, we return a as the result
 *
* Difficulty: O(log(min(a,b)))
*/
long long gcd(long long a, long long b) {
while(b != 0) {
        long long temp = b; // Saving the current b
        b = a % b; // Update b as the remainder of the division
        a = temp; // Restoring a from a temporary value
    }
    return a;
}

/**
 * Calculates a^b mod m (exponentiation modulo)
* using the fast exponentiation method.
 *
* @param a Foundation
 * @param b Exponent
 * @param m Module
 * @return a^b mod m
 *
* Algorithm:
* 1. Initialize the result as 1
* 2. We reduce a modulo m
* 3. As long as b > 0:
* - If b is odd, multiply the result by a mod m
 * - Squaring a mod m
 * - Divide b by 2 (integer division)
*
* Difficulty: O(log b)
*/
long long mod_pow(long long a, long long b, long long m) {
long long result = 1; // Initialization of the result
    a = a % m; // Reduction of a modulo m
    
    while (b > 0) {
        if (b % 2 == 1) { // If b is odd
            result = (result * a) % m; // Multiplying the result by a mod m
        }
        a = (a * a) % m;  // Squaring a mod m
        b = b / 2; // Integer division of b by 2
    }
    
    return result;
}

/**
* A pseudorandom function for Pollard's algorithm.
 * f(x) = (x^2 + c) mod n
 *
* @param x Input value
 * @param c Constant
 * @param n Module
 * @return (x^2 + c) mod n
 * 
 * Note:
* The function generates a deterministic pseudorandom sequence,
 * used to find collisions in Pollard's algorithm.
 */
long long f(long long x, long long c, long long n) {
    return ((x * x) % n + c) % n;  // x^2 + c mod n
}

/**
* Implementation of the Pollard factorization algorithm (p-algorithm).
 * Finds a nontrivial divisor of the number n.
 *
* @param n is the number to factorize
 * @return Non-trivial divisor n
*
* Algorithm:
 * 1. Check the trivial divisors (2, 3, 5)
* 2. Infinite loop:
* - Choose a random c and initial x
* - Initialize y = x
* - Inner loop:
* * x = f(x)
* * y = f(f(y)) (moving to 2 times faster)
* * Calculate d = NODE(|x-y|, n)
* * If d > 1 and d < n, return d
 * * If x == y, we start over with a new c
*
* Difficulty: O(n^(1/4)) in the average case
 */
long long pollards_rho(long long n) {
// Checking for trivial divisors
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    if (n % 5 == 0) return 5;

    long long c, x, y, d;

    while (1) {
        c = rand() % (n - 1) + 1; // Random c ∈ [1, n-1]
        x = rand() % (n - 1) + 1; // Random initial x
        y = x; // Initialization of y

        do {
            x = f(x, c, n); // One step for x
            y = f(f(y, c, n), c, n); // Two steps for y
            d = gcd(llabs(x - y), n); // Calculate the NODE of difference and n
} while (d == 1); // Until we find the divisor

        if (d != n) return d; // Return the found divisor
    }
}

/**
 * Calculates the modular inverse element (e⁻1 mod phi)
* using the extended Euclidean algorithm.
 *
* @param e Number
 * @param phi Module
 * @return e⁻¹ mod phi
 *
* Algorithm:
* 1. Save the original phi
* 2. Initialize the coefficients x0=0, x1=1
* 3. Until e > 1:
 * - Calculate the quotient q = e/phi
* - Update phi as e%phi, e as old phi
 * - Update the coefficients x0 and x1
* 4. If x1 is negative, add m0
*
* Difficulty: O(log min(e, phi))
*/
long long mod_inv(long long e, long long phi) {
    long long m0 = phi, t, q; // Save the original phi
    long long x0 = 0, x1 = 1; // Initialization of coefficients

    if (phi == 1) return 0; // Special case

    while (e > 1) {
        q = e / phi; // Quotient of division
        t = phi; // Save phi
        phi = e % phi; // New phi as remainder
        e = t; // Updating e
        t = x0; // Save x0
        x0 = x1 - q * x0; // Update x0
        x1 = t; // Updating x1
    }

    if (x1 < 0) x1 += m0; // Adjust if x1 is negative
    return x1;
}

int main() {
    srand(time(0));

    // Public key (n, e) and ciphertext c
long long n = 10966061467LL;
    long long e = 65537;
    long long c = 748847340;

    printf("[+] Attack on RSA with Pollard's Rho:\n");
    printf("n = %lld\n", n);
    printf("e = %lld\n", e);
    printf("c = %lld\n", c);

    // Factorize n
long long p = pollards_rho(n);
long long q = n / p;

    printf("\n[+] Found divisors:\n");
    printf("p = %lld\n", p);
    printf("q = %lld\n", q);

    // We calculate φ(n) and d
    long long phi = (p - 1) * (q - 1);
    long long d = mod_inv(e, phi);

    printf("\n[+] Calculating the secret key:\n");
    printf("φ(n) = (p-1)(q-1) = %lld\n", phi);
    printf("d = e⁻¹ mod φ(n) = %lld\n", d);

    // Decoding the message
    long long m = mod_pow(c, d, n);
    printf("\n[+] Decrypted message:\n");
    printf("m = c^d mod n = %lld\n", m);

    return 0;
}