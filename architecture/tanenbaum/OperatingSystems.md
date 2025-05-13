
---

# **Modern Operating Systems (Tanenbaum) – Full table of contents**

## **Part 1: Introduction**
### **Chapter 1: Introduction**
1.1 What is an Operating System?  
1.2 History of Operating Systems  
  - 1.2.1 Early Systems (1945–1955)  
  - 1.2.2 Mainframes (1955–1980)  
  - 1.2.3 Personal Computers (1980–Present)  
1.3 Operating System Concepts  
  - 1.3.1 Processes  
  - 1.3.2 Memory Management  
  - 1.3.3 File Systems  
1.4 System Calls  
1.5 OS Structure (Monolithic, Microkernel, etc.)  

---

## **Part 2: Processes & Threads**
### **Chapter 2: Processes and Threads**
2.1 Process Model  
2.2 Thread Model  
  - 2.2.1 User-Level vs Kernel-Level Threads  
2.3 Interprocess Communication (IPC)  
  - 2.3.1 Race Conditions  
  - 2.3.2 Mutexes and Semaphores  
2.4 Scheduling  
  - 2.4.1 Round Robin, Priority Scheduling  
  - 2.4.2 Multicore Scheduling  

### **Chapter 3: Deadlocks**
3.1 Resource Allocation Graphs  
3.2 Deadlock Detection & Recovery  
3.3 Deadlock Avoidance (Banker’s Algorithm)  
3.4 Deadlock Prevention  

---

## **Part 3: Memory Management**
### **Chapter 4: Memory Management**
4.1 Address Spaces  
4.2 Swapping  
4.3 Virtual Memory  
  - 4.3.1 Paging  
  - 4.3.2 Page Replacement Algorithms (FIFO, LRU)  
4.4 Segmentation  
4.5 Memory Management in x86-64  

### **Chapter 5: Input/Output**
5.1 Principles of I/O Hardware  
5.2 Device Drivers  
5.3 Disks and SSDs  
  - 5.3.1 RAID Systems  
5.4 Clocks and Timers  

---

## **Part 4: File Systems**
### **Chapter 6: File Systems**
6.1 Files and Directories  
6.2 File System Implementation  
  - 6.2.1 Inodes  
  - 6.2.2 Journaling (ext4, NTFS)  
6.3 SSD Optimization Techniques  
6.4 Distributed File Systems (NFS, AFS)  

### **Chapter 7: Multimedia & Real-Time Systems**
7.1 Multimedia File Formats  
7.2 Disk Scheduling for Multimedia  
7.3 Real-Time Scheduling Algorithms  

---

## **Part 5: Security & Virtualization**
### **Chapter 8: Security**
8.1 Security Threats (Malware, DDoS)  
8.2 Cryptography in OS  
8.3 Authentication & Access Control  
8.4 Sandboxing and Capabilities  

### **Chapter 9: Virtualization & Cloud**
9.1 Virtual Machines (Type 1/Type 2 Hypervisors)  
9.2 Containers (Docker, Kubernetes)  
9.3 Cloud Computing Models (IaaS, PaaS, SaaS)  

---

## **Part 6: Case Studies**
### **Chapter 10: Unix/Linux**
10.1 Process Management (`fork()`, `exec()`)  
10.2 Linux Kernel Modules  
10.3 ext4 File System  

### **Chapter 11: Windows 10**
11.1 Windows NT Architecture  
11.2 Win32 API  
11.3 NTFS Features  

### **Chapter 12: Android**
12.1 Android Kernel Modifications  
12.2 Binder IPC  
12.3 Security Model (SELinux)  

