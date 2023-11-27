
#pragma once
#include <unordered_map>

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

extern ssize_t MAX_REQUEST_QUEUE_SIZE;
extern ssize_t FILE_SIZE_LIMIT;
extern int MAX_THREADS;

void* handleClient(void* arg);
void* calculateAverageRequests(void* arg) ;

void *saveStateToFile(void *arg);
void loadStateFromFile() ;

std::string generateRequestID();
bool readConfig(const std::string &configFile);
int main(int argc, char* argv[]);
