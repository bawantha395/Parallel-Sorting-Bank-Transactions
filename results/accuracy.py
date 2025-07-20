import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def calculate_rmse(serial_file, parallel_file):
    """
    Calculate the Root Mean Square Error (RMSE) between two sorted transaction files.

    Args:
        serial_file (str): Path to the serial sorted file.
        parallel_file (str): Path to the parallel sorted file.

    Returns:
        float: The RMSE value.
    """
    # Load data from files
    serial_data = np.loadtxt(serial_file, delimiter=',', skiprows=1, usecols=1)
    parallel_data = np.loadtxt(parallel_file, delimiter=',', skiprows=1, usecols=1)

    # Ensure both arrays have the same length
    if len(serial_data) != len(parallel_data):
        raise ValueError("Files have different number of transactions.")

    # Calculate RMSE
    rmse = np.sqrt(np.mean((serial_data - parallel_data) ** 2))
    return rmse

if __name__ == "__main__":
    try:
        # Define file paths for comparisons
        comparisons = [
            ("results/serial_sorted.txt", "results/openmp_sorted.txt", "Serial vs OpenMP"),
            ("results/serial_sorted.txt", "results/mpi_sorted.txt", "Serial vs MPI"),
            ("results/serial_sorted.txt", "results/hybrid_sorted.txt", "Serial vs Hybrid")
        ]

        # Prepare data for table and graph
        table_data = []
        labels = []
        rmse_values = []

        for serial_file, parallel_file, label in comparisons:
            rmse = calculate_rmse(serial_file, parallel_file)
            table_data.append({"Comparison": label, "RMSE": rmse})
            labels.append(label)
            rmse_values.append(rmse)

        # Print results in a table format
        df = pd.DataFrame(table_data)
        print(df.to_string(index=False))

        # Save the RMSE plot to the results folder
        plt.bar(labels, rmse_values, color=['blue', 'green', 'orange'])
        plt.ylabel("RMSE Value")
        plt.title("RMSE Comparisons")
        plt.xticks(rotation=45, ha="right")
        plt.tight_layout()
        plt.savefig("results/rmse_comparison.png")
        print("Plot saved to results/rmse_comparison.png")

    except Exception as e:
        print(f"Error: {e}")
