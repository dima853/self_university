# **MyISAM engine in MySQL**

## **1. General Characteristics of MyISAM**
MyISAM is a traditional MySQL engine that was the default engine until version 5.5. Main Features:
- **No transactions** — no ACID support
- **Tabular locks** (not string locks)
- **No crash-safety** — data may be corrupted by crashes
- **Support for compression** and full-text indexes

## **2. Data storage structure**
MyISAM stores each table in two files:
- `.MYD` — data file
- `.MYI` — index file

**String formats**:
- **FIXED** — fixed length (for CHAR, INTEGER)
- **DYNAMIC** — variable length (for VARCHAR, BLOB/TEXT)
- **COMPRESSED** — compressed tables (read-only)

## **3. Key Features**

### **3.1 Locks and Concurrency**
- **Shared locks** for SELECT operations
- **Exclusive locks** for INSERT/UPDATE/DELETE
- **Competitive inserts**: INSERT can run in parallel with SELECT

### **3.2 Data Recovery**
```sql
-- Checking the table
CHECK TABLE mytable;

-- Restoring the table
REPAIR TABLE mytable;
```
Or via the command line:
```bash
myisamchk --check /path/to/mytable
myisamchk --recover /path/to/mytable
```

### **3.3 Indexing**
- Indexes for the first 500 characters of BLOB/TEXT
- Support for **fulltext search** (FULLTEXT)
- Delayed index entry (DELAY_KEY_WRITE)

## **4. MyISAM Performance**
**Advantages**:
- Less data volume compared to InnoDB
- Quick SELECT on immutable data
- Efficient use of disk space

**Restrictions**:
- Scaling issues due to table-level locks
- No crash-safety — requires manual recovery
- Poor performance under mixed load (read/write)

## **5. Compressed MyISAM Tables**
```bash
myisampack mytable
```
**Features of compressed tables**:
- Read-only
- Save disk space (up to 60-70%)
- Reducing the number of I/O operations
- Index support

## **6. Comparison with InnoDB**

| Feature of       | MyISAM        | InnoDB                      |
| ---------------- | ------------- | --------------------------- |
| Transactions     | ❌ No          | ✅ Supports                  |
| Locks            | Tabular       | String                      |
| Crash-safety     | ❌ No          | ✅ Yes                       |
| FULLTEXT indexes | ✅ Are         | ✅ (Starting with MySQL 5.6) |
| Compression      | ✅ (Read only) | ✅ (Transport)               |
| Optimal load     | Read-heavy    | Read/Write mixed            |

## **7. Alternative MySQL engines**

### **7.1 Archive**
- INSERT/SELECT only
- Data compression (zlib)
- No indexes (up to version 5.1)
- Suitable for logs

### **7.2 Memory (HEAP)**
- Data in RAM
- Support for HASH indexes
- Disappear when the server is restarted
- Restrictions: no TEXT/BLOB, fixed VARCHAR length

### **7.3 CSV**
- Storing data in CSV files
- There are no indexes
- Useful for data exchange

### **7.4 Federated**
- Access to tables on remote servers
- A lot of performance issues
- Replaced with FederatedX in MariaDB

### **7.5 Merge**
- Combining multiple MyISAM tables
- Outdated in favor of partitioning

## **8. When to use MyISAM**
1. **Logging** — data is only being added
2. **Read-only data** — reference books, archived data
3. **Full-text search** (in versions up to 5.6)
4. **Temporary tables** (but it is better to use Memory)

**Important**: In new versions of MySQL, it is recommended to use InnoDB as the default engine. MyISAM is only suitable for special occasions.