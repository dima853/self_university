## 🚀 `write()' - briefly

**WHAT DOES:**
- Throws data into RAM (core cache)
- Returns success immediately (does not wait for the disk)

**WHY IS THIS SO:**
- RAM is 100,000 times faster than disk
- Otherwise, the system hangs on every write()

**WHAT IS DANGEROUS:**
- In case of a power failure, the data in RAM burns out
- write() lied about "success"

**HOW TO GUARANTEE A RECORD:**
```c
write(fd, data, size); // fast
fsync(fd); // waiting for a real write to disk 🛡️
```

**WHEN TO USE:**
- `write()` - for logs, cache (it's not a pity to lose)
- `write() + fsync()` - for money, transactions, configs

**THE ESSENCE:** The choice between speed and reliability