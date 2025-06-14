#include <stdio.h>
#include <time.h>

typedef struct {
    char tx_id[20];
    double amount;
} Transaction;

int main() {
    // Read input file
    FILE* file = fopen("data/transactions.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    // Skip header
    char header[256];
    fgets(header, sizeof(header), file);

    // Read transactions
    Transaction transactions[100000];
    int total = 0;
    while (fscanf(file, "%[^,],%lf\n",
                  transactions[total].tx_id,
                  &transactions[total].amount) == 2) {
        total++;
    }
    fclose(file);

    // Bubble sort
    clock_t start = clock();
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1; j++) {
            if (transactions[j].amount > transactions[j + 1].amount) {
                Transaction temp = transactions[j];
                transactions[j] = transactions[j + 1];
                transactions[j + 1] = temp;
            }
        }
    }
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sequential Sort Time: %f sec\n", duration);

    // Write results in reverse order (descending)
    FILE* out = fopen("results/sequential_sorted.txt", "w");
    if (out) {
        fprintf(out, "TransactionID,Amount\n");
        for (int i = total - 1; i >= 0; i--) {
            fprintf(out, "%s,%.2f\n", 
                    transactions[i].tx_id,
                    transactions[i].amount);
        }
        fclose(out);
    }

    // Write performance data
    FILE* perf = fopen("results/performance.txt", "w");
    if (perf) {
        fprintf(perf, "Model,Processes,Threads,SortTime\n");
        fprintf(perf, "Sequential,1,1,%.5f\n", duration);
        fclose(perf);
    }

    return 0;
}

