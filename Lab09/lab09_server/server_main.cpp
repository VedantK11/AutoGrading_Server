
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

#include "server_main.h"

// Define global variables
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueNotEmpty = PTHREAD_COND_INITIALIZER;
std::queue<ThreadData> requestQueue;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <thread_pool_size>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    int thread_pool_size = std::atoi(argv[2]);

    pthread_t threads[MAX_THREADS];

    // Create worker threads
    for (int i = 0; i < thread_pool_size; i++) {
        if (pthread_create(&threads[i], NULL, handleClient, NULL) != 0) {
            std::cerr << "Failed to create a thread." << std::endl;
            return 1;
        }
    }

  // Create a thread to calculate and display the average request in a queue
    pthread_t averageThread;
    if (pthread_create(&averageThread, NULL, calculateAverage, NULL) != 0) {
        std::cerr << "Failed to create the average thread." << std::endl;
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind error");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        perror("Listen error");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

     while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            perror("Accept error");
            continue;
        }

        // Enqueue the request in the shared queue
        ThreadData threadData;
        threadData.clientSocket = clientSocket;
        threadData.folderName = std::to_string(clientSocket);

        pthread_mutex_lock(&queueMutex);
        requestQueue.push(threadData);    
        pthread_mutex_unlock(&queueMutex);

        // Notify one of the worker threads to pick up the request
        pthread_cond_signal(&queueNotEmpty);

    }
    
     for (int i = 0; i < thread_pool_size; i++) {
        pthread_join(threads[i], NULL);
    }
    close(serverSocket);
    return 0;


}
