
if [ $# -ne 3 ]; then
    echo "Usage: $0 <numClients> <loopNum> <sleepTimeSeconds>"
    exit 1
fi

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
successfulRequests=0
cpuUtilization=0
activeThreads=0


for ((i = 1; i <= numClients; i++)); do
    ./gradingclient "$Server_Ip:$portNum" pass.cpp "$loopNum" "$sleepTimeSeconds" "$timeOutTime" > "$outputDir/client_$i.txt" &
    # pids[${i}]=$! # Store the PID of each background job
done

s_pid=$(pgrep server)
starttime=$(date +%s)
echo "Average number of threads : " > nlwp.txt
# echo "Average CPU Utilisation : " > cput.txt
nlwp=$(ps -T -p $s_pid | wc -l)
nlwp=`expr $nlwp - 3`
echo $nlwp
echo $nlwp >> nlwp.txt
# v_pid=$(pgrep vmstat)

echo $nlwp
less=$(ps aux | grep -i "./gradingclient 127.0" | wc -l)

while [[ $less -gt 1 ]];
    do 
    endtime=$(date +%s)
    diff=`expr $endtime - $starttime`

    if [[ $diff -gt 1 ]]; then
        starttime=$(date +%s)
        echo "Average number of threads : " >> nlwp.txt
        nlwp=$(ps -T -p $s_pid | wc -l)
        nlwp=`expr $nlwp - 3`
        echo $nlwp >> nlwp.txt
    fi
    less=$(ps aux | grep -i "./gradingclient 127.0" | wc -l)
done

# kill -9 $v_pid
echo "$nlwp"


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
successfullthroughput=0
totalsuccessfullThroughput=0.0
totalThroughput=0.0
timeoutRate=0
totalTimeoutRate=0.0
errorRate=0.0
totalErrorRate=0.0
totalResponseTime=0
totalSamples=0

for ((i = 1; i <= numClients; i++)); do
    output_file="$outputDir/client_$i.txt"

       # Calculate average response time
    avgResponseTime=$(grep "Average Response Time:" "$output_file" | awk '{print $(NF-1)}')
    numSamples=$(grep "Number of Successful Responses:" "$output_file" | awk '{print $NF}')
    totalResponseTime=$(echo "$totalResponseTime + ($avgResponseTime * $numSamples)" | bc)
    totalSamples=$(echo "$totalSamples + $numSamples" | bc)
 
    successfullthroughput=$(grep "Throughput:" "$output_file" | awk '{print $(NF-1)}')
    totalsuccessfullThroughput=$(echo "scale=2; $totalsuccessfullThroughput +$successfullthroughput" | bc)
     
    timeoutRate=$(grep "Timeout Rate:" "$output_file" | awk '{print $(NF-1)}')
    totalTimeoutRate=$(echo "scale=2; $totalTimeoutRate + $timeoutRate" | bc)
    errorRate=$(grep "Error Rate:" "$output_file" | awk '{print $(NF-1)}')
    totalErrorRate=$(echo "scale=2; $totalErrorRate + $errorRate" | bc)
    
done

RequestsSentRate=$(echo "scale=2; $totalsuccessfullThroughput +$totalTimeoutRate +$totalErrorRate" | bc)
totalThroughput=$(echo "scale=2; $totalsuccessfullThroughput +$totalTimeoutRate +$totalErrorRate" | bc)

# Calculate overall average response time
if [ $totalSamples -gt 0 ]; then
    overallAvgResponseTime=$(echo "scale=2; $totalResponseTime / $totalSamples" | bc)
else
    overallAvgResponseTime=0
fi

lwp=$(cat nlwp.txt | grep -E "[0-9]+")
# avg_cpu_ut=0
avg_nlwp=0

it=0
for i in $lwp
    do
    avg_nlwp=`expr $i + $avg_nlwp`
    it=`expr $it + 1`
done
avg_nlwp=$(echo "scale=3; $avg_nlwp / $it" | bc)
activeThreads=$avg_nlwp

echo "Overall Average Response Time: $overallAvgResponseTime seconds"
echo "Overall Throughput: $totalThroughput requests/second"
echo "Successful Requests Rate(Goodput): $totalsuccessfullThroughput requests/second"
echo "Timeout Requests Rate: $totalTimeoutRate"
echo "Error Requests Rate: $totalErrorRate"
echo "Requests Sent Rate: $RequestsSentRate"
echo "Average Number of Active Threads: $activeThreads"
echo "Overall CPU Utilization: $averageCPU%"

