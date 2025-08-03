
# Scalable Multi-Threaded Autograding Server with Asynchronous Processing and Load Testing Framework
A comprehensive multi-version server implementation demonstrating the evolution from single-threaded to asynchronous architecture for automated code compilation, execution, and grading.

## Architecture Evolution

This project implements 5 progressive server versions, each addressing specific scalability and performance challenges:

| Version | Architecture | Key Features |
|---------|-------------|--------------|
| **V1** | Single-Threaded | Baseline implementation with basic client-server model |
| **V2** | Load Testing Framework | Multi-client simulation and performance measurement |
| **V3** | Multi-Threaded (Create-Destroy) | Dynamic thread creation per request |
| **V4** | Thread Pool | Controlled resource management with worker threads |
| **V5** | Asynchronous Processing | Asynchronous handling of client requests|
| **V5+** | Asynchronous Processing | Non-blocking request handling with persistent storage and security|

## Directory Structure

```bash
├── Experiments/                     # Comprehensive performance comparison
│   ├── AvgResponseTimeComparison_allVersions.csv
│   ├── AvgResponseTimeComparison.png
│   ├── ComparisonCPU_utilisation.csv
│   ├── ComparisonThroughput.csv
│   ├── cpu_utilisation.png
│   ├── plot.py
│   └── ThroughptutComparison.png
├── Lab06/                          # Version 1: Single-Threaded Server
│   ├── lab06_client/
│   ├── lab06_server/
│   └── Performance_experiments_lab06.pdf
├── Lab07/                          # Version 2: Load Testing Framework
│   ├── Experiments/
│   ├── lab07_client/
│   └── lab07_server/
├── Lab08/                          # Version 3: Multi-Threaded (Create-Destroy)
│   ├── Experiments/
│   ├── lab08_client/
│   └── lab_08_server/
├── Lab09/                          # Version 4: Thread Pool Architecture
│   ├── Experiments/
│   ├── lab09_client/
│   └── lab09_server/
├── Lab10/                          # Version 5: Asynchronous Server
│   ├── Experiments/
│   ├── lab10_client/
│   └── lab10_server/
├── Lab11/                          # Version 5+: Enhanced Asynchronous with Security
│   ├── Experiments/
│   ├── lab11_client/
│   └── lab11_server/
├── presentation/
│   └── DECS.pdf
└── README.md
```

## Features

### Core Functionality
- **Automated Code Grading**: Compiles, executes, and validates C/C++ programs
- **Multi-Version Architecture**: Progressive complexity from single-threaded to asynchronous
- **Comprehensive Error Handling**: Compiler errors, runtime errors, and output mismatches
- **Performance Monitoring**: Throughput, response time, CPU utilization tracking

### Technical Highlights
- **Thread Pool Management**: Efficient worker thread allocation and resource control
- **Asynchronous Request Processing**: Non-blocking architecture with request ID tracking
- **Persistent Storage**: File-based result storage for server restart resilience  
- **Load Testing Framework**: Multi-client simulation with configurable parameters
- **Performance Analysis**: Queuing theory validation and comparative benchmarking

## 🛠️ Technologies Used

- **C++**: Core server and client implementation
- **Multithreading**: pthreads for concurrent request handling
- **Bash Scripting**: Load testing automation and performance measurement
- **Python**: Data analysis, plotting, and client polling mechanisms
- **System Programming**: Socket programming, process management, file I/O

##  Quick Start Guide

### Prerequisites
```bash
sudo apt-get install build-essential python3
```

##  Detailed Usage Instructions

### Version 1: Single-Threaded Server (Lab06)

**Basic single-threaded implementation for baseline performance**

#### Client
```bash
# Compile
g++ gradingclient.cpp -o gradingclient

# Run
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded>
```

#### Server
```bash
# Compile
g++ gradingserver.cpp -o gradingserver

# Run
./gradingserver <port>
```

---

### Version 2: Load Testing Framework (Lab07)

**Adds performance measurement capabilities with multi-client simulation**

#### Client
```bash
# Compile
g++ gradingclient.cpp -o gradingclient

# Run single client
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded> <loopNum> <sleepTimeSeconds>

# Run load test
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```

#### Server
```bash
# Compile
g++ gradingserver.cpp -o gradingserver

# Run
./gradingserver <port>
```

---

### Version 3: Multi-Threaded Server (Lab08)

**Implements dynamic thread creation with timeout support**

#### Client
```bash
# Compile
g++ gradingclient.cpp -o gradingclient

# Run
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded> <loopNum> <sleepTimeSeconds> <timeout-seconds>

# Load test
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```

#### Server
```bash
# Compile
g++ gradingserver.cpp -o gradingserver

# Run
./gradingserver <port>
```

---

### Version 4: Thread Pool Architecture (Lab09)

**Controlled thread management with request queuing**

#### Client
```bash
# Compile
g++ gradingclient.cpp -o gradingclient

# Run
./gradingclient <serverIP:port> <sourceCodeFileTobeGraded> <loopNum> <sleepTimeSeconds> <timeout-seconds>

# Load test
./loadtest.sh <numClients> <loopNum> <sleepTimeSeconds>
```

#### Server
```bash
# Compile
make

# Run
./server <port> <thread_pool_size>
```

---

### Version 5: Asynchronous Server (Lab10)

**Non-blocking request processing with persistent storage**

#### Client
```bash
# Compile
g++ client.cpp -o client

# Submit new request
./client new <serverIP:port> <sourceCodeFileTobeGraded>

# Check request status
./client status <serverIP:port> <requestID>

# Run with automatic polling
python3 clientPolling.py <serverIP:port> <sourceCodeFile> <polling_interval>

# Load test
./loadtest.sh <serverIP:port> <sourceCodeFile> <num_clients> <polling_interval>
```

#### Server
```bash
# Compile
make

# Run
./server <port> <thread_pool_size>
```

---

### Version 5+: Enhanced Asynchronous (Lab11)

**Adds security features and resource limitations**

#### Client
```bash
# Compile
g++ client.cpp -o client

# Submit new request
./client new <serverIP:port> <sourceCodeFileTobeGraded>

# Check request status
./client status <serverIP:port> <requestID>

# Run with automatic polling
python3 clientPolling.py <serverIP:port> <sourceCodeFile> <polling_interval>

# Load test
./loadtest.sh <serverIP:port> <sourceCodeFile> <num_clients> <polling_interval>
```

#### Server
```bash
# Compile
make

# Run
./server <port> <thread_pool_size> <config_file>
```

## Server Response Types

- **PASS**: Program executed successfully with correct output
- **COMPILER ERROR**: Compilation failed (includes compiler output)
- **RUNTIME ERROR**: Program crashed during execution
- **OUTPUT ERROR**: Program ran but produced incorrect output (includes diff)

## Performance Analysis

### Key Metrics Measured
- **Goodput vs Number of Clients**
- **Average Response Time**
- **CPU Utilization**
- **Request Queue Length**
- **Thread Pool Efficiency**

### Test Programs Available
- `pass.cpp` - Correct program that should pass
- `CompileTimeErrorCode.cpp` - Program with compilation errors
- `RuntimeErrorCode.cpp` - Program that crashes during execution
- `OutputErrorCode.cpp` - Program with incorrect output

## Experimental Results

Our comprehensive performance analysis in the `Experiments/` directory includes:
- Cross-version performance comparisons
- CPU utilization analysis
- Response time characteristics
- Throughput measurements
- Queue theory validation

Key findings demonstrate progressive improvements:
- **Single-threaded**: Baseline performance
- **Multi-threaded**: Better concurrency but resource contention
- **Thread Pool**: Optimal resource utilization
- **Asynchronous**: Best performance under variable loads

## Usage Examples

### Basic Grading Flow
```bash
# Version 1-3 (Synchronous)
./gradingclient localhost:8080 hello-world.cpp
# Response: "PASS" or error details immediately

# Version 4-5 (Asynchronous)
./client new localhost:8080 hello-world.cpp
# Response: "Your grading request ID 12345 has been accepted..."

./client status localhost:8080 12345
# Response: "Your grading request ID 12345 processing is done, PASS"
```

### Load Testing Example
```bash
# Test with 20 concurrent clients, 10 requests each, 1 second between requests
./loadtest.sh 20 10 1

# Generates performance metrics and plots
```

##  Implementation Details

### Asynchronous Architecture (Lab10/11)
- **Request ID Management**: Unique identifier generation and tracking
- **State Management**: Request states (Waiting, Processing, Complete)
- **Persistent Storage**: File-based result storage with CSV tracking
- **Polling Mechanism**: Client-side status checking with configurable intervals

### Security Features (Lab11)
- **Resource Limits**: Configurable via `Parameters.config`
- **Safe Execution**: Controlled environment for student code
- **Process Isolation**: Secure compilation and execution

## Acknowledgments

This project was developed as part of CS744: Design and Engineering of Computing Systems at IIT Bombay under the guidance of Prof. Varsha Apte

## Authors

All authors contributed equally in this project

---

*This project showcases the evolution from basic client-server architecture to enterprise-level asynchronous processing systems, with comprehensive performance analysis validating theoretical predictions.*
<!-- 
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
``` -->