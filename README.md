# Parallel-Sorting-Bank-Transactions

Compile each implementation using the following commands:

1. **Sequential Sort**
   ```bash
   gcc src/sequential_sort.c -o sequential_sort
   ```

2. **OpenMP Sort**
   ```bash
   gcc-15 -fopenmp src/openmp_sort.c -o openmp_sort
   ```



## Running the Implementations

Run the implementations in the following order to generate performance data:

1. **Generate Test Data** (if not already generated)
   ```bash
   python3 data/gen_data.py
   ```

2. **Run Sequential Sort**
   ```bash
   ./sequential_sort
   ```

3. **Run OpenMP Sort**
   ```bash
   ./openmp_sort
   ```

