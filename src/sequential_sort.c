#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRANSACTIONS 100000
#define ID_LENGTH 20

int main() {
    // File reading
    FILE* file = fopen("data/transactions.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    // Skip header
    char header[256];
    fgets(header, sizeof(header), file);

    // Static arrays
    char tx_ids[MAX_TRANSACTIONS][ID_LENGTH];
    double amounts[MAX_TRANSACTIONS];

    // Read transactions
    int total = 0;
    while (fscanf(file, "%[^,],%lf\n", tx_ids[total], &amounts[total]) == 2) {
        total++;
    }
    fclose(file);

    clock_t start = clock();

    // Step 1: Bubble Sort (ascending order)
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1; j++) {
            if (amounts[j] > amounts[j + 1]) {
                // Swap amounts
                double temp_amount = amounts[j];
                amounts[j] = amounts[j + 1];
                amounts[j + 1] = temp_amount;

                // Swap string contents
                char temp_id[ID_LENGTH];
                strcpy(temp_id, tx_ids[j]);
                strcpy(tx_ids[j], tx_ids[j + 1]);
                strcpy(tx_ids[j + 1], temp_id);
            }
        }
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Write results in reverse order (descending)
    FILE* out = fopen("results/sequential_sorted.txt", "w");
    if (out) {
        fprintf(out, "TransactionID,Amount\n");
        // Write in reverse order (from end to start)
        for (int i = total - 1; i >= 0; i--) {
            fprintf(out, "%s,%.2f\n", tx_ids[i], amounts[i]);
        }
        fclose(out);
    }

    // Write performance data
    FILE* perf = fopen("results/performance.txt", "a");
    if (perf) {
        fprintf(perf, "Sequential,1,1,%.5f\n", time_taken);
        fclose(perf);
    }

    printf("Sequential sort completed in %.5f seconds\n", time_taken);

    return 0;
}

