# 🔄 **Does history repeat itself? Document databases vs. Hierarchical models of the past**  

> *"Everything new is well—forgotten old."* 🕰️  

## 🏛️ **Historical context: IMS and the Hierarchical Model (1960s)**  
✔ **IBM IMS** is one of the first business databases (it was even used in the Apollo program!).  
 The data was stored as **trees** (very similar to JSON in MongoDB!).  
✔ **Problems**:  
   - There is no support for many-to-many relationships.  
   - No JOINS → duplication or manual link resolution.  

> *"What was fought in the 1970s returned in the 2010s with NoSQL!"* 🔄  

---  

## ⚔️ **The Great Debate of the 1970s: Relational vs. The Network Model (CODASYL)**  

### 📌 **The Network Model (CODASYL)**  
✔ Data is like a graph with pointers (not tables).  
✔ Access to records only through **"access paths"** (access paths).  
✔ **Problems**:  
   - The query code is complex and fragile.  
   - Changing the data structure = rewriting the code.  

```plaintext
Example:
User → (points to) → Company → (indicates) → Region  
```  

### 📌 **Relational Model (SQL)**  
✔ Data — **flat tables** without hidden structures.  
✔**Requests** are declarative (`SELECT * WHERE ...`).  
✔**Query optimizer** chooses the best path by itself.  

> *"The relational model won because it gave **flexibility** and **simplicity**!"* 🏆  

---  

## 🔍 **Comparison with modern document databases**  

| **Aspect**      | **The Network Model (CODASYL)** | **Document Databases (MongoDB)** | **Relational Databases (SQL)** |
| --------------- | ------------------------------- | -------------------------------- | ------------------------------ |
| **Connections** | Pointers (manual operation)     | Document references              | FOREIGN KEY + JOIN             |
| **Flexibility** | Difficult to change             | Flexible circuit                 | Rigid circuit                  |
| **Requests**    | Imperative (code)               | Semi-declarative                 | Declarative (SQL)              |

---  

## 💡 **Key findings**  
1. **Document databases are "new" hierarchical databases**, but with JSON instead of the ancient formats.  
2. **The problems are the same**:
- Convenient for **trees** (for example, a resume).  
   - Inconvenient for ** graphs** (social networks, recommendations).  
3. **Relational databases won** in the 1980s because:  
   - Query Optimizer **saved developers** from manual control of access paths.  
   - JOINS are **universal** for any connections.  

---  

## 🚀 **What's next?**  
Current trends:
- **Hybrid approaches** (for example, PostgreSQL with JSON + relational tables).  
- **Graph databases** (Neo4j) for complex relationships.  
- **NewSQL** (CockroachDB) is an attempt to combine the scalability of NoSQL and ACID SQL.  

> *"Choosing a database is not a religion. Use ** the best of all worlds**!"* 🌍  

