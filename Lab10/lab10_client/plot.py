# import subprocess
# import csv
# import matplotlib.pyplot as plt

# # Initialize lists to store results
# num_clients_list = [10,20]
# avg_response_time_list = []
# throughput_list = []
# cpu_utilization_list = []

# # Create a CSV file to store the data
# csv_file_path = "performance_metrics_test.csv"
# with open(csv_file_path, mode='w', newline='') as csv_file:
#     fieldnames = ['Number of Clients', 'Response Time (seconds)', 'Throughput', 'CPU Utilization']
#     writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
    
#     # Write the CSV header
#     writer.writeheader()

#     # Loop through different numbers of clients
#     for num_clients in num_clients_list:
#         # Run the bash script with the specified number of clients
#         command = f"./loadtest.sh 127.0.0.1:8000 pass.cpp {num_clients} 2"
#         result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)

#         # Parse the output to extract metrics
#         output_lines = result.stdout.strip().split('\n')
#         avg_response_time = float(output_lines[0].split(':')[1].split()[0])
#         throughput = float(output_lines[1].split(':')[1])
#         cpu_utilization = float(output_lines[2].split(': ')[1].rstrip('%'))


#         # Append the metrics to the lists
#         avg_response_time_list.append(avg_response_time)
#         throughput_list.append(throughput)
#         cpu_utilization_list.append(cpu_utilization)

#         # Write the data to the CSV file
#         writer.writerow({
#             'Number of Clients': num_clients,
#             'Response Time (seconds)': avg_response_time,
#             'Throughput': throughput,
#             'CPU Utilization': cpu_utilization,
#         })

# # Create subplots
# fig, axes = plt.subplots(1, 3, figsize=(15, 5))
# fig.suptitle('Performance Metrics vs. Number of Clients')

# # Plot metrics
# metrics = {
#     'Average Response Time (seconds)': avg_response_time_list,
#     'Throughput': throughput_list,
#     'CPU Utilization': cpu_utilization_list,
# }

# for i, (metric_name, metric_data) in enumerate(metrics.items()):
#     ax = axes[i]
#     ax.plot(num_clients_list, metric_data, marker='o')
#     ax.set_xlabel('Number of Clients')
#     ax.set_ylabel(metric_name)
#     ax.set_title(f'{metric_name} vs. Number of Clients')
#     ax.grid(True)

# # Save the plot to an image file
# plt.savefig("performance_metrics_plot_test.png")

# Display the plots
# plt.show()


import matplotlib.pyplot as plt

# Data
num_clients = [10, 20, 40, 80, 100, 200, 300, 400]
cpu_utilization = [60, 100, 100, 100, 100, 100, 100, 100]

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(num_clients, cpu_utilization, marker='o', linestyle='-', color='b', label='CPU Utilization')

# Customize the plot
plt.title('CPU Utilization vs. Number of Clients')
plt.xlabel('Number of Clients')
plt.ylabel('CPU Utilization (%)')
plt.grid(True)
plt.legend()

# Show the plot
plt.savefig('cpu_utilisation.png')
plt.show()
