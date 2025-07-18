### Thinking about data systems

We usually think of databases, queues, caches, and other tools as completely different categories. Although the database and the message queue have some superficial similarities (both store data for some time), their access models differ, resulting in different performance characteristics and, consequently, completely different implementations.

So why do we combine them under the general term "data systems"?

#### Blurring the boundaries between categories
In recent years, many new tools for data storage and processing have emerged. They are optimized for different usage scenarios and no longer fit into traditional categories. For example:
- Some data stores are used as message queues (for example, Redis).
- Some message queues have security guarantees, like databases (for example, Apache Kafka).

The boundaries between categories are becoming increasingly blurred.

#### Combining tools
Modern applications often have such complex or diverse requirements that no single tool can meet all their data processing and storage needs. Instead, the work is broken down into tasks that are efficiently performed by separate tools, and then these tools are combined using application code.

For example:
- If you have a caching layer (for example, Memcached) or a full-text search (for example, Elasticsearch) separated from the main database, then synchronization of these components usually falls on the application code.

When several tools are combined to provide a service, their implementation details are hidden behind the API. This creates a new, specialized data system made up of simpler, universal components. This system may provide certain guarantees (for example, consistency of data when updating the cache). In such a situation, the developer also becomes the architect of the data system.

#### Data system design issues
When designing a data system, many difficult questions arise.:
- How to ensure the correctness and integrity of data even in case of failures?
- How to maintain stable performance in case of degradation of a part of the system?
- How to scale up to the growing workload?
- What should be a good API for the service?

Many factors influence the design of the system: the team's experience, legacy dependencies, deadlines, the organization's risk tolerance, regulatory requirements, etc. However, there are three key aspects that are important for most systems:

1. **Reliability**  
   The system must work correctly (perform the necessary functions with the required performance) even in emergency situations (hardware/software failures, human errors).  

2. **Scalability**  
   The system must cope with the growth (data volume, traffic, complexity) in a reasonable way.  

3. **Maintainability**  
   Different people (developers, operators) should be able to work effectively with the system: maintain its current state and adapt it to new requirements.  

These terms are often used without a clear understanding of their meaning. Next, we'll explore in detail what they mean in practice, and in the following chapters we'll explore techniques, architectures, and algorithms to achieve them.

---

### Reliability

Intuitively, everyone understands what "reliability" means. For software, this usually implies:
- The application performs the functions expected by the user.
- It is resistant to user errors or unexpected usage patterns.
- Its performance is sufficient for target scenarios under the expected load.
- The system prevents unauthorized access and abuse.

If all this is "correct operation", then **reliability** can be defined as ** the ability to continue working correctly even when problems arise**.

#### Fault tolerance
The problems that may arise are called **failures** (faults), and systems capable of anticipating and handling them are **fault-tolerant** (fault-tolerant) or **resilient** (resilient).  

It is important to distinguish:
- **Fault** — deviation of one component from the specification.
- **Failure** — termination of the provision of the service by the system as a whole.  

It is impossible to completely eliminate failures, so they design mechanisms that do not allow them to lead to failures. In this book, we will explore such techniques.

#### Stability check
Paradoxically, in fault-tolerant systems, it is sometimes useful **to intentionally cause failures** (for example, to accidentally stop processes). This helps to identify weaknesses in error handling. An example is the Netflix Chaos Monkey. (No, this is not a paradox, it is a normal practice of a sane person. It's obvious)

Although it is usually better to tolerate failures than to prevent them, in some cases (for example, in security) prevention is critically important. However, this book focuses on failures that can be fixed.

### **Hardware failures**  

When we think about the causes of system failures, the first thing that comes to mind is **hardware failures**.  

- **Hard drives** fail.  
- **RAM** becomes faulty.  
- **The power grid** is shutting down.  
- Someone **accidentally pulls the cable** out of the network connector.  

Those who have worked with large data centers know that such things happen **all the time** when you have thousands of machines.  

📊 **Reliability statistics:**  
- The average operating time to failure (MTTF) for hard drives is **10-50 years**.  
- In a cluster of **10,000 disks**, on average **one disk dies every day**.  

#### **How to deal with it?**  
The first solution is **redundancy**.  
- **RAID** for disks.  
- **Dual power supplies** and CPU hot swap in servers.  
- **Backup generators** in data centers.  

✅ **Positive:**  
- Allows you to replace a broken component without shutting down the system.  
- Classical approach, well studied.  

❌ **Minuses:**  
- Does not protect against **total machine failure**.  
- In modern cloud systems (for example, AWS), virtual machines can **disappear without warning**.  

#### **Trend: the transition to software fault tolerance**  
As data volumes and computing requirements increase, **hardware redundancy is no longer sufficient**.  

🔧 **Decision:**  
- **Systems that are resistant to the loss of entire machines** (for example, distributed fault-tolerant architectures).  
- **Rolling upgrades** — software updates without stopping the service (gradually, node by node).  

---

### **Software errors**  

Hardware failures are usually **random and independent**, but **software errors** are **system problems** that can **paralyze the entire infrastructure**.  

#### **Examples:**  
🐞 **Critical bugs:**  
- **Leap second bug (2012)** — due to a bug in the Linux kernel, many servers crashed at the same time.  
- **Resource leaks** — the process "eats up" the CPU, memory or network.  
- **Cascading failures** — one mistake triggers a chain reaction.  

🔄 **Why are they difficult to predict?**  
Software errors often **sleep** for a long time until a rare combination of conditions works.  

#### **How to minimize the risks?**  
- **Thorough testing** (unit tests, integration tests).  
- **Process isolation** (so that one failure does not kill the entire system).  
- **Monitoring and alerts** (for example, checking that the number of incoming messages is equal to outgoing ones).  
- **Automatic restarts** (if the process crashes, we raise it again).  

---

### **The human factor**  

People design systems, set them up, and maintain them. And ** are wrong**.  

📉 **Statistics:**  
- **Configuration errors** are the main cause of downtime (more often than hardware failures!).  

#### **How to reduce the influence of the human factor?**  
✅ **Designing error-minimizing systems:**  
- Intuitive APIs and interfaces.  
- Protection against "stupid" actions (but without unnecessary restrictions).  

🛡 **Protective mechanisms:**  
- **Sandboxes** for testing changes.  
- **Automatic rollbacks of** configurations.  
- **Gradual deployment** (so that bugs affect only a small part of users).  

📊 **Monitoring and telemetry:**  
- **Performance metrics.**  
- **Error logging.**  
- **Alert systems.**  

🎓 **Training and processes:**  
- Clear instructions.  
- Training sessions for DevOps.  

---

### **How important is reliability?**  

🔹 **For critical systems (nuclear power plants, aviation)** — required.  
***For business applications** — mistakes = loss of money + reputational risks.  
🔹 **Even for "simple" services** (for example, photo hosting) — data loss = disaster for users.  

💡 **Conclusion:**  
Reliability can **be sacrificed** only consciously (for example, in MVP or low-budget projects). But you always need to ** understand the risks**.  

* **The best approach:**  
- **Prevent** what can be prevented.  
-**Provide recovery** where it is impossible to prevent.

---

### 🔧 **What is Disk RAID?**  
**RAID** (*Redundant Array of Independent Disks* — "redundant array of independent disks") is a technology that combines multiple hard drives into one system to improve **reliability** and/or **performance**.  

#### 🔹 **The main types of RAID:**  
1. **RAID 0 (Stripe)** – **speeds up work**, but ** does not protect against failures**.  
   - Data is split into blocks and written to multiple disks in parallel.  
   - ❌ **Minus:** If one disk fails, all data will be lost.  

2. **RAID 1 (Mirror)** – **a complete copy of the data on the second disk**.  
   - If one disk breaks, the data will remain on the second one.  
   - ✅ **Plus:** High reliability.  
   - ❌ **Minus:** requires twice as many disks (for example, 2 disks of 1 TB each = 1 TB of usable space).  

3. **RAID 5** – ** a balance of reliability and efficiency**.  
   - Data + checksums (*parity*) are distributed across all disks.  
   - If one disk fails, the information will be restored automatically.  
   - ✅**Optimal** for most servers.  

4. **RAID 6** – like RAID 5, but with **dual protection** (you can lose **2 disks** without data loss).  
   - Used in mission-critical systems.  

5. **RAID 10 (1+0)** – **mirror + alternation**.  
   - Disks are paired (RAID 1), and then these pairs work as RAID 0.  
   - ✅ **High speed + fault tolerance**.  
   - ❌**Expensive** (you need at least 4 disks).  

---

### ⚡ **Dual power supplies and hot-swappable CPU**  
#### 1. **Dual Power Supplies (Dual PSU)**  
- The server is connected **to two independent power sources**.  
- If one power supply unit breaks down or the electricity goes out on one line, the server will continue to operate from the second one.  
- 🔌 **Example:** In data centers, servers are often connected to two different UPS systems (and even to different substations).  

#### 2. **Hot-swap CPU**  
- In some servers, you can **replace the processor without shutting down the system**.  
- 💻 **How does it work?**
- The server has several CPU sockets.  
  - If one processor fails, the system automatically switches to another.  
  - The admin can remove the broken CPU and insert a new one **without downtime**.  

---

### ⚡ **Backup generators in data centers**  
Data centers consume **a huge amount of energy**, and even a short-term power outage can lead to data loss.  

#### 🔋 **How is the uninterruptible power supply provided?**  
1. **UPS** – batteries that provide energy for ** the first minutes** after disconnection.  
2. **Diesel generators** – turn on if the power is not restored in 10-30 seconds.  
   - They can work ** hours or even days** (until the fuel arrives).  
3. **Dual power lines** – Power is supplied from two independent substations.  

#### 🏢 **Example: how Google / AWS / Yandex.Cloud works**  
- Multiple redundancy levels: **UPS → Generators → Different power grids**.  
- Even if the lights are turned off in the city, the data center will continue to work.  

---

### 🎯 **Conclusion**  
- **RAID** protects against disk failures.  
- **Dual power supplies and hot-swappable CPUs** reduce server downtime.  
- **Generators in data centers** ensure that services do not crash even in the event of an accident in the power grid.  

💡 **Result:** all this is necessary for the system to work ** 24/7 without interruptions**! 🚀

