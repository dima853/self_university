### **Summary: Domain Models in software development**  

#### **1. Models as a simplification of reality**  
- **An example of an 18th-century Chinese map**:
— China is depicted in the center, the rest of the countries are schematically shown.  
  - This reflected China's isolationist policy, but it did not help to interact with the outside world.  
  - **Conclusion**: A model is ** a simplified representation of reality** that focuses on important aspects and ignores unnecessary ones.  

#### **2. What is a Domain?**  
- **Domain** is the user's area of activity that the program concerns.  
  - Examples:
- Airline tickets: real people, planes, flights.  
    - Accounting: money, bills, taxes.  
    - Version control system: software development.  
- A domain is usually **not directly connected to computers**, but requires a deep understanding of the subject area.  

#### **3. Why do we need domain models?**  
- **Problem**: The amount of domain knowledge is huge and complex.  
- **Solution**: **Model** is:  
  - **Structured simplification** of knowledge.  
  - **Abstraction**, which helps to focus on the task.  
  - **A tool** to combat information overload.  

#### **4. What is the Domain Model?**  
- **It's not just a diagram or a code**, but the **idea** that they convey.  
- ** Not "realism"**, but a useful abstraction (like cinema— not real life, but interpretation).  
- **Example**:  
  - The documentary film **edits reality** to convey meaning.  
  - Similarly, the domain model **selects the important** and discards the unimportant.  

#### **5. How is the Domain Model created?**  
1. **Domain analysis** (communication with experts).  
2. **Structuring knowledge** (highlighting key entities and relationships).  
3. **Abstraction** (ignoring unnecessary details).  
4. **Fixing the model** (in the form of diagrams, code or descriptions).  

#### **6. Examples of models**  
| Real world                    | Model in the program        |
| ----------------------------- | --------------------------- |
| Passenger, Plane, flight      | `Class Passenger`, `Flight` |
| Money, accounts, transactions | `Account`, `Transaction`    |

#### **7. Conclusion**  
- **Domain Model** is **a deliberately simplified representation of knowledge** about a domain.  
- She helps:  
  - **Understand** complex processes.  
  - **Communicate** between developers and experts.  
  - **Create software** that solves users' real-world problems.  

**Key point**: A good model is not one that is "realistic", but one that is **useful** for solving a specific problem.