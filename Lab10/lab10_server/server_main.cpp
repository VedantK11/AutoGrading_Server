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

//  global variables
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueNotEmpty = PTHREAD_COND_INITIALIZER;

//variable to generate unique request ID
std::mutex countMutex;
long int count=0;

std::queue<ThreadData> requestQueue;
std::unordered_map<std::string, char> requestStates; // char: 'W', 'P', or 'C'

// Function to generate a unique request ID
std::string generateRequestID()
{
    std::lock_guard<std::mutex> lock(countMutex);
    count++;
    return std::to_string(count);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <thread_pool_size>" << std::endl;
        return 1;
    }

    std::string msg;
    int port = std::atoi(argv[1]);
    int thread_pool_size = std::atoi(argv[2]);

    pthread_t threads[MAX_THREADS];

    // Create worker threads
    for (int i = 0; i < thread_pool_size; i++)
    {
        if (pthread_create(&threads[i], NULL, handleClient, NULL) != 0)
        {
            std::cerr << "Failed to create a thread." << std::endl;
            return 1;
        }
    }

     // Load the previous requests and their state from the file
    loadStateFromFile();

    // Create a thread to calculate and display the average
    pthread_t averageThread;
    if (pthread_create(&averageThread, NULL, calculateAverageRequests, NULL) != 0)
    {
        std::cerr << "Failed to create the average thread." << std::endl;
        return 1;
    }

    pthread_t requestStatesThread;
    if (pthread_create(&requestStatesThread, NULL, saveStateToFile, NULL) != 0)
    {
        std::cerr << "Failed to create the average thread." << std::endl;
        return 1;
    }

   

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Bind error");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        perror("Listen error");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1)
        {
            perror("Accept error");
            continue;
        }

        // Read the operation type from the client (new or status)
        char operation[10];
        memset(operation,0,sizeof(operation));
        recv(clientSocket, operation, sizeof(operation), 0);
        // std::cout<<operation<<std::endl;

        if (strncmp(operation, "new",3) == 0)
        {
            // std::cout<<"***************in new*****************"<<std::endl;
            
            send(clientSocket,"Send the file",sizeof("Send the file"),0);
            
            // Enqueue the request in the shared queue
            ThreadData threadData;
            // threadData.clientSocket = clientSocket;
            threadData.requestID = generateRequestID();
            threadData.folderName = threadData.requestID;

            // Add the request ID entry in the table with initial state 'W'
            pthread_mutex_lock(&queueMutex);
            requestStates[threadData.requestID] = 'W';
            pthread_mutex_unlock(&queueMutex);
            // std::cout<<"***************File not recv *****************"<<std::endl;
            
            // Receive and save the file with the request ID as its name
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            // std::cout << "File received for grading\n";
        
            std::string file;
            std::string folder="mkdir "+threadData.folderName;
            system(folder.c_str());
            file = threadData.folderName + "/" + threadData.requestID + ".cpp";
            pthread_mutex_lock(&fileMutex);
            std::ofstream sourceCodeFile(file);
            // std::cout<<"***************File recv *****************"<<std::endl;
            // sourceCodeFile.write(buffer, bytesRead);
            // std::cout<<"++++++"<<buffer<<"++++"<<std::endl;
            sourceCodeFile<<buffer;
            sourceCodeFile.close();
            pthread_mutex_unlock(&fileMutex);

            msg = "Your request is accepted and is being processed for grading. Request Id is: "+threadData.requestID ;
            send(clientSocket, msg.c_str(), msg.size(), 0);

            // Enqueue the request in the shared queue
            pthread_mutex_lock(&queueMutex);
            requestQueue.push(threadData);
            pthread_mutex_unlock(&queueMutex);
            // std::cout<<"***************new req responded *****************"<<std::endl;

            // Notify one of the worker threads to pick up the request
        }
        else if (strncmp(operation, "status",6) == 0)
        {
            // std::cout<<"#################In status################"<<std::endl;
            send(clientSocket,"Send the requestId",sizeof("Send the requestId"),0);
            //  std::cout<<"#################asked for request Id################"<<std::endl;

            // Read the Reqeust Id from the client
            char buffer[1024];
            memset(buffer,0,sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);
            std::string requestID = std::string(buffer);
            // std::cout<<requestID<<std::endl;
            auto stateIter = requestStates.find(requestID);
            if (stateIter != requestStates.end())
            {
                char state = stateIter->second;
                if (state == 'C')
                {

                    // Read the output from the log and send it to the client
                    std::string filePath;
                    std::stringstream file;
                    filePath = stateIter->first + "/" + stateIter->first + "_log.txt";
                    std::ifstream outpufile(filePath);
                    file << outpufile.rdbuf();
                    outpufile.close();
                    msg = "Your grading request ID " + stateIter->first + " processing is done, here are the results:\n" + file.str();
                    send(clientSocket, msg.c_str(), msg.size(), 0);

                    // Remove the entry from the table and delete the table
                    std::string folderdelete="rm -r " +stateIter->first;
                    system(folderdelete.c_str());
                    requestStates.erase(stateIter);
                }
                else if (state == 'W')
                {
                    // Send the appropriate message based on the state
                    msg = "Your grading request ID " + stateIter->first + " has been accepted. It is currently in the queue.";
                    send(clientSocket, msg.c_str(), msg.size(), 0);
                }
                else if (state == 'P')
                {
                    msg = "Your grading request ID " + stateIter->first + " has been accepted and is currently being processed.";
                    send(clientSocket, msg.c_str(), msg.size(), 0);
                }
            }
            else
            {
                // Send appropriate message for request not found
                msg = "Your grading request ID " + requestID + " not found. Please check and resend your request ID or re-send your original grading request.";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
            // std::cout<<"################# status Gaya################"<<std::endl;

        }

        // Notify one of the worker threads to pick up the request
        // if (requestQueue.size() <= thread_pool_size)
        // {
        //     pthread_cond_signal(&queueNotEmpty);
        // }
         pthread_cond_signal(&queueNotEmpty);
    }
    close(serverSocket);
    return 0;
}

