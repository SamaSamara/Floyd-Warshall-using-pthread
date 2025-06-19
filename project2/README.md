# Floyd-Warshall Using OpenMP
### Overview
This project implements a parallel version of Floyd-Warshall algorithm using OpenMP. The goal was to improve the performace compared to the sequential implementation.
### Parallelization Strategy
- This code uses OpenMP directive `#pragma omp parallel for` to parallelize the inner nested loop for the algorithm.
- The implementation doesn't contain any race condition, and there was no need for dynamic scheduling.
### How to compile 
- make sure you have GCC with openmp support.
- compile:
```bash
g++ -fopenmp -o floyd openmp.cpp
```
- Run the code:
```bash
./floyd
```

### Performance Comparison
#### Hardware used
- **CPU**: AMD Ryzen 5 5625u, 6 cores, 12 Threads.
- **RAM**: 16 GB
- **OS**: Windows 11 WSL/Ubuntu 24.04.
#### Input sizes tested 
Tested for 100, 300, 500, 600, 2000

### Speedup Achieved
