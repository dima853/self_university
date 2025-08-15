# 🌟 **SSTables and LSM-trees: Superstructures for data storage** 🌟

## 📜 **From unordered logs to SSTables**
Previously, we had **simple logs** (key-value in write order). Now we go one step further:
🔹 **SSTable (Sorted String Table)** is a **sorted by key** log.
🔹 Each key occurs **only once** in a segment (duplicates are removed during merging).

### 🎯 **Advantages of SSTables over hash indexes**
1. **Efficient merging of segments** (like in mergesort)
- Compare keys from several files and write the minimum.
- If the key is repeated, take the value from the **newest** segment.

2. **Faster search without a full index**
- A **sparse** index in memory is enough (e.g. only every 10th key).
- Example:
- Looking for `handiwork`, but only know `handbag` (offset 1000) and `handsome` (offset 2000).
- Go to `handbag` and linearly scan to `handiwork`.

3. **Data compression**
- Keys and values are grouped into **blocks** and compressed before writing.
- Space savings + less I/O.

---

## 🌳 **How to build an SSTable?**
The data comes in **random** order, but we need to keep it **sorted**.

### 🔥 **LSM-tree (Log-Structured Merge-Tree) algorithm**
1. **Memtable** — **red-black tree** (or similar) in memory.
- All records first go here.
- Supports sorting by key.

2. **Flush to disk**
- When memtable reaches **several megabytes**, it is written to disk as **SSTable**.
- New records go to **new memtable**.

3. **Read**
- First we check **memtable**, then **the most recent SSTable**, then the old ones.

4. **Background compaction**
- Segments are periodically merged, duplicates and "tombstones" (deleted keys) are removed.

### ⚠️ **Crash protection**
- Before writing to memtable, data is written to **write-ahead log (WAL)**.
- After flushing SSTable, WAL can be deleted.

---

## 🏎️ **LSM tree optimizations**

### 🔍 **Bloom filters**
- **Problem:** Searching for non-existent keys requires checking **all** levels.
- **Solution:** Bloom filter quickly answers "maybe the key exists" or "definitely not".

### 🔄 **Compactization strategies**
1. **Size-tiered** (like in HBase)
- Small SSTables are merged into large ones.
2. **Leveled** (like in RocksDB)
- Data is divided into levels, where each level is **10 times larger** than the previous one.
- Compaction occurs **incrementally**.

---

## 🌍 **Where is it used?**
- **RocksDB, LevelDB** are built-in engines for key-value.
- **Cassandra, HBase** are distributed databases.
- **Lucene** (Elasticsearch, Solr) are search indexes.

---

## 💡 **Conclusion**
✅ **SSTables** are **sorted** segments that speed up search and merging.
✅ **LSM trees** combine **memtable + SSTables + compaction**.
✅ **Optimizations** (Bloom filters, leveled compaction) make them even faster.

