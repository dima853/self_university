# **🏗️ Chapter 4: Isolating the Domain**

The piece of software that solves problems from the domain is usually only a small part of the entire system, but its importance is DISPROPORTIONATELY high! In order for us to think about it clearly, we need to see the elements of our model as a system, rather than looking for them in a general pile of objects, like looking for constellations in the night sky. 🌌

This requires **separating domain objects** from other system functions so that:
* They are not lost in the general mass.
 Domain concepts were not blurred or confused with concepts related to software technologies.

There are advanced isolation techniques for this. This is a well-trodden path, but it is CRITICAL for the successful application of domain modeling principles.

---

## **🍰 Layered Architecture**

**Figure 4.1: Layered architecture**

Imagine that you need to perform a simple function in the shipping application: select the destination city of the cargo from the list. To do this, you need a code that:
* Draws a widget on the screen.
* Goes to the database for a list of cities.
 Interprets and validates user input.
* Connects the selected city with the cargo.
* Commits a change to the database.

But only a small part of this code relates to the **transportation business**! 🤯

**The problem:** In OOP programs, UI, database, and other support code are often written directly into business objects. Additional business logic is embedded in the behavior of UI widgets and database scripts. In the short term, this is the easiest way to make everything work.

** But when the domain code is smeared all over this pile of other code, it becomes damn difficult to see and think about it!** 😫
* A superficial UI change can accidentally change the business logic.
* Changing business rules requires painstaking tracking of UI code, database code, etc.
* Creating coherent model-driven objects becomes impractical.
* Automatic testing becomes inconvenient.

**Solution:** **Separation of concerns**. Allows you to focus on different parts of the design in isolation. The experience and conventions of the industry have led to **LAYERED Architecture**.

**The basic principle:** A layer element has dependencies ONLY on other elements in the same layer or on elements of layers **BELOW** it. Communication up should be through some kind of indirect mechanism.

---

### **📚 Standard layers**

Most successful architectures use some version of these four conceptual layers.:

| Layer                                           | Responsibility                                                                                                                                                                                                                                                                                                                                                                |
| :---------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **🎨 User Interface (UI / Presentation Layer)**  | Showing information to the user and interpreting his commands. An external actor may be another computer system.                                                                                                                                                                                                                                                              |
| **⚙️ Application Layer**                         | Defines what kind of work the software should do and directs expressive domain objects to solve problems. **The layer is thin!** It does not contain business rules or knowledge, but only coordinates tasks and delegates work to domain objects. It does not have a status reflecting the business situation, but it may have a status reflecting the progress of the task. |
| **🧠 Domain Layer (Domain Layer / Model Layer)** | **THE HEART OF business software!** Responsible for presenting business concepts, information about the business situation, and business rules. The state reflecting the business situation is monitored and used here.                                                                                                                                                       |
| **Infrastructure Layer**                        | Provides general technical capabilities to support higher layers: sending messages, persistence, rendering widgets, etc.                                                                                                                                                                                                                                                      |

**The key separation is the isolation of the domain layer!** This is exactly what **Model-Driven Design** allows.

---

## **💡 Example: Transfer of funds in an online bank**

**Figure 4.2: Interaction of layers during the transfer of funds**

*   **UI:** `transferController' — accepts a request from the user.
* **Application:** `FundsTransferService' — coordinates the task. Calls domain objects.
*   **Domain:** `Account' (a123, a456) — contains business logic: "each credit has a debit." Executes `debit()` and `credit()'.
* **Infrastructure:** `UnitOfWork', `O-R Mapper' — provides persistence, transaction management.

**Important:** The domain layer, not the application layer, is responsible for the fundamental business rules! The application also makes no assumptions about the source of the request (UI or XML request over the network). The clean division of responsibility allows the design of each layer to remain understandable.

---

## **🔗 Linking layers**

The layers should be loosely coupled, with design dependencies in only one direction (down).

* **The upper layers** can use the elements of the lower ones directly, calling their public interfaces.
* **When a lower-level object needs to report to the top** (not in response to a direct request), another mechanism is needed, for example, **callbacks** or **Observers**.

**The classic pattern for UI communication with the application and domain is MVC (Model-View-Controller).**

**The infrastructure layer** usually does not initiate actions in the domain layer. He is located "below" and should not have specific knowledge about the domain. Its features are often offered in the form of **Services**.

---

## **🖼️ Architectural frameworks**

When the infrastructure is provided in the form of **Services** called through interfaces, it is quite intuitive to understand and maintain the weak connectivity of the layers.

But some technical challenges require more "intrusive" forms of infrastructure. Frameworks that integrate multiple infrastructure needs often require other layers to be implemented in a very specific way (as a subclass of the framework class, with specific method signatures, etc.).

**The best architectural frameworks** solve complex technical problems, allowing the domain developer to focus on expressing the model. But they can also get in the way, either by imposing restrictions on domain design, or by making implementation so difficult that development slows down.

**Output:** Use frameworks selectively to solve complex problems, without looking for solutions "for all occasions." This will make the implementation less tied to the framework and keep the business objects readable and expressive.

---

## **🚫 ANTIPATTERN: Smart UI (Smart UI Anti-Pattern)**

* **When to use it?** The project needs simple functionality (data entry, mapping, few business rules). The team does not consist of advanced object designers.
* **What should I do?** Shove all the business logic into the UI! Cut the application into small functions, implement them as separate UI, embedding business rules in them. Use a relational database as a shared data warehouse. Use the most automated UI-building and visual programming tools.

** Yes, this is heresy!** 😈 But it works in certain circumstances.

**Advantages of Smart UI:**
* High and immediate productivity for simple applications.
* Less capable developers can work like this with minimal training.
* Even shortcomings in the requirements analysis can be overcome by releasing a prototype to users and quickly changing the product to meet their needs.
* Applications are loosely coupled, it is possible to plan delivery schedules of small modules relatively accurately.

**Disadvantages of Smart UI:**
* **Application integration is difficult**, only through the database.
* **There is no reuse of behavior** and no abstraction of the business problem.
* Rapid prototyping and iteration are hitting the ceiling.
* **Difficulty quickly buries you.** There is no elegant path to richer behavior.

**Output:** Smart UI is a conscious choice for simple projects with an inexperienced team. But if the project is ambitious and the team has the necessary experts — **avoid Smart UI like the plague!** 🚷

---

## **🎯 Bottom line: The domain layer is where the model lives.**

**Layered architecture** is used in most modern systems. But **Domain-Driven Design requires the mandatory presence of one specific layer — the domain layer.**

**A domain model is a set of concepts. The domain layer is the embodiment of this model and all directly related design elements.** The design and implementation of business logic make up the domain layer. In Model-Driven Design software, domain layer constructs will mirror the concepts of the model.

**It is impractical to achieve this correspondence when domain logic is mixed with other concerns of the program. Domain implementation isolation is a prerequisite for domain—driven design.**
