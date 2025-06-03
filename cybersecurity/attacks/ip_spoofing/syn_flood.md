**"How Checksum works in networks: an analysis of RFC 1071"**

---

**Introduction:**

In the world of networks, data transmission must be reliable. Imagine that you are sending an important document by e-mail, and it gets corrupted during transmission. To avoid such troubles, checksums are used. The checksum is like a "fingerprint" of your data, which allows you to make sure that the data has not changed during the transfer.  In this post, we will look at how the checksum is calculated using the algorithm described in RFC 1071, the standard underlying many network protocols (TCP, UDP, IP).

---

**The `checksum' function (Code parsing):**

Here is a function in C that calculates the checksum according to RFC 1071:

```c
unsigned short checksum(void *b, int len) {
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    unsigned long sum = 0;
    unsigned short *addr = (unsigned short *)b; // pointer conversion

     while( len > 1 )  {
        /*  This is the inner loop */
            sum += *addr++;
            len -= 2;
    }

        /*  Add left-over byte, if any */
    if( len > 0 )
            sum += *(unsigned char *)addr;

        /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return (unsigned short)(~sum);
}
```

Let's analyze this function line by line.:

1. **`unsigned short checksum(void *b, int len) { ... }`:**
* This is a function declaration.
    * `unsigned short`: The type of the returned value (the checksum is a 16-bit number).
    * `void *b': A pointer to the data for which the checksum needs to be calculated.  `void *` means that the function can process any type of data.
    * `int len`: The length of the data in bytes.
2. **`unsigned long sum = 0;`:**
* Declare a variable `sum` of type `unsigned long` (32 bits), which will store the sum of the data.  Initialize it with zero.  `unsigned long` is used to avoid overflow during summation.
3. **`unsigned short *addr = (unsigned short *)b;`:**
* **The most important:** We take the pointer `b` (pointing to our data) and *convert* it to the type `unsigned short*`.
    * `(unsigned short *)b`: This is called "type casting". We tell the compiler: "Consider the data pointed to by `b` as a sequence of 16-bit numbers (`unsigned short`)."
* `addr`: `addr` now points to the same data as `b', but we interpret them as 16-bit numbers.
4.  **`while( len > 1 ) { ... }`:**
    *   A loop that processes data in 16-bit blocks (2 bytes each).
    *   The loop runs as long as the length of the data (`len') is more than 1 byte.

    * **Inside the loop:**
* `sum += *addr++;`:
            *   `*addr`: Dereference `addr'. This means "take the value that `addr` points to". Since 'addr` points to an `unsigned short', we take a single 16-bit number.
            * `sum += ...`: Add this 16-bit number to the `sum` variable.
            * 'addr++': We increase the pointer `addr` by 2 bytes (the size of the `unsigned short`).  We move on to the next 16-bit number.
        * `len -= 2;`: We reduce the length of `len` by 2, because we processed 2 bytes.
5. **`if( len > 0 ) { ... }`:**
* We check if there is any raw data left (when the length was odd).
    * `sum += *(unsigned char *)addr;`: If there is one byte left:
        * `(unsigned char *)addr`: We convert `addr` to the type `unsigned char *` in order to treat it as a pointer to one byte.
        * `*...`: We take the value of this byte.
        * `sum += ...`: Add this byte to the `sum`.
6.  **`while (sum>>16)`:**
    *   A loop that handles overflow (if the `sum` has become too large).
    * `sum >> 16`: Shifts the bits of `sum` by 16 positions to the right, highlighting the highest 16 bits (if any).
    * `sum = (sum & 0xffff) + (sum >> 16);`: Adds the low-order 16 bits (`sum & 0xffff`) with the high-order bits shifted (`sum >> 16`).
7. **` return (unsigned short)(~sum);`:**
* `~sum`: Bitwise inversion: each bit of the `sum` is reversed (0 becomes 1 and 1 becomes 0).
    *   `(unsigned short)(...)`: Convert the result to the type `unsigned short` (16-bit number).
    * `return ...`: We return the calculated checksum.

---

**Example (for a post):**

Imagine that we want to calculate the checksum for the following data (bytes): `0x12 0x34 0x56 0x78`.

1. The function receives this data and the length is 4 bytes.
2. The summation of 16-bit blocks begins:
* `0x1234` + `0x5678` =  `0x68AC`
3. Since all 4 bytes have been processed, the loop with `len > 0` is skipped.
4. The overflow processing cycle is also skipped in this case, since the summation result is placed in 16 bits.
5. The result is inverted: `~0x68AC` = `0x9753`.
6. The function returns `0x9753'.

---

**How it is used (in short):**

1. **Calculation:** The sender calculates a checksum for their data before sending.
2. **Transfer:** The sender adds a checksum to the data and sends it over the network.
3. **Verification:** The recipient calculates a checksum for the received data.
4. **Comparison:** The recipient compares the calculated checksum with the received one.
5. **Result:**
* **If the checksums match:** The data was most likely not corrupted.
    * **If the checksums do not match:** The data is corrupted and needs to be requested again.

---

**Conclusion:**

The checksum algorithm according to RFC 1071 is a simple but effective way to verify data integrity in networks. This C function implements this algorithm, ensuring reliable data transmission and helping to detect errors.  Understanding this principle is important for working with network protocols and ensuring secure transmission of information.

---

# STEP 4

In step 4, when we talk about the `while(len > 1 ) { ... }` loop, we are essentially splitting our 4 bytes of data into pairs (two 16-bit "numbers" or words).

This is how it works in the context of our example with the data `0x12 0x34 0x56 0x78`:

1. **Start of the cycle:** `len = 4`
2. **First pass of the loop:**
* `sum += *addr++;`
        *   `*addr`:  Dereferences the `addr'.  'addr` points to the first two bytes: `0x12 0x34`.  We consider these two bytes as one 16-bit number: `0x1234` (4660).
* `sum += 0x1234;`
        * `addr++`: The pointer `addr` is shifted, now it points to the following two bytes: `0x56 0x78`.
        *   `len -= 2;` Now `len = 2'.
1. **The second pass of the loop:**
* `sum += *addr++;`
        *   `*addr`:  Dereference `addr'.  'addr` points to `0x56 0x78`, which we consider to be a single 16-bit number `0x5678` (22136).
* `sum += 0x5678;`
        * `addr++': The pointer `addr` moves outside the data (there is nothing more to process).
        *   `len -= 2;` Now `len = 0'.
1. **The cycle is completed:** The loop stops because `len` is no longer greater than 1.

**The key point:**

In each pass of the loop, we take a *pair* of bytes (two bytes) and treat them as a single 16-bit number.  We add these 16-bit numbers, one by one, into the 'sum` variable.

If we had more data (for example, 8 bytes), the cycle would repeat more times, processing each pair of bytes (each 16-bit number) in sequence.
- How does the checksum function (and other similar checksum algorithms) handle large amounts of data by processing them "in pairs"? Let's look at this in more detail.

**Key idea: Divide and Conquer**

The basic principle is that the checksum algorithm does not require the entire amount of data to be loaded into memory at the same time. It works iteratively, processing data in small chunks (in our case, 2 bytes each) and accumulating the result. That's how it works.:

1. **Processing of small portions:**
    * The checksum function takes data in parts. In the loop `while(len > 1)`it processes data 2 bytes at a time.
    * For each 16-bit word (a pair of bytes) it performs the addition operation ('sum += *addr++;`).
    * 'addr++` moves the pointer to the next 16-bit word, preparing for processing the next pair of bytes.
2. **Accumulation of the result:**
* Each addition operation ('sum += *addr++;`) updates the value of the `sum` variable.
    * 'sum` acts as an accumulator, accumulating the results of processing each piece of data.
3. **Overflow handling:**
    * If the `sum` becomes too large (greater than 65535), an overflow occurs.
    * In the checksum function, overflow handling is implemented in the while (sum>>16) { ... } loop. This loop transfers the high-order bits of the `sum` (which go beyond 16 bits) back to the low-order bits, preserving the integrity of the information.
4. **Final checksum:**
* After processing all the data, the `sum` contains the final result, the checksum.
    * This checksum is a fingerprint of all the input data.

**Advantages of this approach:**

* **Memory saving:** There is no need to load the entire amount of data into memory at the same time. This is especially important for large files or network packets, where the amount of data can be very large.
* **Performance:** Addition and bitwise shift operations (which are used in checksum) are very fast operations that the processor can perform efficiently.
* **An iterative process:** The algorithm is performed iteratively, which makes it suitable for processing data arriving gradually (for example, data received over the network).

**Analogy:**

Imagine that you want to calculate the total amount of money in a large box of coins. You can:

1. Take a small handful of coins.
2. Calculate the sum of these coins.
3. Write this amount in a notebook (battery).
4. Take the next handful of coins.
5. Calculate the sum of these coins.
6. Add this amount to the amount in the notebook.
7. Repeat steps 4-6 until you have processed all the coins.
8. The total amount will be in the notebook.

**As applied to `checksum`:**

* Each handful of coins is 2 bytes of data (or a 16-bit word).
* The coin counter is a `sum'.
* Notepad is a 'sum` variable that stores the total amount.

**Example (more data):**

Suppose we want to calculate a checksum for a 10,000 byte file. The `checksum` function will be:

1. Take the first 2 bytes, calculate their sum, add to the `sum'.
2. Take the next 2 bytes, calculate their sum, add to the `sum'.
3. Repeat steps 1-2 until it has processed all 10,000 bytes.

After processing all the bytes, the `sum` will contain the checksum for the entire file.

**In conclusion:**

The checksum algorithm works "in pairs" (or in a different block size, depending on the implementation), but this does not mean that it processes only small amounts of data. Thanks to its iterative approach and battery usage, it can efficiently process data of any size, breaking it down into smaller, manageable chunks. This allows you to save memory and process data gradually.