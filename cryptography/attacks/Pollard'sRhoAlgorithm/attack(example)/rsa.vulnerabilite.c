#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/**
* Calculates a^b mod m (exponentiation modulo)
*
* @param a Base
 * @param b Exponent 
 * @param m Module
 * @return a^b mod m
 * 
 * Operating principle:
 * Uses the "exponentiation through squares" method for efficient calculation.
 * The basic formula: 
 * a^b mod m = (a^(b/2) mod m)^2 mod m if b is even
 * a^b mod m = (a * a^(b-1) mod m) mod m if b is odd
*/
long long mod_pow(long long a, long long b, long long m) {
long long result = 1;
    a = a % m; // First we take a modulo m
    
    while (b > 0) {
        // If b is odd, multiply the result by a mod m
        if (b % 2 == 1) {
            result = (result * a) % m;
        }
        
        // Now b is exactly even - we square a
        a = (a * a) % m;
        
        // Divide b by 2
        b = b / 2;
    }
    
    return result;
}

/**
 * Calculates the modular inverse element (a⁻1 mod m)
*
* @param a The number for which we are looking for the inverse element
 * @param m Module
 * @return a⁻¹ mod m
 * 
 * Operating principle:
 * Uses the extended Euclidean algorithm to solve the equation:
 * a*x ≡ 1 mod m
 * where x is the desired inverse element.
 *
* The algorithm is based on the fact that if gcd(a,m)=1, then there exist x,y such that:
 * a*x + m*y = 1
 * then x mod m and will be the inverse element.
 */
long long mod_inv(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1; // Coefficients for the equation a*x + m*y = gcd(a,m)

    if (m == 1) return 0; // Special case

    // Using the extended Euclidean algorithm
    while (a > 1) {
        // q is the quotient of a by m
        q = a / m;
        
        t = m;
        
        // m becomes the remainder of dividing a by m
        // corresponds to a step in the usual Euclidean algorithm
        m = a % m;
        a = t;
        
        // Updating the coefficients
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // Make sure that x1 is positive
    if (x1 < 0) x1 += m0;
    
    return x1;
}

int main() {
    srand(time(0));

    // RSA Key Generation
    long long p = P;
    long long q = Q;
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    long long e = 65537; // Standard public exponent
long long d = mod_inv(e, phi); // Secret key

    printf("[+] RSA key generation:\n");
    printf("p = %lld\n", p);
    printf("q = %lld\n", q);
    printf("n = p * q = %lld\n", n);
    printf("φ(n) = (p-1)(q-1) = %lld\n", phi);
    printf("e = %lld\n", e);
    printf("d = e⁻¹ mod φ(n) = %lld\n", d);

    // We encrypt the message m = 123456789
    long long m = 123456789;
    long long c = mod_pow(m, e, n);
    printf("\n[+] Encryption:\n");
    printf("Message m =%lld\n", m);
    printf("Ciphertext c= m^e mod n=%lld\n", c);

    return 0;
}