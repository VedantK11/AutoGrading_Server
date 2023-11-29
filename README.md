
# Author's Name

1. Ravi Kumar Choubey
2. Vedant Kalbande


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
g++ gradingclient.c -o client
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
g++ gradingclient.c -o client
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


##GitHub Link: https://github.com/VedantK11/Project_DECS_SERVER






## Repo Details and Author's coding work
```bash
├── Lab06
│   ├── lab06_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp                --> Vedant
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   └── RuntimeErrorCode.cpp
│   ├── lab06_server
│   │   ├── expected_output.txt
│   │   └── gradingserver.cpp                 --> Ravi
│   └── Performance_experiments_lab06.pdf
├── lab07
│   ├── Experiment_lab07.pdf
│   ├── Figure.png
│   ├── lab07_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp                   --> Ravi
│   │   ├── loadtest.sh                         --> Ravi & Vedant
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   ├── plot.py
│   │   └── RuntimeErrorCode.cpp
│   ├── lab07_server
│   │   ├── expected_output.txt
│   │   └── gradingserver.cpp                  --> Vedant
│   └── performance_metrics_test_lab07.csv
├── Lab08
│   ├── Experiments_Lab08.pdf
│   ├── Figure_1.png
│   ├── Figure_2.png
│   ├── lab08_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp                    --> Vedant
│   │   ├── loadtest.sh                          --> Vedant
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   └── RuntimeErrorCode.cpp
│   ├── lab_08_server
│   │   ├── expected_output.txt
│   │   └── gradingserver.cpp                   --> Ravi
│   └── performance_metrics_lab8.csv
├── Lab09
│   ├── Experiment_Lab09.pdf
│   ├── Experiments
│   │   ├── Avg_Queue_Request.png
│   │   ├── AvgResponseTimeComparison.csv
│   │   ├── AvgResponseTime_Comparison.png
│   │   ├── ComparisonThroughput.csv
│   │   ├── Figure_1.png
│   │   ├── Figure_2.png
│   │   ├── performance_metrics.csv
│   │   └── Throughput_Comparison.png
│   ├── lab09_client
│   │   ├── CompileTimeErrorCode.cpp
│   │   ├── gradingclient.cpp                      --> Vedant
│   │   ├── loadtest.sh                             --> Vedant
│   │   ├── OutputErrorCode.cpp
│   │   ├── pass.cpp
│   │   └── RuntimeErrorCode.cpp
│   └── lab09_server
│       ├── expected_output.txt
│       ├── Makefile                            --> Ravi
│       ├── server.cpp                           --> Ravi    
│       ├── server.h                            --> Ravi
│       ├── server_main.cpp                     --> Vedant
│       ├── server_main.h                        --> Vedant
└── README.md      
```