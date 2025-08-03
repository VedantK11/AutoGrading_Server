#pragma once

#include <string>
#include <queue>

int avgCount=0;
double averageRequests=0.0;
void* calculateAverageRequests(void* arg) ;

void* handleClient(void* arg);
std::string compileAndRun(const char* sourceFileName, const std::string& folder);
