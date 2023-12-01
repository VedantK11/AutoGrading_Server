
# Author's Name

1. Ravi Kumar Choubey
2. Vedant Kalbande


## Directory Structure
```bash
├── Experiments
│   ├── AvgResponseTimeComparison_allVersions.csv
│   ├── AvgResponseTimeComparison.png
│   ├── ComparisonCPU_utilisation.csv
│   ├── ComparisonThroughput.csv
│   ├── cpu_utilisation.png
│   ├── plot3.py
│   └── ThroughptutComparison.png
├── Lab06
│   ├── lab06_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   └── RuntimeErrorCode.cpp
│   ├── lab06_server
│   │   ├── compile_output.txt
│   │   ├── expected_output.txt
│   │   ├── gradingserver.cpp
│   │   ├── program_output.txt
│   │   └── temp_source.cpp
│   └── Performance_experiments_lab06.pdf
├── Lab07
│   ├── Experiments
│   │   ├── Experiment_lab07.pdf
│   │   ├── figure.png
│   │   └── performance_metrics_test_lab07.csv
│   ├── lab07_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp
│   │   ├── loadtest.sh
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   ├── plot.py
│   │   └── RuntimeErrorCode.cpp
│   └── lab07_server
│       ├── compile_output.txt
│       ├── expected_output.txt
│       ├── gradingserver.cpp
│       └── program_output.txt
├── Lab08
│   ├── Experiments
│   │   ├── Experiments_Lab08.pdf
│   │   ├── Figure_1.png
│   │   ├── Figure_2.png
│   │   └── performance_metrics_lab8.csv
│   ├── lab08_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp
│   │   ├── loadtest.sh
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   ├── plot.py
│   │   └── RuntimeErrorCode.cpp
│   └── lab_08_server
│       ├── expected_output.txt
│       └── gradingserver.cpp
├── Lab09
│   ├── Experiments
│   │   ├── Avg_Queue_Request.png
│   │   ├── AvgResponseTimeComparison.csv
│   │   ├── AvgResponseTime_Comparison.png
│   │   ├── ComparisonThroughput.csv
│   │   ├── Experiment_Lab09.pdf
│   │   ├── Figure_1.png
│   │   ├── Figure_2.png
│   │   ├── Figure_3.png
│   │   ├── performance_metrics_lab9.csv
│   │   ├── plot3.py
│   │   └── Throughput_Comparison.png
│   ├── lab09_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp
│   │   ├── loadtest.sh
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   └── RuntimeErrorCode.cpp
│   └── lab09_server
│       ├── expected_output.txt
│       ├── Makefile
│       ├── server.cpp
│       ├── server.h
│       ├── server_main.cpp
│       └── server_main.h
├── Lab10
│   ├── Experiments
│   │   ├── Asynchronous_Server_Design.pdf
│   │   ├── cpu_utilisation.png
│   │   ├── performance_metrics.csv
│   │   └── performance_metrics_plot.png.png
│   ├── lab10_client
│   │   ├── client.cpp
│   │   ├── clientPolling.py
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── loadtest.sh
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   ├── plot.py
│   │   └── RuntimeErrorCode.cpp
│   └── lab10_server
│       ├── expected_output.txt
│       ├── Makefile
│       ├── requestStates.cpp
│       ├── requestStates.h
│       ├── server.cpp
│       ├── server.h
│       ├── server_main.cpp
│       └── server_main.h
├── Lab11
│   ├── Experiments
│   │   ├── cpu_utilisation.png
│   │   ├── performance_metrics.csv
│   │   └── performance_metrics_plot.png
│   ├── lab11_client
│   │   ├── client.cpp
│   │   ├── clientPolling.py
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── loadtest.sh
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   ├── plot.py
│   │   └── RuntimeErrorCode.cpp
│   └── lab11_server
│       ├── expected_output.txt
│       ├── Makefile
│       ├── Parameters.config
│       ├── requestStates.cpp
│       ├── requestStates.csv
│       ├── requestStates.h
│       ├── server.cpp
│       ├── server.h
│       ├── server_main.cpp
│       └── server_main.h
├── presentation
│   └── 23d0366_23m2108_DECS.pdf
└── README.md
```



## Instructions to run each versions
### 1. Version 1
#### a> lab 06
####  CLIENT

##### Compile :- 
```bash
g++ gradingclient.c -o gradingclient
```
##### Run :-
```bash
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded>

```

#### SERVER 
##### Compile :- 
```bash
g++ gradingserver.c -o gradingserver
```
##### Run :-
```bash
./gradingserver <port>

```

#### b> lab 07
####  CLIENT

##### Compile :- 
```bash
g++ gradingclient.c -o gradingclient
```
##### Run :-
```bash
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded> <loopNum> <sleepTimeSeconds>

```

##### Loadtest :-
```bash
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```
#### SERVER 
##### Compile :- 
```bash
g++ gradingserver.c -o gradingserver
```
##### Run :-
```bash
./gradingserver <port>
```

### 2. Version 2
#### a> lab 08
####  CLIENT

##### Compile :- 
```bash
g++ gradingclient.c -o gradingclient
```
##### Run :-
```bash
./gradingclient <serverIP:port>  <sourceCodeFileTobeGraded>  <loopNum> <sleepTimeSeconds> <timeout-seconds>

```
##### Loadtest :-
```bash
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```

#### SERVER 
##### Compile :- 
```bash
g++ gradingserver.c -o gradingserver
```
##### Run :-
```bash
./gradingserver <port>

```

### 3. Version 3
#### a> lab 09
####  CLIENT

##### Compile :- 
```bash
g++ gradingclient.c -o gradingclient
```
##### Run :-
```bash
./gradingclient <serverIP:port>  <sourceCodeFileTobeGraded>  <loopNum> <sleepTimeSeconds> <timeout-seconds>

```
##### Loadtest :-
```bash
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```

#### SERVER 
##### Compile :- 
```bash
make
```
##### Run :-
```bash
./server <port> <thread_pool_size>

```

### 4. Version 4
#### a> lab 10
####  CLIENT

##### Compile :- 
```bash
g++ client.c -o client
```
##### To run only client:-
```bash
./client  <new|status> <serverIP:port> <sourceCodeFileTobeGraded|requestID>

```

##### To run client with polling:-
```bash
python3  clientPolling.py  <serverIP:port> <sourceCodeFile> <polling_interval>

```

##### Loadtest :-
```bash
./loadtest.sh <serverIP:port> <sourceCodeFile>  <num_clients> <polling_interval> 
```

#### SERVER 
##### Compile :- 
```bash
make
```
##### Run :-
```bash
./server <port> <thread_pool_size>

```

### 4. Version 5
#### a> lab 11
####  CLIENT

##### Compile :- 
```bash
g++ client.c -o client
```
##### To run only client:-
```bash
./client  <new|status> <serverIP:port> <sourceCodeFileTobeGraded|requestID>

```

##### To run client with polling:-
```bash
python3  clientPolling.py  <serverIP:port> <sourceCodeFile> <polling_interval>

```

##### Loadtest :-
```bash
./loadtest.sh <serverIP:port> <sourceCodeFile>  <num_clients> <polling_interval> 
```

#### SERVER 
##### Compile :- 
```bash
make
```
##### Run :-
```bash
./server <port> <thread_pool_size>  <config_file>
```