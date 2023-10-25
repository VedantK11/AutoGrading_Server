// server.h

#pragma once

#include <string>
#include <queue>

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

// Variable to store the start time
int avgCount=0;
double averageRequests=0.0;
void* calculateAverage(void* arg) ;

void* handleClient(void* arg);
std::string compileAndRun(const char* sourceFileName, const std::string& folder);
