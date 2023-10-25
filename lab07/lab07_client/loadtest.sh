#!/bin/bash
SERVER_IP="10.130.171.214"
#SERVER_IP="192.168.0.5"
PORT="8888"
SOURCE_FILE="pass.cpp"

if [ $# -ne 3 ]; then
    echo "Usage: $0 <numClients> <loopNum> <sleepTimeSeconds>"
    exit 1
fi

numClients=$1
loopNum=$2
sleepTimeSeconds=$3

# Create a directory to store client output files
outputDir="client_output"
mkdir -p "$outputDir"

# Start clients in the background and store their PIDs
pids=()
for ((i = 1; i <= numClients; i++)); do
    ./gradingclient "$SERVER_IP:$PORT" "$SOURCE_FILE" "$loopNum" "$sleepTimeSeconds" > "$outputDir/client_$i.txt" &
   # pids+=($!)
done
wait

# Wait for all clients to finish
#for pid in "${pids[@]}"; do
 #   wait "$pid"
#done

# Calculate overall throughput and average response time
totalThroughput=0
totalResponseTime=0
totalSamples=0

for ((i = 1; i <= numClients; i++)); do
    output_file="$outputDir/client_$i.txt"

    # Calculate throughput (number of responses / total time)
#    numResponses=$(grep "Response successful: PASS" "$output_file" | wc -l)
    #numSamples=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    #clientTime=$(grep "Time Taken for Completing the Loop:" "$output_file" | awk '{print $(NF-1)}')
    clientTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    floatvalue=0.0
    result=$(echo "$clientTime > $floatvalue" | bc)
    if [ $result -eq 1 ]; then
         throughput=$(echo "scale=2; 1 / $clientTime" | bc)
    else
         throughput=0
    fi
    totalThroughput=$(echo "$totalThroughput + $throughput" | bc)

    # Calculate average response time
    avgResponseTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    numSamples=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    totalResponseTime=$(echo "$totalResponseTime + ($avgResponseTime * $numSamples)" | bc)
    totalSamples=$(echo "$totalSamples + $numSamples" | bc)
done

# Calculate overall average response time
#if [ $totalSamples -gt 0 ]; then
if [ -n "$totalSamples" ] && [ "$totalSamples" -gt 0 ]; then
    overallAvgResponseTime=$(echo "scale=2; $totalResponseTime / $totalSamples" | bc)
    #totalThroughput=$(echo "scale=2; 1/$overallAvgResponseTime" | bc)
else
    overallAvgResponseTime=0
fi

echo "Overall Throughput: $totalThroughput requests/second"
echo "Overall Average Response Time: $overallAvgResponseTime seconds"

