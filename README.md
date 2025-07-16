# Parallel-Sorting-Bank-Transactions

env
python3 -m venv venv
source venv/bin/activate


Compile each implementation using the following commands:

1. **Serial Sort**
   ```bash
   gcc src/serial_sort.c -o serial_sort
   ```

2. **OpenMP Sort**
   ```bash
   gcc-15 -fopenmp src/openmp_sort.c -o openmp_sort
   ```

3. **MPI Sort**
   ```bash
   mpicc src/mpi_sort.c -o mpi_sort
   ```

4. **Hybrid Sort**
   ```bash
   mpicc -fopenmp src/hybrid_sort.c -o hybrid_sort
   gcc-15 -fopenmp -I/opt/homebrew/Cellar/mpich/4.3.0/include -L/opt/homebrew/Cellar/mpich/4.3.0/lib -lmpi -lpmpi src/hybrid_sort.c -o hybrid_sort
   ```




## Running the Implementations

Run the implementations in the following order to generate performance data:

1. **Generate Test Data** 
   ```bash
   python3 data/gen_data.py
   ```

2. **Run Serial Sort**
   ```bash
   ./serial_sort
   ```

3. **Run OpenMP Sort**
   ```bash
   ./openmp_sort
   ```

4. **Run MPI Sort** (with 4 processes)
   ```bash
   mpirun -np 4 ./mpi_sort
   ```

5. **Run Hybrid Sort** (with 4 processes)
   ```bash
   mpirun -np 4 ./hybrid_sort
   ```


## Performance Analysis

```bash
python3 results/analyze_performance.py
```
