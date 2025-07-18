### 🚀 **Scalability**  

Even if the system is working reliably today, this does not mean that it will cope with the load tomorrow.  

**🔹 What is scalability?**  
This is the ability of the system ** to cope with increasing load** (for example, more users, more data, more requests).  

* **The myth:** "This system is scalable" is meaningless.  
✅ **Correctly:** "How will the system scale ** with such an increase in load **?"  

---

### 📊 **How can I describe the load?**  
The load can be measured using **load parameters** (load parameters). They depend on the system:  
- **RPS (requests per second)** — for the web server.  
- **The read/write ratio** is for the database.  
- **Number of active users** — for the chat.  
- **Cache hit rate** — for caching systems.  

#### 🐦 **Example: Twitter (2012)**  
Two key operations:  
1. **Tweet post** – 4.6K RPS (peak – 12K RPS).  
2. **Feed (home timeline)** – **300K RPS** (much more often than posts!).  

**The problem:**  
Not the volume of tweets, but **fan-out** — each tweet should get into the feed of all subscribers.  

---

### ⚖️ **Two approaches to scaling Twitter**  

#### **1. Approach: "Lazy" feed generation**  
- **How does it work?**
- The tweet is simply added to the general table.  
  - When a user opens the feed, the system searches for the tweets of everyone they follow and sorts them.  
  - The SQL query looks like this:
```sql
    SELECT tweets.*, users.* FROM tweets
    JOIN users ON tweets.sender_id = users.id
    JOIN follows ON follows.followee_id = users.id
    WHERE follows.follower_id = current_user
```
- **Positive:**
- Easy to record (only 1 insert per tweet).  
- **Minuses:**
- Reading the tape becomes **very difficult** (300K RPS!).  

#### **2. Approach: "Preliminary" tape generation**  
- **How does it work?**
- When a tweet is published, it ** is immediately added to the cache of each subscriber's feed**.  
  - Reading the feed is just downloading the finished list.  
- **Positive:**
- Tape reading ** is very fast** (already done).  
- **Minuses:**
- Recording becomes more difficult:
- On average, a tweet gets to **75 subscribers** → 4.6K tweets/s turn into **345K records/s**.  
    - **Celebrities** >30M followers → one tweet = **30M posts**!  

**The result:** Twitter initially used **1st approach**, but switched to **2nd** because reading the feed was bottleneck.  

![alt text](images\image.png)

---

### 🔄 **Hybrid approach (modern Twitter)**  
- **Regular users:** fan-out when publishing (approach 2).
-**Celebrities:** fan-out when reading (approach 1).  
- **Why?**
- Approach 2 is effective for 99% of users.  
  - But celebrity tweets **overburden** the system when fan-out.  

💡 **Conclusion:**  
Scalability is ** a balance between different strategies** for a specific load.  

---

### 📌 **Key thoughts**  
1. **The load varies** — it is important to choose the right parameters.  
2. **Reading vs Writing** — sometimes it is more profitable to complicate writing in order to speed up reading (and vice versa).  
3. **Flexibility** — combine approaches to make the system scale.  

***In the following chapters** we will analyze specific techniques: sharding, replication, caching, etc.

### 📊 **Performance and scalability: how to measure and improve?**  

#### 🔹 **Two key issues with increasing workload:**  
1. **How will the performance change** if the load increases, but the resources (CPU, RAM, network) remain the same?  
2. **How can I increase resources** to maintain productivity with increasing workload?  

---

### ⏱ **Performance Metrics**  
#### 1. **Throughput (Throughput)**  
- **For batch systems (Hadoop, Spark):**
- How many records are processed per second.  
  - How long does it take to process the entire dataset.  

#### 2. **Response Time**  
- **For online services:**
- The time between the client's request and receiving the response.  
  - **Important:** This is not only the processing time, but also network delays, queues, etc.  

📌 **The difference between Latency and Response Time:**  
- **Latency** is the waiting time for processing (for example, a queue).  
- **Response Time** = Latency + processing time + Network delays.  

---

### 📈 **Response time analysis**  
The response time **is not constant** — it is a **distribution of values**.  

#### 📉 **Example (Fig. 1-4):**  
- Most requests are fast.  
- But there are **"outliers"** (outliers) — slow queries.  

#### 📊 **How to measure correctly?**  
❌ **Average (mean)** is a bad indicator because it hides outliers.  
✅ **Percentiles** — better reflect the real user experience.  

- **p50 (median)** — 50% of requests are faster than this value.  
- **p95** — 95% faster queries.  
- **p99** — 99% of requests are faster.  
- **p999 ("tail")** — 99.9% of requests are faster.  

#### 🐌 **Why is the "tail" important (p99, p999)?**  
- **1% of slow requests** can ruin the UX for the most active users (for example, Amazon customers with large purchases often end up in p999).  
- **Deceleration effect:**
- +100 ms → -1% of sales (Amazon).  
  - +1 sec → -16% satisfaction (research).  

---

### 🚦 **Reasons for slow queries**  
1. **Queues (Queuing delays)**
- If the server is overloaded, new requests are waiting for the old ones to complete (**head-of-line blocking**).  
2. **Parallel calls**
- If the client's request depends on multiple services, it waits **for the slowest** (tail latency amplification).  

---

### ⚙ **How to handle the workload?**  
#### 🔹 **Scaling strategies:**  
1. **Vertical (Scale Up)**
- Increase server power (more CPU, RAM).  
   - **Positive:** Simplicity.  
   - **Cons:** Expensive, there are limits.  

2. **Horizontal (Scale Out)**
- Add more servers, distribute the load.  
   - **Positive:** Cheaper, you can scale almost infinitely.  
   - **Cons:** complexity (especially for stateful systems, such as databases).  

3. **Elastic Scaling**
- The system adds resources by itself as the load increases.  
   - **Positive:** Flexibility.  
   - **Cons:** the complexity of the setup, the risks of unexpected behavior.  

#### 📌 **Important:**  
- **There is no universal solution** — Architecture depends on load (reads vs writes, data volume, delays, etc.).
- **Early stages of the project:** It is better to optimize for development speed than for hypothetical scale.  

---

### 🧩 **Practical advice**  
1. **Monitoring:**
- Keep an eye on **p95/p99**, not just the average.  
   - Use histograms (HdrHistogram, t-digest).  

2. **Tail optimization:**
- Parallelize calls.  
   - Reduce queues (for example, by configuring thread pools).  

3. **Testing:**
- Load the system **regardless of response time** (so as not to hide queues).  

🔜 **Later in the book:** how to design distributed systems for different scenarios.