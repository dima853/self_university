# 🌡️ PID Thermostat Simulator (Java)

**Project of a learning management system with feedback**  
Simulates the operation of a "smart thermostat" with a PID controller algorithm.

## 🎯 Why do we need this project?

1. **Learning objectives**:
- Visual demonstration of feedback control principles
    - The practice of implementing a PID controller
- Working with a dynamic model of the system

2. **Technology stack**:
- Pure Java (without dependencies)
- Object-oriented approach
- "Real-time" model (step-by-step execution)

## 🛠️ How does it work?

```mermaid
graph TD
    A[Set temperature] --> B(PID controller)
C[Temperature sensor] --> B
    B --> D[Heater control]
    D --> E[Room]
    E --> C