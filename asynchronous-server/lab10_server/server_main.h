
#pragma once
#include <unordered_map>

#define MAX_THREADS 16  // depends on the number of cores

// Declare global variables as extern
extern pthread_mutex_t fileMutex ;
extern pthread_mutex_t queueMutex ;
extern pthread_cond_t queueNotEmpty;


struct ThreadData
{
    std::string folderName;
    std::string requestID;
};
extern std::queue<ThreadData> requestQueue;
extern std::unordered_map<std::string, char> requestStates; // char: 'W', 'P', or 'C'


void* handleClient(void* arg);
void* calculateAverageRequests(void* arg) ;

void *saveStateToFile(void *arg);
void loadStateFromFile() ;

std::string generateRequestID();
int main(int argc, char* argv[]);
