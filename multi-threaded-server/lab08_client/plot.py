import subprocess
import matplotlib.pyplot as plt
import csv
import os
import time

# Initialize lists to store results
num_clients_list = [
10,
20,
40,
80,
100,
200,
300
]
throughput_list = []
avg_response_time_list = []
requests_sent_rate_list = []
successful_requests_rate_list = []
timeout_requests_rate_list = []
error_requests_rate_list = []
average_active_threads_list = []
overall_cpu_utilization_list = []

# Loop through different numbers of clients
for num_clients in num_clients_list:
    # Run the bash script with the specified number of clients
   
    command = f"./loadtest.sh {num_clients} 2 1"
    result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)
    time.sleep(1)
    # Parse the output to extract metrics
    output_lines = result.stdout.strip().split('\n')
    # throughput = float(output_lines[0].split(': ')[1].split()[0])
    avg_response_time = float(output_lines[0].split(': ')[1].split()[0])
    throughput = float(output_lines[1].split(': ')[1].split()[0])
    successful_requests_rate = float(output_lines[2].split(': ')[1].split()[0])
    timeout_requests_rate = float(output_lines[3].split(': ')[1])
    error_requests_rate = float(output_lines[4].split(': ')[1])
    requests_sent_rate = float(output_lines[5].split(': ')[1])
    average_active_threads = float(output_lines[6].split(': ')[1])
    overall_cpu_utilization = float(output_lines[7].split(': ')[1].rstrip('%'))


    avg_response_time_list.append(avg_response_time)
    throughput_list.append(throughput)
    successful_requests_rate_list.append(successful_requests_rate)
    timeout_requests_rate_list.append(timeout_requests_rate)
    error_requests_rate_list.append(error_requests_rate)
    requests_sent_rate_list.append(requests_sent_rate)
    average_active_threads_list.append(average_active_threads)
    overall_cpu_utilization_list.append(overall_cpu_utilization)

# Save the data to a CSV file
with open('performance_metrics_lab8.csv', 'w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['Num Clients', 'Avg Response Time', 'Throughput',
                     'Successful Requests Rate', 'Timeout Requests Rate', 'Error Requests Rate',
                     'Requests Sent Rate','Average Active Threads', 'Overall CPU Utilization'])

    for i in range(len(num_clients_list)):
        writer.writerow([num_clients_list[i], avg_response_time_list[i], throughput_list[i],
                         successful_requests_rate_list[i],
                         timeout_requests_rate_list[i], error_requests_rate_list[i], requests_sent_rate_list[i],
                         average_active_threads_list[i], overall_cpu_utilization_list[i]])

# Create subplots
fig, axes = plt.subplots(2, 3, figsize=(15, 10))
fig.suptitle('Performance Metrics vs. Number of Clients')

# Plot metrics
metrics = {
    'Average Response Time (seconds)': avg_response_time_list,
    'Throughput (requests/second)': throughput_list,
    'Successful Requests Rate (Goodput)': successful_requests_rate_list,
    'Timeout Requests Rate': timeout_requests_rate_list,
    'Error Requests Rate': error_requests_rate_list,
    'Requests Sent Rate': requests_sent_rate_list
    
}

for i, (metric_name, metric_data) in enumerate(metrics.items()):
    ax = axes[i // 3, i % 3]
    ax.plot(num_clients_list, metric_data, marker='o')
    ax.set_xlabel('Number of Clients')
    ax.set_ylabel(metric_name)
    ax.set_title(f'{metric_name} vs. Number of Clients')
    ax.grid(True)

# Create a separate plot for CPU utilization and active threads
fig2, axes2 = plt.subplots(1, 2, figsize=(15, 5))
fig2.suptitle('Resource Utilization vs. Number of Clients')

# Plot CPU utilization
axes2[0].plot(num_clients_list, overall_cpu_utilization_list, marker='o', color='red')
axes2[0].set_xlabel('Number of Clients')
axes2[0].set_ylabel('Overall CPU Utilization (%)')
axes2[0].set_title('Overall CPU Utilization vs. Number of Clients')
axes2[0].grid(True)

# Plot active threads
axes2[1].plot(num_clients_list, average_active_threads_list, marker='o', color='blue')
axes2[1].set_xlabel('Number of Clients')
axes2[1].set_ylabel('Average Number of Active Threads')
axes2[1].set_title('Average Number of Active Threads vs. Number of Clients')
axes2[1].grid(True)

plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()

