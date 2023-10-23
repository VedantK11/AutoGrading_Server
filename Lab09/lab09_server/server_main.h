#pragma once
#define MAX_THREADS 16  // depends on the number of cores

// Declare global variables as extern
extern pthread_mutex_t queueMutex;
extern pthread_cond_t queueNotEmpty;
struct ThreadData {
    int clientSocket;
    std::string folderName;
    // You can add more data if needed
};
extern std::queue<ThreadData> requestQueue;



void* handleClient(void* arg);

int main(int argc, char* argv[]);
