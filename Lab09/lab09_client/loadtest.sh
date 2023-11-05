#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <numClients> <loopNum> <sleepTimeSeconds>"
    exit 1
fi

#Server_Ip="192.168.0.5"
#Server_Ip="10.96.20.169"
#Server_Ip="192.168.1.106"
#Server_Ip="192.168.113.248"
#Server_Ip="10.96.15.177";
# Server_Ip="10.130.171.214"
Server_Ip="127.0.0.1"
portNum="8000"
numClients=$1
loopNum=$2
sleepTimeSeconds=$3
timeOutTime=40

outputDir="client_output"
mkdir -p "$outputDir"

# Initialize variables for CPU utilization
averageCPU=0
totalIdle=0

# Run vmstat to capture CPU utilization metrics
vmstat 1 $((loopNum + 1)) > "$outputDir/vmstat.log" &
#vmstat 10 > "$outputDir/vmstat.log" &
vmstat_pid=$!

# Initialize variables for metrics
# totalRequests=0
successfulRequests=0
cpuUtilization=0
activeThreads=0


for ((i = 1; i <= numClients; i++)); do
    ./gradingclient "$Server_Ip:$portNum" pass.cpp "$loopNum" "$sleepTimeSeconds" "$timeOutTime" > "$outputDir/client_$i.txt" &
    pids[${i}]=$! # Store the PID of each background job
done

activeThreads=$(ps -eLf | grep -w ./gradingserver | awk '{ if(NR==1){ print $6}}')

# Wait for all background jobs to finish
for ((i = 1; i <= numClients; i++)); do
    wait ${pids[i]}
done


# Wait for vmstat to finish and collect data
wait $vmstat_pid

# Parse vmstat output and calculate CPU utilization
vmstat_file="$outputDir/vmstat.log"
lines=0
while read -r line; do
    if [[ "$line" =~ ^[0-9]+ ]]; then
    	let lines=lines+1
    	if [[ $lines -ne 1 ]]; then
		idle=$(echo "$line" | awk '{print $15}')
		totalIdle=$((totalIdle + idle))
	fi
    fi
done < "$vmstat_file"

let lines=lines-1
# Calculate average CPU utilization
averageIdle=$(($totalIdle / lines))
averageCPU=$(echo "100 - $averageIdle" | bc)

# Calculate overall throughput and average response time
throughput=0
totalThroughput=0.0
timeoutRate=0
totalTimeoutRate=0.0
errorRate=0.0
totalErrorRate=0.0
totalResponseTime=0
totalSamples=0

for ((i = 1; i <= numClients; i++)); do
    output_file="$outputDir/client_$i.txt"

    # Calculate throughput (requests per second)
    # clientTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    # floatvalue=0.0
    # result=$(echo "$clientTime > $floatvalue" | bc)
    # if [ $result -eq 1 ]; then
    #      throughput=$(echo "scale=2; 1 / $clientTime" | bc)
    # else
    #      throughput=0
    # fi
    
    # totalThroughput=$(echo "$totalThroughput + $throughput" | bc)

    # Calculate average response time
    avgResponseTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    numSamples=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    totalResponseTime=$(echo "$totalResponseTime + ($avgResponseTime * $numSamples)" | bc)
    totalSamples=$(echo "$totalSamples + $numSamples" | bc)

    # Extract and calculate metrics from gradingclient output
    # # totalRequests=$($totalRequests + $(grep "Total Requests Sent:"  "$output_file" | awk '{print $NF}' ) )
    # successfulRequests=$($successfulRequests + $(grep "Successful Requests (Goodput): " "$output_file" | awk '{print $NF}'))
    
    throughput=$(grep "Throughput:" "$output_file" | awk '{print $(NF-1)}' )
    totalThroughput=$(echo "scale=2; $totalThroughput + $throughput" | bc)
    timeoutRate=$(grep "Timeout Rate:" "$output_file" | awk '{print $(NF-1)}')
    totalTimeoutRate=$(echo "scale=2; $totalTimeoutRate + $timeoutRate" | bc)
    errorRate=$(grep "Error Rate:" "$output_file" | awk '{print $(NF-1)}')
    totalErrorRate=$(echo "scale=2; $totalErrorRate + $errorRate" | bc)
    
    # ... (existing code for other metrics)

    # Measure CPU utilization of gradingclient
    #cpuUtilization=$(echo "scale=2; $cpuUtilization + $(top -b -n 1 -p ${pids[i]} | awk 'NR > 7 { sum += $9 } END {print sum}') / 100" | bc)
done

# timeoutRateRate=$(echo "scale=2; $timeoutRate / $totalRequests" | bc)
# errorRateRate=$(echo "scale=2; $errorRate / $totalRequests" | bc)
# successfulRequestsRate=$(echo "scale=2; $totalThroughput / $numClients" | bc)
RequestsSentRate=$(echo "scale=2; $totalThroughput +$totalTimeoutRate +$totalErrorRate" | bc)

# Calculate the average number of active threads
#activeThreads=$(pgrep gradingserver | wc -l)

# Calculate overall average response time
if [ $totalSamples -gt 0 ]; then
    overallAvgResponseTime=$(echo "scale=2; $totalResponseTime / $totalSamples" | bc)
else
    overallAvgResponseTime=0
fi

# echo "Overall Throughput: $totalThroughput requests/second"
echo "Overall Average Response Time: $overallAvgResponseTime seconds"
echo "Successful Requests Rate(Goodput): $totalThroughput requests/second"
echo "Timeout Requests Rate: $totalTimeoutRate"
echo "Error Requests Rate: $totalErrorRate"
echo "Requests Sent Rate: $RequestsSentRate"
echo "Average Number of Active Threads: $activeThreads"
echo "Overall CPU Utilization: $averageCPU%"

