import subprocess
import csv
import matplotlib.pyplot as plt

num_clients_list= [
10,
20,
40,
80,
100,
200
]
throughput_list = []
avg_response_time_list = []

# Loop through different numbers of clients
for num_clients in num_clients_list:
    # Run the bash script with the specified number of clients
    command = f"./loadtest.sh {num_clients} 2 1"
    result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)

    # Parse the output to extract metrics
    output_lines = result.stdout.strip().split('\n')
    throughput = float(output_lines[0].split(': ')[1].split()[0])
    avg_response_time = float(output_lines[1].split(': ')[1].split()[0])

    throughput_list.append(throughput)
    avg_response_time_list.append(avg_response_time)

with open('performance_metrics_test_lab07.csv', 'w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['Num Clients', 'Throughput', 'Avg Response Time'])

    for i in range(len(num_clients_list)):
        writer.writerow([num_clients_list[i], throughput_list[i], avg_response_time_list[i]])


# Create subplots
fig, axes = plt.subplots(2, 3, figsize=(15, 10))
fig.suptitle('Performance Metrics vs. Number of Clients')

# Plot metrics
metrics = {
    'Throughput (requests/second)': throughput_list,
    'Average Response Time (seconds)': avg_response_time_list,
}

for i, (metric_name, metric_data) in enumerate(metrics.items()):
    ax = axes[i // 3, i % 3]
    ax.plot(num_clients_list, metric_data, marker='o')
    ax.set_xlabel('Number of Clients')
    ax.set_ylabel(metric_name)
    ax.set_title(f'{metric_name} vs. Number of Clients')
    ax.grid(True)



plt.tight_layout(rect=[0, 0, 1, 0.96])
# plt.show()

