# 🐬 **MySQL Latency Analysis: A Complete Guide**

Latency (response time) is a critical metric for MySQL performance. Let's figure out how to analyze and optimize it.

## 🔍 **1. The main sources of latency in MySQL**

## 1.1. Delay levels (expected values)**
| Component        | Typical delay | Notes                      |
| ---------------- | ------------- | -------------------------- |
| **InnoDB Cache** | 0.1-1 ms      | Buffer pool in RAM         |
| **SSD drive**    | 0.5-5 ms      | For random reads           |
| **HDD**          | 5-15 ms       | Depends on the positioning |
| **Network**      | 1-100 ms      | Local vs Remote database   |
| **Locks**        | 0.1 ms - ∞    | Depends on the competition |

### ⚠️ **1.2. Alarms**
- Requests > 100 ms — require urgent analysis
- Bursts > 1 sec — critical problem

## 🛠️ **2. Tools for analysis**

### **2.1. Built-in MySQL metrics**
```sql
-- Query execution time (activate in the config):
SET GLOBAL slow_query_log = 'ON';
SET GLOBAL long_query_time = 0.1; -- log requests >100ms

-- View slow queries:
SELECT * FROM mysql.slow_log;
```

### **2.2. Performance Schema**
```sql
-- Top 10 slowest queries:
SELECT digest_text, avg_timer_wait/1e9 as avg_ms 
FROM performance_schema.events_statements_summary_by_digest 
ORDER BY avg_timer_wait DESC LIMIT 10;
```

### **2.3. InnoDB Metrics**
```sql
SHOW ENGINE INNODB STATUS\G
```
Pay attention to:
- **SEMAPHORES**: Waiting for locks
- **TRANSACTIONS**: Long transactions
- **BUFFER POOL**: Cache efficiency

## 📉 **3. Typical reasons for high latency**

### **3.1. Disk problems**
```sql
-- Checking disk operations:
SHOW GLOBAL STATUS LIKE 'Innodb%read%';
``
**Solution:**
- Increase `innodb_buffer_pool_size' (up to 80% RAM)
- For HDD: decrease `innodb_io_capacity`

### **3.2. Blockages**
```sql
-- Current locks:
SELECT * FROM sys.innodb_lock_waits;
``
**Solution:**
- Optimize transactions (less isolation)
- Add indexes to reduce the blocking time

### **3.3. Bad queries**
```sql
-- Full scan queries:
SELECT * FROM sys.statements_with_full_table_scans;
``
**Solution:**
- Add indexes
- Rewrite queries

## 🚀 **4. Practical optimizations**

### **4.1. my.cnf settings**
``ini
[mysqld]
innodb_flush_log_at_trx_commit = 2 # To reduce write
delays innodb_read_io_threads = 16 # For SSD/NVMe
innodb_write_io_threads = 16
```

### **4.2. Optimization of the scheme**
```sql
-- Switching from TEXT to VARCHAR:
ALTER TABLE posts MODIFY content VARCHAR(5000);

-- Adding a composite index:
ALTER TABLE orders ADD INDEX idx_status_created (status, created_at);
``

### **4.3. Real-time monitoring**
```bash
# pt-query-digest utility:
pt-query-digest /var/log/mysql/mysql-slow.log

# Monitoring with Percona:
sudo apt install percona-toolkit
``

## 📊 **5. Grafana analysis (dashboard example)**

![MySQL Latency Dashboard](https://i.imgur.com/JQ4Xz1l.png )
*Metrics for monitoring:*
- `mysql_query_response_time_95percentile`
- `innodb_row_lock_time_avg`
- `disk_read_latency`

## 💡 **6. Advanced techniques**

**A/B testing of changes:**
```sql
-- Enabling the optimizer for a specific session:
SET SESSION optimizer_switch='index_merge=off';

-- EXPLAIN before/after comparison:
EXPLAIN FORMAT=JSON SELECT * FROM large_table WHERE ...;
``

**Time series analysis:**
```sql
-- Run time histogram:
SELECT
FLOOR(query_time_ms/10)*10 as bucket,
  COUNT(*) 
FROM slow_log 
GROUP BY bucket;
```

## 🔧 **When should I do what?**
| Symptom                   | First actions                    |
| ------------------------- | -------------------------------- |
| High `CPU` with low `IO`  | Slow query analysis              |
| The growth of `lock_time` | Checking long transactions       |
| Sudden spikes in latency  | Check network/disks for overload |