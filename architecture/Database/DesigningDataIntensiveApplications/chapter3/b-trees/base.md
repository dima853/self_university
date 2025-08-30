# 🌳 **B-Trees: Data Structures That Keep Your Database Running** 🌳  

## 🏗️ **The simplest database on Bash**  

Consider **the simplest database** implemented with just two functions in Bash:  

```bash
#!/bin/bash

db_set() {
echo "$1,$2" >> database # Just adding the key and value to the file }
}

db_get() {
grep "^$1," database | sed -e "s/^$1,//" | tail -n 1 # Searching for the last key value
}
```  

### 🎯 **How does it work?**  
- **`db_set`** saves the key and value to the `database` file.  
- **`db_get`** finds the last value by the key.  

**Usage Example:**  
```bash
$ db_set 123456 '{"name":"London","attractions":["Big Ben","London Eye"]}'  
$ db_set 42 '{"name":"San Francisco","attractions":["Golden Gate Bridge"]}'  
$ db_get 42  
{"name":"San Francisco","attractions":["Golden Gate Bridge"]}  
```  

### 📜 **Storage format**  
The `database` file looks like this:  
```
123456,{"name":"London","attractions":["Big Ben","London Eye"]}  
42,{"name":"San Francisco","attractions":["Golden Gate Bridge"]}  
42,{"name":"San Francisco","attractions":["Exploratorium"]}  
```  

🔹 **Updating the key** simply adds a new entry to the end of the file.  
🔹**Reading ('db_get')** returns **the last** value.  

---

## ⚡ **Performance issues**  
- **`db_set`** works fast because writing to the end of the file is **O(1)**.  
- **`db_get`** is **slow** (O(n)) because it has to scan the entire file!  

### 🚀 **Solution: Indexes!**  
To speed up the search, you need **an additional data structure** — **index**.  

📌 **The index** is metadata that helps you find data quickly.  
📌 **Positive:** Speeds up reading.  
📌 **Minuses:** slows down the recording (since the index also needs to be updated).  

---

## 🧩 **Hash indexes (as in Bitcask)**  

### 🔍 **How does it work?**  
1. A **hash table** is stored in memory, where:
- **Key** → **The offset in the file is ** (where the value lies).  
2. When adding data:
- Write to the end of the file.  
   - Updating the hash table.  
3. When reading:
- We look at the offset in the hash table.  
   - Go to the right place in the file and read the value.  

**Example (pseudocode):**  
```java
Map<String, Long> hashIndex = new HashMap<>(); // Key → Offset in the file

void dbSet(String key, String value) {
    long offset = appendToFile(key, value); // Write to the end of the file
    hashIndex.put(key, offset); // Updating the index
}

String dbGet(String key) {
long offset = hashIndex.get(key); // Finding the offset
    return readFromFile(offset); // Reading the value
}
``  

### 🔄 **Compactification and merging of segments**  
Over time, the file gets too big. Decision:  
1. Split the file into **segments** of a fixed size.  
2. **Compactification** — delete the old values, leaving only the latest ones.  
3. **Merge** — merge several segments into one.  

**A real-life example:**  
Imagine that you have a transaction log in a bank. It gets huge over time, but you only need **up-to-date balances**, not the entire transaction history.  

---

## ⚖️ **Pros and cons of hash indexes**  

### ✅ **Advantages**  
- **Quick recordings** (appended to the end of the file).  
- **Ease of implementation**.  
- **Well suited for frequent updates** (for example, counters).  

### ❌ **Disadvantages**  
- **The index must fit in RAM** (a problem for big data).  
- **Is ineffective for range queries** (for example, "find all keys from A to B").  

---

## 🌟 **What's next? B-trees!**  
Hash indexes are good, but **B-trees** solve their main problems.:  
 Suitable for **big data** (the index is stored on disk).  
, Supports **range queries**.  

We'll talk about them next time! 🚀  

---

### 💡 **Conclusion**  
1. **Log (append-only file)** is the basis of many DBMS.  
2. **Hash indexes** speed up the search, but require memory.  
3. **Compactification and merging** help to combat fragmentation.