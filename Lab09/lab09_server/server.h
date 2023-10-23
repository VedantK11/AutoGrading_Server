// server.h

#pragma once

#include <string>
#include <queue>

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

void* handleClient(void* arg);
std::string compileAndRun(const char* sourceFileName, const std::string& folder);
