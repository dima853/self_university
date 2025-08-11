**Graph data models: Properties and Queries**  

> *"If your data looks like a web of connections, you need a graph!"* 🕷️  

## 📌 **What is a property graph?**  
Each vertex (node) contains:
✔ **Unique identifier**  
✔ **Incoming and outgoing edges** (connections)
✔ **Properties** (key-value, for example `name: "Lucy"`)  

Each edge contains:
✔ **Unique identifier**  
✔ **Starting** (tail) and **ending** (head) vertices  
✔**Communication type** (for example, `BORN_IN`)
✔ **Properties** (for example, `since: 1990`)  

### Schema example in SQL (PostgreSQL)  
```sql
CREATE TABLE vertices (
    vertex_id SERIAL PRIMARY KEY,
    properties JSONB  -- {name: "Lucy", type: "Person"}
);

CREATE TABLE edges (
    edge_id SERIAL PRIMARY KEY,
    tail_vertex INT REFERENCES vertices(vertex_id), -- From where
    head_vertex INT REFERENCES vertices(vertex_id), -- To
    label TEXT, -- Link type (BORN_IN, LIVES_IN)
properties JSONB -- {since: 1990}
);

-- Indexes for quick link search
CREATE INDEX edges_tails ON edges(tail_vertex);
CREATE INDEX edges_heads ON edges(head_vertex);
```

---  

## 🔍 **Why are graphs powerful?**  
***Flexible connections**: Any vertex can be connected to any other.  
***Efficient traversal**: You can quickly find all incoming/outgoing edges.  
***Heterogeneous data**: You can store people, places, and events in one column.  

### Example data (Figure 2-5 from the book):
```
(Lucy) -[BORN_IN]-> (Idaho) -[WITHIN]-> (USA) -[WITHIN]-> (North America)
(Lucy) -[LIVES_IN]-> (Europe)
```

---  

## 🛠️ **The Cypher Query Language (Neo4j)**  
Example 2-3: Creating Data  
```cypher
CREATE
  (NAmerica:Location {name:'North America', type:'continent'}),
  (USA:Location {name:'United States', type:'country'}),
  (Idaho:Location {name:'Idaho', type:'state'}),
  (Lucy:Person {name:'Lucy'}),
  (Idaho) -[:WITHIN]-> (USA) -[:WITHIN]-> (NAmerica),
  (Lucy) -[:BORN_IN]-> (Idaho)
```

Example 2-4: Search for people who moved from the USA to Europe  
```cypher
MATCH
  (person) -[:BORN_IN]-> () -[:WITHIN*0..]-> (us:Location {name:'United States'}),
(person) -[:LIVES_IN]-> () -[:WITHIN*0..]-> (eu:Location {name:'Europe'})
RETURN person.name
```
**`How does it work?**  
1. Find everyone who was born (`BORN_IN`) anywhere in the United States (`WITHIN*0..` — any nesting depth).  
2. And who currently lives (`LIVES_IN`) anywhere inside Europe.  
3. Return their names.  

---  

# Graph queries in SQL (recursive CTE)**  
Example 2-5: The same query in SQL (PostgreSQL)  
```sql
WITH RECURSIVE
  in_usa(vertex_id) AS (
    SELECT vertex_id FROM vertices WHERE properties->>'name' = 'United States'
    UNION
    SELECT edges.tail_vertex FROM edges
    JOIN in_usa ON edges.head_vertex = in_usa.vertex_id
    WHERE edges.label = 'within'
  ),
  in_europe(vertex_id) AS (...),
  born_in_usa(vertex_id) AS (...)
SELECT vertices.properties->>'name'
FROM vertices
JOIN born_in_usa ON vertices.vertex_id = born_in_usa.vertex_id
JOIN lives_in_europe ON vertices.vertex_id = lives_in_europe.vertex_id;
```
**Problems:**  
➖ **Difficulty**: 29 lines vs. 4 in Cypher.  
➖ **Performance**: Recursive CTEs can be slow.  

---  

## 🏆 **When should I use graphs?**  
✔ **Social networks**: Friends of friends, recommendations.  
✔**Geodata**: Hierarchies (city → region → country).  
✔ **Recommendation systems**: "They bought this, they also buy that."  

> *"Graphs are **magic** for data with complex connections!"* ✨  

---  

### 💡 **The Council**  
- For simple links (trees), **document databases** are enough.  
- For many-to-many networks, choose graph databases (Neo4j, Amazon Neptune).  
- Modern SQL databases (PostgreSQL) support **recursive queries**, but specialized graph engines are more convenient.  

