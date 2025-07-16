#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MAX_TRANSACTIONS 100000
#define NUM_THREADS 4

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total = 0;
    int tx_ids[MAX_TRANSACTIONS];
    double amounts[MAX_TRANSACTIONS];

    int local_count;
    int *local_ids;
    double *local_amounts;

    double start_time, end_time;

    if (rank == 0) {
        FILE* file = fopen("data/transactions.txt", "r");
        if (!file) {
            perror("Failed to open input file");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char line[256];
        fgets(line, sizeof(line), file); // header

        while (fscanf(file, "%d,%lf\n", &tx_ids[total], &amounts[total]) == 2) {
            total++;
        }
        fclose(file);

        printf("Total transactions: %d\n", total);
    }

    // Broadcast total count
    MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // local count
    int base = total / size;
    int rem = total % size;
    if (rank < rem) {
        local_count = base + 1;
    } else {
        local_count = base;
    }

    local_ids = (int*) malloc(local_count * sizeof(int));
    local_amounts = (double*) malloc(local_count * sizeof(double));

    // Scatterv setup
    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        sendcounts = (int*) malloc(size * sizeof(int));
        displs = (int*) malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++) {
            sendcounts[i] = (i < rem) ? base + 1 : base;
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Scatter transaction IDs
    MPI_Scatterv(tx_ids, sendcounts, displs, MPI_INT,
                 local_ids, local_count, MPI_INT,
                 0, MPI_COMM_WORLD);

    // Scatter amounts
    MPI_Scatterv(amounts, sendcounts, displs, MPI_DOUBLE,
                 local_amounts, local_count, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // Set OpenMP threads
    omp_set_num_threads(NUM_THREADS);

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // bubble sort
    #pragma omp parallel 
    for (int i = 0; i < local_count - 1; i++) {
        #pragma omp for
        for (int j = 0; j < local_count - 1; j++) {
            if (local_amounts[j] > local_amounts[j + 1]) {
                double tmp_amt = local_amounts[j];
                local_amounts[j] = local_amounts[j + 1];
                local_amounts[j + 1] = tmp_amt;

                int tmp_id = local_ids[j];
                local_ids[j] = local_ids[j + 1];
                local_ids[j + 1] = tmp_id;
            }
        }
        
    }

    MPI_Barrier(MPI_COMM_WORLD);
    

    // Gather sorted chunks
    int *recvcounts = NULL;
    int *recvdispls = NULL;
    int *all_ids = NULL;
    double *all_amounts = NULL;

    if (rank == 0) {
        recvcounts = sendcounts;
        recvdispls = displs;
        all_ids = (int*) malloc(total * sizeof(int));
        all_amounts = (double*) malloc(total * sizeof(double));
    }

    MPI_Gatherv(local_ids, local_count, MPI_INT,
                all_ids, recvcounts, recvdispls, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Gatherv(local_amounts, local_count, MPI_DOUBLE,
                all_amounts, recvcounts, recvdispls, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    //final sort 
    if (rank == 0) {
        omp_set_num_threads(NUM_THREADS);

        for (int i = 0; i < total - 1; i++) {
            #pragma omp parallel for
            for (int j = 0; j < total - 1; j++) {
                if (all_amounts[j] > all_amounts[j + 1]) {
                    double tmp_amt = all_amounts[j];
                    all_amounts[j] = all_amounts[j + 1];
                    all_amounts[j + 1] = tmp_amt;

                    int tmp_id = all_ids[j];
                    all_ids[j] = all_ids[j + 1];
                    all_ids[j + 1] = tmp_id;
                }
            }
            
        }
        end_time = MPI_Wtime();

        // Write result
        FILE* out = fopen("results/hybrid_sorted.txt", "w");
        if (out) {
            fprintf(out, "TransactionID,Amount\n");
            for (int i = total - 1; i >= 0; i--) {
                fprintf(out, "%d,%.2f\n", all_ids[i], all_amounts[i]);
            }
            fclose(out);
        }

        // Log performance
        FILE* perf = fopen("results/performance.txt", "a");
        if (perf) {
            fprintf(perf, "Hybrid,%d,%d,%.5f\n", size, NUM_THREADS, end_time - start_time);
            fclose(perf);
        }

        printf("Hybrid MPI+OpenMP sort completed in %.5f seconds with %d processes and %d threads\n",
               end_time - start_time, size, NUM_THREADS);

        free(all_ids);
        free(all_amounts);
        free(sendcounts);
        free(displs);
    }

    free(local_ids);
    free(local_amounts);

    MPI_Finalize();
    return 0;
}
