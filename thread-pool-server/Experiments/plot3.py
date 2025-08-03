# import matplotlib.pyplot as plt

# # Data
# num_clients = [10, 20, 40, 80, 100, 200]
# version1 = [1.09, 1.4, 6.79, 9.38, 10.12, 11.91]
# version2 = [1.49, 2.67, 5.12, 10.37, 13.34, 25.47]
# version3 = [1.33, 0.98, 1.69, 3.73, 4.62, 9.29]

# # Plotting
# plt.figure(figsize=(10, 6))
# plt.plot(num_clients, version1, marker='o', label='Version 1')
# plt.plot(num_clients, version2, marker='s', label='Version 2')
# plt.plot(num_clients, version3, marker='^', label='Version 3')

# # Customize the plot
# plt.title('Average Response Time Comparison')
# plt.xlabel('Number of Clients')
# plt.ylabel('Average Response Time (seconds)')
# plt.grid(True)
# plt.legend()

# # Show the plot
# plt.show()


# import matplotlib.pyplot as plt

# # Data
# num_clients = [10, 20, 40, 80, 100, 200]
# version1 = [4.555996, 5.040966, 6.0332023, 9.54905349, 9.81251234, 11.02214359]
# version2 = [4.001521, 5.443122, 6.547793, 7.0229534, 6.9510808, 7.5346308]
# version3 = [4.280308, 10.155241, 15.012177, 17.285337, 18.237232, 20.0442716]

# # Plotting
# plt.figure(figsize=(10, 6))
# plt.plot(num_clients, version1, marker='o', label='Version 1')
# plt.plot(num_clients, version2, marker='s', label='Version 2')
# plt.plot(num_clients, version3, marker='^', label='Version 3')

# plt.xlabel('Number of Clients')
# plt.ylabel('Throughput')
# plt.title('Comparison of Throughput of Different Versions')
# plt.grid(True)
# plt.legend()
# plt.show()
import matplotlib.pyplot as plt

# Data
num_clients = [10, 20, 40, 80, 100, 200, 300, 400]
# avg_requests = [1.07, 1.08, 7.13, 28.04, 38.75, 83.02, 133.36, 189.32]
avg_threads = [16, 16, 16, 16, 16, 16, 16, 16]

# Plotting
plt.figure(figsize=(10, 6))
# plt.plot(num_clients, avg_requests, marker='o', linestyle='-', color='b')
plt.plot(num_clients, avg_threads, marker='s', linestyle='-', color='r')

plt.xlabel('Number of Clients')
# plt.ylabel('Average Requests in Queue')
plt.ylabel('Average Threads')
# plt.title('Number of Clients vs. Average Requests in Queue')
plt.title('Number of Clients vs. Average number of Threads')
plt.grid(True)
plt.show()
