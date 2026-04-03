# CN2 High-Speed Log Scanner V1.1
**Infrastructure Optimization: 50x faster than Python, reducing cloud compute costs by 98%.**

This is a systems-level C engine designed to saturate NVMe hardware limits for high-frequency log analysis. By utilizing memory mapping (`mmap`) and SIMD-optimized jumps (`memchr`), it bypasses standard I/O bottlenecks to process multi-gigabyte datasets in real-time.

### **🚀 Performance Benchmarks (Acer Nitro 16 / NVMe SSD)**
| Metric | Result |
| :--- | :--- |
| **Throughput** | **2.92 GB/s** |
| **Execution Time** | **1.23 Seconds** |
| **Dataset Size** | 3.7 GB (approx. 107M lines) |
| **Optimization** | SIMD Vectorization / Zero-Copy I/O |

### **🛠️ Build & Run**
Requires `gcc`. 
```bash
make
./fast_filter_v2 data.txt ERROR WARNING CRITICAL
