#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Vulnerable primes (for demonstration purposes)
#define P 104723LL
#define Q 104729LL

// Function for exponentiation modulo (a^b mod m)
long long mod_pow(long long a, long long b, long long m) {
long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b = b / 2;
    }
    return result;
}

// Function for calculating the inverse element (a⁻1 mod m)
long long mod_inv(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

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