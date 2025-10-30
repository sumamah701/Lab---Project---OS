# 🧵 Thread-pool Based Parallel Task Executor

## 📘 Project Overview
This project implements a **thread-pool based parallel task executor** in **C++17**, designed for efficient and thread-safe execution of multiple concurrent tasks.  
It demonstrates communication between a **client thread pool** that submits work and a **worker thread pool** that executes those tasks using a **bounded blocking queue**.

---

## 👥 Group Members
| Roll Number | Name |
|--------------|------|
| BSCS23203 | Sumamah Ahmad |
| BSCS23175 | Danial Shakeel |

---

## 🧩 Design Summary

### **Goals**
- Implement a robust and reusable thread pool using modern C++.
- Provide safe communication between producer (client) and consumer (worker) threads.
- Prevent data races, deadlocks, and memory leaks.
- Ensure graceful shutdown and task completion.
- Support easy validation using **ThreadSanitizer (TSAN)** and **Valgrind**.

### **Architecture**
- **ThreadPool**: Manages worker threads and task scheduling.
- **Task Queue**: A bounded FIFO queue implemented using `std::deque`, protected by `std::mutex` and `std::condition_variable`.
- **Client API**: Allows safe task submission using `submit(std::function<void()>)`.

---

## 🔄 Communication Mechanism

**Chosen Mechanism:**  
**Bounded Blocking Queue (Producer-Consumer Pattern)**  
Implemented using `std::mutex` and `std::condition_variable`.

This ensures:
- Safe synchronization between clients and workers.
- Controlled memory usage (queue cannot grow indefinitely).
- Automatic blocking of clients when the queue is full and workers when it is empty.

---

## ⚙️ How to Build & Run

### **Requirements**
- C++17 or higher
- `make` utility
- Linux environment (recommended)
- Tools: **ThreadSanitizer (TSAN)** and **Valgrind**

---

### **Build Commands**

```bash
# Normal build
make

# Run demo
./threadpool_demo

# Run with ThreadSanitizer
make tsan && ./threadpool_demo

# Run with Valgrind
make valgrind && valgrind --leak-check=full --show-leak-kinds=all ./threadpool_demo
