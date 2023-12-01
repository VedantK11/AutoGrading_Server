#pragma once

#include <string>
#include <queue>


// Variable to store the start time
int avgCount=0;
double averageRequests=0.0;
void* calculateAverageRequests(void* arg) ;

void* handleClient(void* arg);
std::string compileAndRun(const char* sourceFileName, const std::string& folder);
