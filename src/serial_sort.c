#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TRANSACTIONS 100000

int main() {
    
    FILE* file = fopen("data/transactions.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    // header
    char header[256];
    fgets(header, sizeof(header), file);

    
    int tx_ids[MAX_TRANSACTIONS];
    double amounts[MAX_TRANSACTIONS];

    // Read transaction
    int total = 0;
    while (fscanf(file, "%d,%lf\n", &tx_ids[total], &amounts[total]) == 2) {
        total++;
    }
    fclose(file);

    clock_t start = clock();

    
    for (int i = 0; i < total - 1; i++) {
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
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Write result 
    FILE* out = fopen("results/serial_sorted.txt", "w");
    if (out) {
        fprintf(out, "TransactionID,Amount\n");
        
        for (int i = total - 1; i >= 0; i--) {
            fprintf(out, "%d,%.2f\n", tx_ids[i], amounts[i]);
        }
        fclose(out);
    }

    // appnd performance data
    FILE* perf = fopen("results/performance.txt", "a");
    if (perf) {
        fprintf(perf, "Serial,1,1,%.5f\n", time_taken);
        fclose(perf);
    }

    printf("Serial sort completed in %.5f seconds\n", time_taken);

    return 0;
}

