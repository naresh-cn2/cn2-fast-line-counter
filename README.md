# CN2 Fast Line Counter (C + mmap)

### 🚀 Performance: 55% Faster than Python
A high-performance C utility designed to bypass the Python GIL and OS memory overhead for massive data processing tasks.

| Tool | Dataset Size | Execution Time | Memory Strategy |
| :--- | :--- | :--- | :--- |
| **CN2-Counter (C)** | **3.6 GB** | **2.64s** | **Zero-Copy (mmap)** |
| Standard Python | 3.6 GB | 4.17s | Heap Allocation |

## 💰 The Business Value
In cloud data pipelines, compute time equals money. By replacing standard Python line-counting with this zero-allocation C binary, enterprise teams can reduce infrastructure costs by up to 50% on massive log-processing workloads.

## 🛠️ Technical Architecture
- **Zero-Copy Memory Mapping:** Uses `mmap` to map the file directly into the process address space.
- **Hardware-Level Scanning:** Scans raw bytes for `\n` (0x0A) without creating high-level string objects.
- **Zero Heap Overhead:** Eliminates the pressure on the OS memory allocator.

## How to Run
1. `gcc -O3 main.c -o cn2-counter`
2. `./cn2-counter data.txt`
