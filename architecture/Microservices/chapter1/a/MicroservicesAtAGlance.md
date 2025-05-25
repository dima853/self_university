# Microservices At a Glance

## **Summary: Microservices and Service-oriented architecture (SOA)**  

#### **1. Defining microservices**  
Microservices are **independently deployable services** organized around a **business domain**. Each service encapsulates certain functionality and provides it to other services through **network interfaces** (for example, REST API or queues).  

**Example:**  
- Inventory service ****  
- Order management service ****  
- **Delivery service**  
Together they form **a single e-commerce system**.  

#### **2. Key characteristics of microservices**  
- **Independent deployability** – it is possible to update one service without affecting others.  
- **Technology agnosticism** – Each service can be written on its own technology stack.  
- **Data encapsulation** – each service has its own database, shared databases are avoided.  
- **Black box** – the internal implementation is hidden, the interaction is only through the API.  
- **Information secrecy** – minimum data in the interfaces, maximum inside the service.  

#### **3. Advantages of microservices**  
- **Flexibility of changes** – you can change the internal logic without affecting other services.  
- **Scalability** – Each service scales independently.  
- **Separation of responsibilities** – Teams can work on different services independently.  
- **Technological freedom** – you can choose different languages and databases for specific tasks.  

#### **4. Microservices vs. SOA**  
**SOA (Service-Oriented Architecture)** is a general approach where services interact over a network.  
**The problems of classical SOA:**  
- Unclear boundaries of services.  
- Using shared databases → strong connectivity.  
- Complex protocols (SOAP, ESB).  
- Dependence on vendor solutions.  

**Microservices are an evolution of SOA:**  
- Clear boundaries of services by business domains.  
- Independent deployment.  
- Simple APIs (REST, gRPC, queues).  
- Emphasis on autonomy and loose connectivity.  

#### **5. Principles of microservices design**  
- **Division by business domains** (Domain-Driven Design).  
- **Stable API** – changes should not break clients.  
- **Abandoning shared databases** – each service manages its own data.  
- **Easy replacement** – The service can be rewritten without affecting the system.  

#### **6. Conclusion**  
Microservices are a practical approach to SOA that solves the problems of monoliths and complex SOA systems. Key ideas: **autonomy, weak connectivity, independent deployment**.  

> **Analogy:**  
> If SOA is an "architecture of services in general", then microservices are "SOA done right".