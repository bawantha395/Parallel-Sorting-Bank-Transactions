import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the performance data
def read_performance_data():
    data = []
    with open('results/performance.txt', 'r') as f:
        # Skip header
        next(f)
        for line in f:
            model, processes, threads, time = line.strip().split(',')
            data.append({
                'Model': model,
                'Processes': int(processes),
                'Threads': int(threads),
                'SortTime': float(time)
            })
    return pd.DataFrame(data)

# Create comparison graphs
def create_comparison_graphs(df):
    # Set seaborn style
    sns.set_theme(style="whitegrid")

    # Create a new column for configuration labels
    df['Config'] = df.apply(lambda row: f"{row['Model']} {row['Processes']}x{row['Threads']}", axis=1)

    # Execution Time Plot
    plt.figure(figsize=(10, 6))
    bars1 = sns.barplot(data=df, x='Config', y='SortTime', palette='viridis')
    plt.title('Sort Time Comparison', pad=20, fontsize=12, fontweight='bold')
    plt.ylabel('Time (seconds)', fontsize=10)
    plt.xlabel('Configuration (Processors x Threads)', fontsize=10)
    plt.xticks(rotation=30, ha='right')
    for bar in bars1.patches:
        plt.text(bar.get_x() + bar.get_width()/2., bar.get_height(),
                 f'{bar.get_height():.2f}s',
                 ha='center', va='bottom', fontsize=10)
    plt.tight_layout()
    plt.savefig('results/execution_time_plot.png', dpi=300, bbox_inches='tight')
    plt.close()

    # Speedup calculation (compared to Serial)
    serial_time = df[df['Model'] == 'Serial']['SortTime'].values[0]
    df['Speedup'] = serial_time / df['SortTime']

    # Speedup Plot
    plt.figure(figsize=(10, 6))
    bars2 = sns.barplot(data=df, x='Config', y='Speedup', palette='viridis')
    plt.title('Speedup Comparison (vs Serial)', pad=20, fontsize=12, fontweight='bold')
    plt.ylabel('Speedup Factor', fontsize=10)
    plt.xlabel('Configuration (Processors x Threads)', fontsize=10)
    plt.xticks(rotation=30, ha='right')
    for bar in bars2.patches:
        plt.text(bar.get_x() + bar.get_width()/2., bar.get_height(),
                 f'{bar.get_height():.2f}x',
                 ha='center', va='bottom', fontsize=10)
    plt.tight_layout()
    plt.savefig('results/speedup_plot.png', dpi=300, bbox_inches='tight')
    plt.close()

def main():
    # Read data
    df = read_performance_data()
    
    # Save to CSV
    df.to_csv('results/performance_analysis.csv', index=False)
    
    # Create graphs
    create_comparison_graphs(df)
    
    # Print summary
    print("\nPerformance Analysis Summary:")
    print("============================")
    print("\nRaw Data:")
    print(df.to_string(index=False))
    
    print("\nSpeedup Analysis:")
    serial_time = df[df['Model'] == 'Serial']['SortTime'].values[0]
    for _, row in df.iterrows():
        if row['Model'] != 'Serial':
            speedup = serial_time / row['SortTime']
            print(f"{row['Model']}: {speedup:.2f}x speedup")

if __name__ == "__main__":
    main()