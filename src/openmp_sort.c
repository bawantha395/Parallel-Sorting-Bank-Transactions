#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_TRANSACTIONS 100000
#define ID_LENGTH 20
#define NUM_THREADS 4  // Set number of threads to 4

int main() {
    // File reading
    FILE* file = fopen("data/transactions.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    char tx_ids[MAX_TRANSACTIONS][ID_LENGTH];
    double amounts[MAX_TRANSACTIONS];
    int total = 0;

    // Skip header (optional)
    char line[256];
    fgets(line, sizeof(line), file);  // comment out if file has no header

    while (fscanf(file, "%[^,],%lf\n", tx_ids[total], &amounts[total]) == 2) {
        total++;
    }
    fclose(file);

    printf("Total transactions: %d\n", total);

    // Set number of threads
    omp_set_num_threads(NUM_THREADS);
    printf("Using %d threads\n", NUM_THREADS);

    double start = omp_get_wtime();

    // Parallel bubble sort using parallel for
    #pragma omp parallel 
    for (int i = 0; i < total - 1; i++) {
        #pragma omp for 
        for (int j = 0; j < total - 1; j++) {
            if (amounts[j] > amounts[j + 1]) {
                // Swap amounts
                double temp_amount = amounts[j];
                amounts[j] = amounts[j + 1];
                amounts[j + 1] = temp_amount;

                // Swap IDs
                char temp_id[ID_LENGTH];
                strcpy(temp_id, tx_ids[j]);
                strcpy(tx_ids[j], tx_ids[j + 1]);
                strcpy(tx_ids[j + 1], temp_id);
            }
        }
    }

    double end = omp_get_wtime();
    double time_taken = end - start;

    // Write output in descending order using reverse loop
    FILE* out = fopen("results/openmp_sorted.txt", "w");
    if (out) {
        fprintf(out, "TransactionID,Amount\n");
        for (int i = total - 1; i >= 0; i--) {
            fprintf(out, "%s,%.2f\n", tx_ids[i], amounts[i]);
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
