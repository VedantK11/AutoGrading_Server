#!/bin/bash
# SERVER_IP="10.130.171.214"
#SERVER_IP="192.168.0.5"
SERVER_IP="127.0.0.1"
PORT="8000"
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
done
wait

# Calculate overall throughput and average response time
totalThroughput=0
totalResponseTime=0
totalSuccessfulResponses=0
throughput=0
totalThroughput=0.0

for ((i = 1; i <= numClients; i++)); do
    output_file="$outputDir/client_$i.txt"


    # Calculate average response time
    avgResponseTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    numSuccessfulResponses=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    totalResponseTime=$(echo "$totalResponseTime + ($avgResponseTime * $numSuccessfulResponses)" | bc)
    totalSuccessfulResponses=$(echo "$totalSuccessfulResponses + $numSuccessfulResponses" | bc)
    throughput=$(grep "Throughput:" "$output_file" | awk '{print $(NF-1)}' )
    totalThroughput=$(echo "scale=2; $totalThroughput + $throughput" | bc)

done

if [ -n "$totalSuccessfulResponses" ] && [ "$totalSuccessfulResponses" -gt 0 ]; then
    overallAvgResponseTime=$(echo "scale=2; $totalResponseTime / $totalSuccessfulResponses" | bc)
else
    overallAvgResponseTime=0
fi

echo "Overall Throughput: $totalThroughput requests/second"
echo "Overall Average Response Time: $overallAvgResponseTime seconds"

