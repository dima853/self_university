#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/**
 * @brief This function is vulnerable to timing attacks. It compares two MACs byte by byte and returns 0 (failure)
 *        as soon as a mismatch is found. This early exit creates a timing dependency, where the execution time
 *        depends on how many bytes match at the beginning of the comparison.
 *        - `for (int i = 0; i < 32; i++)`: This loop iterates through all 32 bytes of the MAC.
 *        - `if (correct_mac[i] != user_mac[i])`: This condition checks if the i-th byte of the `correct_mac`
 *          matches the i-th byte of the `user_mac`.
 *        - `return 0;`: If the bytes do not match, the function immediately returns 0, indicating a failed
 *          verification. This is the source of the timing vulnerability. The earlier this return occurs, the
 *          faster the function execution.
 *        - `return 1;`: If the loop completes without finding any mismatches, the function returns 1, indicating
 *          a successful verification.
 * @param correct_mac Pointer to the correct (expected) MAC.
 * @param user_mac Pointer to the user-provided MAC to verify.
 * @return 1 if the MACs match, 0 otherwise.
 */
int vulnerable_verify(const uint8_t *correct_mac, const uint8_t *user_mac) {
    for (int i = 0; i < 32; i++) {
        if (correct_mac[i] != user_mac[i]) {
            return 0; /* Exit on first error */
        }
    }
    return 1; /* All bytes matched */
}

/**
 * @brief A simplified (and insecure) HMAC function for demonstration purposes.  This function should NEVER
 *        be used in a production environment! It uses XOR and cyclic repetition of the key and message, making it
 *        extremely vulnerable to attacks.
 *        - `for (int i = 0; i < 32; i++)`: This loop iterates 32 times to create a 32-byte MAC.
 *        - `mac[i] = key[i % 16] ^ msg[i % 8];`: This line calculates each byte of the MAC by XORing a byte from
 *          the key and a byte from the message.
 *            - `key[i % 16]`: This accesses a byte from the key, using the modulo operator (`%`) to cyclically
 *              repeat the 16-byte key.  For example, when i is 16, `i % 16` will be 0, so `key[0]` will be used.
 *            - `msg[i % 8]`: This accesses a byte from the message, using the modulo operator to cyclically
 *              repeat the 8-byte message.
 *            - `^`: This is the XOR operator, which performs a bitwise exclusive OR between the key byte and the
 *              message byte.
 * @param key Pointer to the secret key.
 * @param msg Pointer to the message.
 * @param mac Pointer to the buffer where the generated MAC will be stored.
 */
void fake_hmac(const uint8_t *key, const uint8_t *msg, uint8_t *mac) {
    for (int i = 0; i < 32; i++) {
        mac[i] = key[i % 16] ^ msg[i % 8]; /* "Hash" via XOR */
    }
}

/**
 * @brief Gets the current time in nanoseconds using CLOCK_MONOTONIC. This is used for timing the `vulnerable_verify`
 *        function.  `CLOCK_MONOTONIC` provides a time source that is not affected by system clock changes, making it
 *        suitable for measuring elapsed time.
 *        - `struct timespec ts;`: This declares a `timespec` structure, which contains the seconds and nanoseconds
 *          components of the time.
 *        - `clock_gettime(CLOCK_MONOTONIC, &ts);`: This function retrieves the current time from the `CLOCK_MONOTONIC`
 *          clock and stores it in the `ts` structure.
 *        - `return ts.tv_sec * 1e9 + ts.tv_nsec;`: This calculates the total time in nanoseconds by multiplying
 *          the seconds component by 1 billion (1e9) and adding the nanoseconds component.
 * @return The current time in nanoseconds.
 */
uint64_t get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

/**
 * @brief Performs a timing attack to recover the correct MAC. This function exploits the timing vulnerability in
 *        `vulnerable_verify` to deduce the correct bytes of the MAC. It works by iterating through all possible
 *        byte values for each position in the MAC and measuring the time it takes for `vulnerable_verify` to
 *        compare the guessed MAC with the correct MAC. The byte value that results in the longest comparison time
 *        is assumed to be the correct byte.
 *        - `uint8_t known_mac[32] = {0};`: This initializes an array to hold the partially recovered MAC.  It starts
 *          with all bytes set to 0.
 *        - `for (int i = 0; i < 32; i++)`:  The outer loop iterates through each byte of the MAC (from byte 0 to byte 31).
 *        - `uint64_t max_time = 0;`:  This initializes a variable to store the maximum time observed for a given byte
 *          position.  It starts at 0.
 *        - `uint8_t correct_byte = 0;`:  This initializes a variable to store the correct byte value for a given
 *          position.  It starts at 0.
 *        - `for (uint8_t byte = 0; byte < 255; byte++)`: The inner loop iterates through all possible byte values (0 to 255).
 *          This loop attempts each possible value for the current byte position in the MAC. Note that I changed the limit from 256 to 255 to prevent potential overflow problems (uint8_t maxes out at 255). If it equals to 256, it will return to zero.
 *        - `known_mac[i] = byte;`:  This sets the i-th byte of the `known_mac` to the current `byte` value being tested.
 *        - `uint64_t total_time = 0;`:  This initializes a variable to store the total time taken for multiple iterations
 *          of the MAC comparison.
 *        - `for (int j = 0; j < 1000; j++)`:  This loop repeats the MAC comparison 1000 times to average out the timing
 *          measurements and reduce the impact of noise.
 *        - `uint64_t start = get_time();`:  This records the starting time before calling `vulnerable_verify`.
 *        - `vulnerable_verify(correct_mac, known_mac);`:  This calls the vulnerable function to compare the correct MAC
 *          with the current guess.
 *        - `total_time += get_time() - start;`:  This calculates the elapsed time for the comparison and adds it to
 *          the `total_time`.
 *        - `if (total_time > max_time)`:  This checks if the current `total_time` is greater than the previously recorded
 *          `max_time`.  If it is, it means that the current byte value resulted in a longer comparison time, suggesting
 *          it's a better guess.
 *        - `max_time = total_time;`: This updates the `max_time` to the current `total_time`.
 *        - `correct_byte = byte;`: This updates the `correct_byte` to the current byte value.
 *        - `known_mac[i] = correct_byte;`: This sets the i-th byte of `known_mac` to the recovered `correct_byte`.
 *        - `printf("Byte %d: %02X\n", i, correct_byte);`:  This prints the recovered byte to the console.
 *        - `printf("Recovered MAC: ");`: This prints a message to the console indicating that the recovered MAC will be displayed.
 *        - `for (int i = 0; i < 32; i++) printf("%02X", known_mac[i]);`: This loop iterates through all 32 bytes of the
 *          recovered MAC and prints them to the console in hexadecimal format.
 *        - `printf("\n");`: This prints a newline character to the console.
 *
 * @param correct_mac Pointer to the correct MAC that we are trying to recover.
 */
void timing_attack(const uint8_t *correct_mac) {
    uint8_t known_mac[32] = {0}; /* Initial guess */

    for (int i = 0; i < 32; i++) { /* Iterate over each MAC byte */
        uint64_t max_time = 0;
        uint8_t correct_byte = 0;

        for (uint8_t byte = 0; byte < 255; byte++) { /* Iterate 0x00..0xFF (byte values) */
            known_mac[i] = byte;
            uint64_t total_time = 0;

            /* Average time (to suppress noise) */
            for (int j = 0; j < 1000; j++) {
                uint64_t start = get_time();
                vulnerable_verify(correct_mac, known_mac);
                total_time += get_time() - start;
            }

            /* Store byte with the max time */
            if (total_time > max_time) {
                max_time = total_time;
                correct_byte = byte;
            }
        }

        known_mac[i] = correct_byte; /* Fix the guessed byte */
        printf("Byte %d: %02X\n", i, correct_byte);
    }

    /* Output the recovered MAC */
    printf("Recovered MAC: ");
    for (int i = 0; i < 32; i++) printf("%02X", known_mac[i]);
    printf("\n");
}

/**
 * @brief Main function. Sets up the key, message, generates the (fake) HMAC using `fake_hmac`, and launches the
 *        timing attack using `timing_attack`.
 *        - `uint8_t key[16] = "secret_key_12345";`: This declares a 16-byte array `key` and initializes it with a
 *          sample secret key. Remember, this key should be randomly generated and securely stored in a real application.
 *        - `uint8_t msg[8] = "message";`: This declares an 8-byte array `msg` and initializes it with a sample message.
 *          This message will be used to generate the HMAC.
 *        - `uint8_t correct_mac[32];`: This declares a 32-byte array `correct_mac` to store the generated HMAC.
 *        - `fake_hmac(key, msg, correct_mac);`: This calls the `fake_hmac` function to generate the HMAC using the
 *          `key` and `msg` and stores the result in `correct_mac`. This should be a secure HMAC in a real application.
 *        - `timing_attack(correct_mac);`: This calls the `timing_attack` function to attempt to recover the `correct_mac`
 *          by exploiting the timing vulnerability in `vulnerable_verify`.
 *        - `return 0;`: This returns 0 to indicate that the program executed successfully.
 *
 * @return 0 on success.
 */
int main() {
    uint8_t key[16] = "secret_key_12345"; /* Secret key */
    uint8_t msg[8] = "message";           /* Message */
    uint8_t correct_mac[32];              /* Correct MAC */

    /* Generate HMAC */
    fake_hmac(key, msg, correct_mac);

    /* Launch the attack */
    timing_attack(correct_mac);

    return 0;
}