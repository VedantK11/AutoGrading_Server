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
totalSuccessfulResponses=0
throughput=0
totalThroughput=0.0

for ((i = 1; i <= numClients; i++)); do
    output_file="$outputDir/client_$i.txt"

    # Calculate throughput (number of responses / total time)
#    numResponses=$(grep "Response successful: PASS" "$output_file" | wc -l)
    #numSuccessfulResponses=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    #clientTime=$(grep "Time Taken for Completing the Loop:" "$output_file" | awk '{print $(NF-1)}')
    # clientTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    # floatvalue=0.0
    # result=$(echo "$clientTime > $floatvalue" | bc)
    # if [ $result -eq 1 ]; then
    #      throughput=$(echo "scale=2; 1 / $clientTime" | bc)
    # else
    #      throughput=0
    # fi

   
    # Calculate average response time
    avgResponseTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    numSuccessfulResponses=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    totalResponseTime=$(echo "$totalResponseTime + ($avgResponseTime * $numSuccessfulResponses)" | bc)
    totalSuccessfulResponses=$(echo "$totalSuccessfulResponses + $numSuccessfulResponses" | bc)
    throughput=$(grep "Throughput:" "$output_file" | awk '{print $(NF-1)}' )
    totalThroughput=$(echo "scale=2; $totalThroughput + $throughput" | bc)

done

# Calculate overall average response time
#if [ $totalSuccessfulResponses -gt 0 ]; then
if [ -n "$totalSuccessfulResponses" ] && [ "$totalSuccessfulResponses" -gt 0 ]; then
    overallAvgResponseTime=$(echo "scale=2; $totalResponseTime / $totalSuccessfulResponses" | bc)
    #totalThroughput=$(echo "scale=2; 1/$overallAvgResponseTime" | bc)
else
    overallAvgResponseTime=0
fi

echo "Overall Throughput: $totalThroughput requests/second"
echo "Overall Average Response Time: $overallAvgResponseTime seconds"

