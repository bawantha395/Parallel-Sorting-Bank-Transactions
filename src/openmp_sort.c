#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_TRANSACTIONS 100000
#define NUM_THREADS 4  

int main() {
    FILE* file = fopen("data/transactions.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    int tx_ids[MAX_TRANSACTIONS];
    double amounts[MAX_TRANSACTIONS];
    
    int total = 0;

    // header 
    char line[256];
    fgets(line, sizeof(line), file);  

    // Read transaction
    while (fscanf(file, "%d,%lf\n", &tx_ids[total], &amounts[total]) == 2) {
        total++;
    }
    fclose(file);

    printf("Total transactions: %d\n", total);

    
    omp_set_num_threads(NUM_THREADS);
    printf("Using %d threads\n", NUM_THREADS);

    double start = omp_get_wtime();

    #pragma omp parallel 
    for (int i = 0; i < total - 1; i++) {
        #pragma omp for
        for (int j = 0; j < total - 1; j++) {
            if (amounts[j] > amounts[j + 1]) {
                
                double temp_amount = amounts[j];
                amounts[j] = amounts[j + 1];
                amounts[j + 1] = temp_amount;

                
                int temp_id = tx_ids[j];
                tx_ids[j] = tx_ids[j + 1];
                tx_ids[j + 1] = temp_id;
            }
        }
        #pragma omp barrier 
        
    }

    double end = omp_get_wtime();
    double time_taken = end - start;

    // Write output 
    FILE* out = fopen("results/openmp_sorted.txt", "w");
    if (out) {
        fprintf(out, "TransactionID,Amount\n");
        for (int i = total - 1; i >= 0; i--) {
            fprintf(out, "%d,%.2f\n", tx_ids[i], amounts[i]);
        }
        fclose(out);
    }

    // Log performance
    FILE* perf = fopen("results/performance.txt", "a");
    if (perf) {
        fprintf(perf, "OpenMP,1,%d,%.5f\n", NUM_THREADS, time_taken);
        fclose(perf);
    }

    printf("OpenMP sort completed in %.5f seconds using %d threads\n", time_taken, NUM_THREADS);

    return 0;
}
