# 📚 Chapter 2: Basic Linux Concepts

This section introduces a number of concepts related to Linux programming. It is intended for readers who have primarily worked with other operating systems or have limited experience with Linux or other UNIX-like systems.

## 🧠 2.1 Operating System kernel

The term "operating system" is usually used in two senses:

1. **The entire package** - central software that manages computer resources + standard tools (command interpreters, GUI, file utilities and editors)
2. **Core only** - central software that manages resources (CPU, RAM, devices)

In this book, we use the second meaning. The `kernel' is the foundation! 💪

### � Where does the core live?
It is usually located along the path: `/boot/vmlinuz' (or similar). History of the name:
- Before: `unix`
- With virtual memory: `vmunix`
- On Linux: `vmlinuz` (where `z` stands for compressed executable file)

### 🔧 What does the core do?
The core performs many important tasks:

| Task                               | Description                                                 | Real-life example 🏙️                                         |
| ---------------------------------- | ----------------------------------------------------------- | ----------------------------------------------------------- |
| **Process planning**               | Controls which processes are using the CPU and for how long | How does a taxi dispatcher distribute orders among drivers  |
| **Memory Management**              | Distributes RAM between processes                           | How a librarian gives books to readers 📚                    |
| **File system**                    | Provides work with files                                    | How the archive stores and organizes documents /            |
| **Creating/terminating processes** | Loads programs into memory and frees up resources           | How the restaurant accepts and clears tables                |
| **Device Access**                  | Manages devices (mice, disks, etc.)                         | How a concierge manages access to hotel services            |
| **Network functions**              | Transmits and receives network packets                      | How does the postal service deliver emails ?                |
| **System Calls (API)**             | Provides an interface for requests from processes           | How the call center accepts and processes customer requests |

Example in Java: when you call `Runtime.getRuntime().exec()`, it eventually leads to a system call through the kernel.

```java
// Example of creating a process in Java
Process process = Runtime.getRuntime().exec("ls -l");
```

### 👑 Processor operating modes

Modern processors support at least two modes:

| Mode                      | Access              | Example                                    |
| ------------------------- | ------------------- | ------------------------------------------ |
| **User**                  | User space only     | Common applications (browser, text editor) |
| **The core (supervisor)** | User + kernel space | Device drivers, critical operations        |

### 👀 Two points of view

**From the point of view of the process:**
- Does not know when it will be interrupted
- Does not know where it is in memory
- Cannot communicate directly with other processes
- Cannot work directly with devices

**From the point of view of the core:**
- Knows and controls EVERYTHING
- Decides which process is running when
- Manages all resources
- Provides isolation of processes

## 🐚 2.2 Shell

Shell is a special program that reads user commands and executes the corresponding programs. It's like a personal assistant in the Linux world! 🤖

## Shell types:
- **Bourne shell (sh)** - the grandfather of all shells
- **C shell (csh)** - with syntax similar to C
- **Korn shell (ksh)** - improved Bourne shell
- **Bash (Bourne again shell)** - the most popular on Linux

### 📜 Shell scripts
Shells can execute not only interactive commands, but also scripts (scripts) - text files with commands. It's like recipes for a chef! 👨🍳

An example of a simple bash script:
```bash
#!/bin/bash
echo "Hello, world! 🌍"
date
```

## 👥 2.3 Users and Groups

Linux is a multiuser system where everyone has their own rights and limitations.

### 🔑 Users:
- Unique name (login) and ID (UID)
- Information is stored in `/etc/passwd`
- Home directory and login shell

### 👪 Groups:
- Organization of users for access control
- Information in `/etc/group`
- A user can be in several groups

## Superuser (root):
- UID = 0
- Bypasses all rights checks
- How is the master key at the hotel

Example of rights verification in Java:
```java
File file = new File("/etc/shadow");
System.out.println("Can I read? " + file.canRead()); // false for a regular user
```

## Conclusion

This chapter has laid the foundation for understanding the basic concepts of Linux. We found out:
- How does the core work and what tasks does it perform
- What kind of shells are there and why are they needed 🐚
- How users and groups are organized in the system

Next, we will delve into these topics and learn how to interact with the system through programming! 🚀

P.S. Remember: when a process "wants" to do something, it actually asks the kernel for it. It's like a child asking his parents to give him candy - the last word is always with the core!

# 🚀 Continuing to learn fundamental Linux concepts!

## ⚖️ 2.7 Resource Limits

Each process in Linux has **limits** on the use of system resources. It's like credit card limits 💳 - you can't spend more than the set limit!

- **Soft limit** - the current limit that the process can change
- **Hard limit** - the maximum ceiling that cannot be exceeded (only the superuser can change it)

```bash
# View current limits in bash
ulimit -a
```

A real-life example: how parents set a limit on their child's pocket money 👶 - you can spend up to N rubles a day (soft limit), but no more than M rubles a month (hard limit).

## 🗺️ 2.8 Memory Mapping (mmap)

The `mmap()` function is like a magic portal between a file and memory! 🧙‍♂️

**Mapping types:**
- **File** - associates a file area with memory (changes can be saved to a file)
- **Anonymous** - simply allocates memory initialized with zeros (like a blank sheet of paper 📄)

**Access modes:**
- `PRIVATE` - changes are visible only to the current process (like a personal diary 🔒)
- `SHARED` - changes are visible to everyone and can be saved to a file (like a public bulletin board 📌)

Example in C:
```c
// Map a file to memory
int fd = open("data.txt", O_RDWR);
char *addr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
```

## 📚 2.9 Libraries: static and shared

### Static libraries (.a)
- Library code is **embedded** into the executable file
- Pros: easy to distribute (all inclusive)
- Cons: code duplication, larger size, harder to update

```bash
# Creating a static library
ar rcs libmylib.a file1.o file2.o
```

### Shared libraries (.so)
- Code is loaded **dynamically** at startup
- Pros: saves memory, easier to update
- Cons: needed on the target system

```bash
# Creating a shared library
gcc -shared -o libmylib.so file1.o file2.o
```

Real-life example:
Static libraries - how to carry all your books 📚 in a backpack (heavy, but always at hand).
Shared libraries - how to borrow books from the library 🏛️ (easy, but you need the library nearby).

## 💬 2.10 Interprocess communication (IPC)

Linux provides a rich set of mechanisms for process communication:

| Mechanism          | Description                | Real-life example                 |
| ------------------ | -------------------------- | --------------------------------- |
| **Signals**        | Event notifications        | Like SMS notification 📱           |
| **Pipes**          | One-way data flow          | Water pipe 🚰                      |
| **FIFO**           | Named pipes                | Mailbox 📬                         |
| **Sockets**        | Network and local exchange | Phone call 📞                      |
| **Shared memory**  | Shared memory area         | Shared whiteboard in the office 📋 |
| **Message Queues** | Structured Messages        | Chat System 💬                     |

Python example (creating a pipe):
```python
import os
r, w = os.pipe()
os.write(w, b"Hello from another process!")
```

## ⚡ 2.11 Signals

Signals are "interrupts" for processes. The most popular ones are:

- `SIGINT` (Ctrl+C) - keyboard interrupt
- `SIGKILL` - immediate process kill
- `SIGTERM` - polite request to terminate
- `SIGSEGV` - segmentation violation (usually memory access error)

Signal handling example in C:
```c
#include <signal.h>

void handler(int sig) {
printf("Received signal %d!\n", sig);
}

int main() {
signal(SIGINT, handler); // Set up the handler
while(1); // Infinite loop
}
```

## 🧵 2.12 Threads

Threads are "lightweight" processes that share:
- Memory (global variables)
- Open files
- Process ID

But each thread has:
- Its own call stack
- Its own processor registers

An example of creating threads in Java:
```java
new Thread(() -> {
System.out.println("I am a new thread!");
}).start();
```

## 🎛️ 2.13 Process groups and job control

When you run a command with `|` in the shell (for example `ls | grep test`), all processes in the pipeline are put into one **process group**.

Job control allows you to:
- Move tasks to foreground/background (Ctrl+Z, `fg`, `bg`)
- Send signals to the entire group

```bash
# Run in background
sleep 100 &
# View jobs
jobs
# Bring to foreground
fg %1
```

## 🕒 2.16 Time in Linux

There are two types of time in Linux:
1. **Real time** (since January 1, 1970 - Epoch)
2. **CPU time** (how much CPU the process used)

Example in C:
```c
#include <time.h>
time_t now = time(NULL); // Current time in seconds since Epoch
printf("Now: %s", ctime(&now));
```

## 🖥️ 2.17 Client-server architecture

Classical model:
- **Server** - provides services (like a restaurant 🍽️)
- **Client** - uses services (like a visitor)

Examples:
- Web server and browser
- Database server and client application
- Print daemon and print program

## ⏱️ 2.18 Realtime

Realtime ≠ real time! This is guaranteed response time ⏳.

Characteristics:
- Hard deadlines
- Predictable behavior
- Priority scheduling

Linux supports realtime via:
- Realtime signals
- Realtime scheduling