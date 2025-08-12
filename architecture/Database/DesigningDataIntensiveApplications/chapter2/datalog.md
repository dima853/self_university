# 🔍 **Datalog: The academic foundation of graph Queries**  

> *"Datalog is like SQL for graphs, but with declarative recursion and logic programming!"*  

## 📌 **What is Datalog?**  
- **Query language**, developed in the 1980s for working with graphs and relational data.  
- Used in **Datomic** and **Hadoop** (via Cascalog).  
- Based on **predicate logic** (like Prolog).  

### Sample data (Example 2-10)  
```prolog
name(namerica, 'North America').  
type(namerica, continent).  
within(usa, namerica).  
born_in(lucy, idaho).  
```  
**Key Features:**  
✔ Data is stored as **predicates**, not triples.  
 There is no division into properties and edges — these are all predicates.  

---  

## 🧠 **How do queries work in Datalog?**  
Example 2-11: Find people who have moved from the USA to Europe  
```prolog
/* Rule 1: Location " contains " itself */
within_recursive(Location, Name) :- name(Location, Name).  

/* Rule 2: Recursive nesting traversal */
within_recursive(Location, Name) :-  
  within(Location, Via),  
  within_recursive(Via, Name).  

/* Rule 3: Migrant search */
migrated(Name, BornIn, LivingIn) :-  
  name(Person, Name),  
  born_in(Person, BornLoc),  
  within_recursive(BornLoc, BornIn),  
  lives_in(Person, LivingLoc),  
  within_recursive(LivingLoc, LivingIn).  

/* Query: Who moved from the USA to Europe? */  
?- migrated(Who, 'United States', 'Europe').  /* Response: Who = 'Lucy' */  
```  

** How does it work?**  
1. 'within_recursive` recursively finds all nested locations (for example, Idaho → USA → North America).  
2. `migrated' connects a person (`Person`) with the places of birth (`BornIn`) and residence (`LivingIn`).
3. The query searches for matches for `BornIn = 'United States' and `LivingIn = 'Europe'.  

---  

## 🔄 **Comparison with Cypher/SPARQL**  
| **Criterion** | **Datalog**                      | **Cypher/SPARQL**                 |
| ------------- | -------------------------------- | --------------------------------- |
| **Syntax**    | Logical Rules                    | Graph Patterns                    |
| **Recursion** | Built-in support                 | Via `*` (for example, `:WITHIN*`) |
| **Usage**     | Academic/spec. systems (Datomic) | Popular Graph databases (Neo4j)   |

> *"Datalog is a 'low—level language' for graph queries in which Cypher and SPARQL can be implemented!"*  

---  

## 🏆 **Why is Datalog important?**  
✔ **A theoretical framework** for modern graph languages.  
✔ **Recursive queries** look natural.  
✔**Composability**: Rules can be reused.  

**Disadvantages:**  
 Less intuitive for simple queries.  
➖ Fewer tools and communities.  

---  

## 🌐 **Where is it used?**  
- **Datomic**: A database with an emphasis on immutability and temporary queries.  
- **Graph analysis**: Social networks, recommendations, ontologies.  
- **Logical analysis**: For example, checking dependencies in the code.  

---  

# 🎯 **Conclusion: Choosing a data model**  

### 📌 **The three main models today:**  
1. **Relational databases** — for strict schemas and complex JOINS.  
2. **Document databases** — for hierarchical data and flexibility.  
3. **Graph databases** — for networks with complex connections.  

### 🔮 **Trends:**  
- **Hybrid solutions** (for example, PostgreSQL with JSON and graph extensions).  
- **Specialized databases** for niche tasks (genomics, particle physics).  

> *"There is no 'silver bullet'. Choose a model for your data, not the other way around!"*  

**What's next?** The next chapter will show how these models ** are implemented at a low level** (indexes, repositories, optimizations).  

---