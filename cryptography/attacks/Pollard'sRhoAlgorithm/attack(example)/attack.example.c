#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// Function for computing nodes (Euclid's algorithm)
long long gcd(long long a, long long b) {
while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

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
// Pseudorandom function (x2 + c) mod n
long long f(long long x, long long c, long long n) {
return ((x *x) %n + c) % n;
}

// Pollard's Rho for factorization
long long pollards_rho(long long n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    if (n % 5 == 0) return 5;

    long long c, x, y, d;

    while (1) {
        c = rand() % (n - 1) + 1;
        x = rand() % (n - 1) + 1;
        y = x;

        do {
            x = f(x, c, n);
            y = f(f(y, c, n), c, n);
            d = gcd(llabs(x - y), n);
        } while (d == 1);

        if (d != n) return d;
    }
}

// Function for calculating d = e⁻1 mod φ(n)
long long mod_inv(long long e, long long phi) {
    long long m0 = phi, t, q;
    long long x0 = 0, x1 = 1;

    if (phi == 1) return 0;

    while (e > 1) {
        q = e / phi;
        t = phi;
        phi = e % phi;
        e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
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