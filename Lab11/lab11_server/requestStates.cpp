#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <pthread.h>
#include <queue>
#include <chrono>
#include <random>
#include <unordered_map>
#include<mutex>

#include "server_main.h"

void *saveStateToFile(void *arg) {
    while (true)
    {
        std::ofstream stateFile("requestStates.csv");
    for (const auto& entry : requestStates) {
        stateFile << entry.first << "," << entry.second << std::endl;
    }
    stateFile.close();
    sleep(1);
    }
    
}

// Function to load the requestStates map from a file
void loadStateFromFile() {
    std::ifstream stateFile("requestStates.csv");
    if (stateFile.is_open()) {
        std::string line;
        while (std::getline(stateFile, line)) {
            std::istringstream iss(line);
            std::string requestId, state;
            if (std::getline(iss, requestId, ',') && std::getline(iss, state)) {
                requestStates[requestId] = state[0]; // Assuming state is a single character
            }
        }
        stateFile.close();
    }

    for (auto it = requestStates.begin(); it!=requestStates.end(); ++it)
    {
        if (it->second=='W' )
        {
          ThreadData threadData;
            threadData.requestID = it->first;
            threadData.folderName = threadData.requestID;   
            requestQueue.push(threadData);
        }
        else if(it->second=='P')
        {
            ThreadData threadData;
            threadData.requestID = it->first;
            it->second='W';
            threadData.folderName = threadData.requestID;   
            requestQueue.push(threadData);
        }
        
        
    }
    
}
