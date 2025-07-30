# 🌟 **MapReduce Querying** 🌟  

## 📚 **What is MapReduce?**  
MapReduce is a programming model for processing large amounts of data on multiple machines, popularized by Google [33]. Some NoSQL repositories, including **MongoDB** and **CouchDB**, support a limited version of MapReduce for performing **read-only queries** on multiple documents.  

🔹 **MapReduce** is **not**a declarative query language and **not** a fully imperative API, but something in between: query logic is expressed by **code snippets** that are called by the processing framework.  

🔹 Is based on the functions:
- **`map`** (also known as `collect`)
-**`reduce`** (also `fold` or `inject`),
which exist in many **functional programming languages**.  

---

## 🦈 **A real-life example: Counting sharks**  
Imagine that you are a marine biologist and you record every observation of animals in the ocean in a database. Now you need to **create a report** on how many sharks you have seen **monthly**.  

### **1️⃣ SQL version (PostgreSQL)**  
```sql
SELECT 
    date_trunc('month', observation_timestamp) AS observation_month,
    sum(num_animals) AS total_animals
FROM observations
WHERE family = 'Sharks'
GROUP BY observation_month;
```
🔹 `date_trunc('month', timestamp)` rounds the date to the beginning of the month.  

### **2️⃣ MongoDB MapReduce version**  
```javascript
db.observations.mapReduce(
    function map() {
        var year = this.observationTimestamp.getFullYear();
        var month = this.observationTimestamp.getMonth() + 1;
        emit(year + "-" + month, this.numAnimals);
    },
    function reduce(key, values) {
        return Array.sum(values);
    },
    {
        query: { family: "Sharks" },
        out: "monthlySharkReport"
    }
);
```
🔹 **How does it work?**  
- `map()' is called for each document matching the `query` condition.  
- `emit()` generates **key-value** pairs (for example, `"1995-12", 3`).  
- `reduce()` summarizes the values for each key.  

### **📌 Sample data**  
```json
[
    {
        "observationTimestamp": Date.parse("Mon, 25 Dec 1995 12:34:56 GMT"),
        "family": "Sharks",
        "species": "Carcharodon carcharias",
        "numAnimals": 3
    },
    {
        "observationTimestamp": Date.parse("Tue, 12 Dec 1995 16:17:18 GMT"),
        "family": "Sharks",
        "species": "Carcharias taurus",
        "numAnimals": 4
    }
]
```
, After processing , we get:  
- `map()` → `emit("1995-12", 3)` and `emit("1995-12", 4)`  
- `reduce("1995-12", [3, 4])` → `7`  

---

## ⚡ **Limitations and Alternatives**  
* **The `map` and `reduce` functions must be clean** (without side effects and additional queries).  
🔹 **Difficulty writing** two consistent functions → MongoDB 2.2 added **declarative `aggregation pipeline`**!  

### **🎯 Aggregation pipeline in MongoDB**  
```javascript
db.observations.aggregate([
    { $match: { family: "Sharks" } },
    { $group: {
        _id: {
            year: { $year: "$observationTimestamp" },
            month: { $month: "$observationTimestamp" }
        },
        totalAnimals: { $sum: "$numAnimals" }
    } }
]);
```
🔹 **Conclusion:** NoSQL sometimes "reinvents SQL" in a different way.  

---

# 🌐 **Graph data models**  
If your data ** has a lot of "many-to-many" relationships**, then the relational model may be inconvenient → the graph model** is better suited!  

## 🏷 **What is a graph?**  
🔹 **Vertices (nodes)** — entities (people, places, events).  
🔹 **Edges (connections)** — the relationship between them.  

### **🌍 Examples of graphs**  
- **Social networks** (people → friends)  
- **Web graph** (pages → links)  
- **Road networks** (intersections → roads)  

---

## 📊 **Property Graph Model**  
🔹 **Each vertex has:**  
- Unique ID  
- Outgoing and incoming edges  
- Properties (key-value)  

🔹 **Each edge has:**  
- Unique ID  
- The starting (`tail_vertex') and ending (`head_vertex') vertices  
- A label (`label`) of the type of connection  
- Properties (key-value)  

### **🗃 Example of storage in SQL**  
```sql
CREATE TABLE vertices (
    vertex_id integer PRIMARY KEY,
    properties json
);

CREATE TABLE edges (
    edge_id integer PRIMARY KEY,
    tail_vertex integer REFERENCES vertices (vertex_id),
    head_vertex integer REFERENCES vertices (vertex_id),
    label text,
    properties json
);

CREATE INDEX edges_tails ON edges (tail_vertex);
CREATE INDEX edges_heads ON edges (head_vertex);
```
🔹 **Why is it convenient?**  
1. Any vertex can be connected to any other.  
2. It is possible to efficiently find **incoming/outgoing edges**.  
3. Different types of links are stored in the same structure.  

---

## 🧩 **Sample data (Fig. 2-5)**  
- **Lucy** from Idaho and **Alain** from Beaune (France) are married and live in London.  
- Different levels of detail:
- Lucy's birthplace is **State** (Idaho).  
  - Current location is **city** (London).  

***Graphs scale perfectly** for new data types!  

---

# 🎯 **Conclusion**  
- **MapReduce** is a powerful but complex tool for distributed processing.  
- **Graph models** are ideal for data with complex relationships.  
- **NoSQL is an "SQL killer"**, and an alternative for specific tasks.  

💡 **Which model should I choose?** Depends on the data structure and queries! 🚀

# 🌟 **Complete guide to MapReduce in MySQL and Spring Data JPA `@Query`** 🌟

## 📌 **Part 1: MapReduce in MySQL**

MySQL does not have built-in MapReduce support, but we can emulate its behavior using SQL queries.

### 🔹 **Example 1: Calculating the number of orders by month (analogous to MapReduce)**

```sql
-- MAP stage: grouping data by month
SELECT 
    DATE_FORMAT(order_date, '%Y-%m') AS month,
    COUNT(*) AS order_count
FROM orders
GROUP BY DATE_FORMAT(order_date, '%Y-%m');

-- REDUCE stage: aggregation of results (already running in GROUP BY)
``

### 🔹 **Example 2: Total sales by product category**

```sql
-- MAP: breaking down the data into categories
-- REDUCE: sum up the cost
SELECT 
    p.category,
    SUM(oi.quantity * oi.price) AS total_sales
FROM order_items oi
JOIN products p ON oi.product_id = p.id
GROUP BY p.category;
```

### 🔹 **Example 3: Complex MapReduce - average check by day of the week**

```sql
SELECT 
    DAYNAME(order_date) AS day_of_week,
    AVG(total_amount) AS avg_order_value,
    COUNT(*) AS order_count
FROM (
-- MAP stage: data preparation
    SELECT 
        order_date,
        SUM(quantity * price) AS total_amount
    FROM order_items
    GROUP BY order_id, order_date
) AS order_totals
GROUP BY DAYNAME(order_date)
ORDER BY FIELD(day_of_week, 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday');
```

## 📌 **Part 2: MapReduce in Spring Data JPA with `@Query`**

### 🔹 **Example 1: Basic MapReduce-like query**

```java
public interface OrderRepository extends JpaRepository<Order, Long> {
    
    @Query("SELECT new map(MONTH(o.orderDate) as month, COUNT(o) as count) " +
           "FROM Order o " +
           "GROUP BY MONTH(o.orderDate)")
    List<Map<String, Object>> countOrdersByMonth();
}
```

### 🔹 **Example 2: Complex aggregation with DTO**

``java
// DTO for results
of public record SalesByCategoryDTO(String category, Double TotalSales) {}

// Repository
public interface OrderItemRepository extends JpaRepository<OrderItem, Long> {
    
    @Query("SELECT new com.example.dto.SalesByCategoryDTO(p.category, SUM(oi.quantity * oi.price)) " +
           "FROM OrderItem oi JOIN oi.product p " +
           "GROUP BY p.category")
    List<SalesByCategoryDTO> getSalesByCategory();
}
```

### 🔹 **Example 3: Multi-stage aggregation (analogous to MapReduce)**

```java
public interface UserActivityRepository extends JpaRepository<UserActivity, Long> {

    // Stage 1: MAP - collecting raw data
    @Query("SELECT new map(FUNCTION('DATE_FORMAT', a.activityDate, '%Y-%m-%d') as date, " +
           "a.user.id as userId, COUNT(a) as activityCount) " +
           "FROM UserActivity a " +
           "WHERE a.activityDate BETWEEN :startDate AND :endDate " +
           "GROUP BY FUNCTION('DATE_FORMAT', a.activityDate, '%Y-%m-%d'), a.user.id")
    List<Map<String, Object>> mapUserActivities(@Param("startDate") LocalDate startDate,
                                               @Param("endDate") LocalDate endDate);

    // Stage 2: REDUCE - we aggregate the results (performed in the service)
    default Map<String, Double> reduceUserActivities(LocalDate startDate, LocalDate endDate) {
        List<Map<String, Object>> mappedData = mapUserActivities(startDate, endDate);
        
        return mappedData.stream()
            .collect(Collectors.groupingBy(
                data -> (String) data.get("date"),
                Collectors.averagingInt(data -> (Integer) data.get("activityCount"))
            ));
    }
}
```

### 🔹 **Example 4: Native query with JSON aggregation (MySQL 5.7+)**

```java
public interface ProductRepository extends JpaRepository<Product, Long> {

    @Query(value = """
        SELECT 
            category,
            JSON_OBJECT(
                'totalProducts', COUNT(*),
                'averagePrice', AVG(price),
                'maxPrice', MAX(price),
                'minPrice', MIN(price)
            ) AS stats
        FROM products
        GROUP BY category
        """, nativeQuery = true)
    List<Map<String, Object>> getProductStatsByCategory();
}
```

## 📌 **Part 3: Custom MapReduce implementations in Spring**

### 🔹 **Example 1: Implementing MapReduce using the Stream API**

```java
@Service
public class OrderAnalysisService {

    @Autowired
    private OrderRepository orderRepository;

    public Map<String, Double> averageOrderValueByCity() {
// MAP stage: we receive all orders with cities
        List<Order> orders = orderRepository.findAllWithCity();
        
        // REDUCE stage: group and calculate the average
        return orders.stream()
            .collect(Collectors.groupingBy(
                Order::getCity,
                Collectors.averagingDouble(Order::getTotalAmount)
            ));
    }
}
```

### 🔹 **Example 2: Parallel processing with ForkJoinPool**

```java
public class ParallelMapReduceService {

    public Map<String, Long> wordCount(List<String> documents) {
        // MAP stage: parallel document processing
        Map<String, Long> wordCounts = documents.parallelStream()
            .flatMap(document -> Arrays.stream(document.split("\\s+")))
            .map(word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase())
            .filter(word -> !word.isEmpty())
            .collect(Collectors.groupingByConcurrent(
                Function.identity(),
                Collectors.counting()
            ));
        
        return wordCounts;
    }
}
```

## 📌 **Part 4: Optimization of MapReduce queries**

### 🔹 **Tip 1: Use indexes for GROUP BY fields**
```sql
CREATE INDEX idx_order_date ON orders(order_date);
CREATE INDEX idx_product_category ON products(category);
```

### 🔹 **Tip 2: Apply batch processing to big data**

```java
@Repository
public interface BigDataRepository extends JpaRepository<BigDataEntity, Long> {

    @Query(value = "SELECT * FROM big_data WHERE id > :lastId ORDER BY id LIMIT :batchSize", 
           nativeQuery = true)
    List<BigDataEntity> findBatch(@Param("lastId") Long lastId, @Param("batchSize") int batchSize);
}

@Service
public class BatchMapReduceService {

    @Autowired
    private BigDataRepository repository;

    public void processBigData() {
        Long lastId = 0L;
        int batchSize = 1000;
        
        while (true) {
            List<BigDataEntity> batch = repository.findBatch(lastId, batchSize);
            if (batch.isEmpty()) break;
            
            // Batch processing (MAP stage)
processBatch(batch);
            
            lastId = batch.get(batch.size() - 1).getId();
        }
    }
}
```

## 🎯 **Conclusion: when should I use what?**

1. **For simple aggregation** - use standard GROUP BY queries
2. **For complex business logic** - use a combination of @Query and Stream API
3. **For big data processing** - implement batch processing
4. **For distributed computing** - consider Apache Spark or Hadoop