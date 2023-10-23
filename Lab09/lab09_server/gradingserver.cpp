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

// Define the maximum number of worker threads in the pool
#define MAX_THREADS 16  //depends on number of cores

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueNotEmpty = PTHREAD_COND_INITIALIZER;

struct ThreadData {
    int clientSocket;
    std::string folderName;
    // You can add more data if needed
};

std::queue<ThreadData> requestQueue;


// Function to compile and execute the source code
std::string compileAndRun(const char* sourceFileName, const std::string& folder) {
    std::string response;

    // Use the folder name in constructing file paths
    std::string compileCommand = "g++ -o " + folder + "/executable " + std::string(sourceFileName) + " > " + folder + "/compile_output.txt 2>&1";
    int compileExitCode = system(compileCommand.c_str());

    if (compileExitCode != 0) {
        std::ifstream compileOutputFile(folder + "/compile_output.txt");
        std::ostringstream compileOutputContent;
        compileOutputContent << compileOutputFile.rdbuf();
        response = "COMPILER ERROR\n" + compileOutputContent.str();
    } else {
        // Execute the compiled program and capture both stdout and stderr
        int runExitCode = system((folder + "/executable > " + folder + "/program_output.txt 2>&1").c_str());

        if (runExitCode != 0) {
            std::ifstream runOutputFile(folder + "/program_output.txt");
            std::ostringstream runOutputContent;
            runOutputContent << runOutputFile.rdbuf();
            response = "RUNTIME ERROR\n" + runOutputContent.str();
        } else {
            // Program executed successfully, compare its output with the expected output
            std::ifstream programOutputFile(folder + "/program_output.txt");
            std::ostringstream programOutputContent;
            programOutputContent << programOutputFile.rdbuf();
            std::string programOutput = programOutputContent.str();

            std::ifstream expectedOutputFile(folder + "/expected_output.txt");
            std::ostringstream expectedOutputContent;
            expectedOutputContent << expectedOutputFile.rdbuf();
            std::string expectedOutput = expectedOutputContent.str();

            if (programOutput == expectedOutput) {
                response = "PASS\n" + programOutput;
            } else {
                // Handle output error
                std::ofstream programOutputFile(folder + "/program_output.txt");
                programOutputFile << programOutput;
                programOutputFile.close();

                system(("diff " + folder + "/program_output.txt " + folder + "/expected_output.txt > " + folder + "/diff_output.txt").c_str());

                std::ifstream diffOutputFile(folder + "/diff_output.txt");
                std::ostringstream diffOutputContent;
                diffOutputContent << diffOutputFile.rdbuf();
                response = "OUTPUT ERROR\n" + programOutput + "\n" + diffOutputContent.str();
            }
        }
    }

    return response;
}

// Function for handling a client connection
void* handleClient(void* arg) {
	while (true) {
        ThreadData threadData;

        // Dequeue a request from the shared queue
        pthread_mutex_lock(&queueMutex);
        while (requestQueue.empty()) {
            pthread_cond_wait(&queueNotEmpty, &queueMutex);
        }
        threadData = requestQueue.front();
        requestQueue.pop();
        pthread_mutex_unlock(&queueMutex);

        int clientSocket = threadData.clientSocket;

        // Process the request as before

        // ...
        
        //std::string folder = threadData.folderName + "_" + std::to_string(gettid());
        std::string folder =  std::to_string(gettid());
        

       

    // Create the thread's folder
    std::string cdfolder = "mkdir " + folder;
    system(cdfolder.c_str());
    std::cout<<folder<<" created."<<std::endl;
    std::string cpfolder = "cp expected_output.txt " + folder + "/";
    system(cpfolder.c_str());

   // while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            //close(clientSocket);
            continue;;
        }

        // Create a temporary source code file in the thread's folder
        std::string tempFileName = folder + "/temp_source.cpp";
        std::ofstream tempFile(tempFileName);
        if (!tempFile.is_open()) {
            close(clientSocket);
            continue;
        }

        tempFile.write(buffer, bytesRead);
        tempFile.close();

        // Process the request (compile and run the temporary file)
        std::string response = compileAndRun(tempFileName.c_str(), folder);

        // Remove the temporary source code file
        remove(tempFileName.c_str());

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.size(), 0);
   // }

    close(clientSocket);

    // Remove the thread's folder
    std::string dfolder = "rm -r " + folder;
    system(dfolder.c_str());
    std::cout<<folder<<" deleted."<<std::endl;
   // return NULL;


     //   close(clientSocket);
    }
    
	/*ThreadData threadData;
        
        // Dequeue a request from the shared queue
        pthread_mutex_lock(&queueMutex);
        while (requestQueue.empty()) {
            pthread_cond_wait(&queueNotEmpty, &queueMutex);
        }
        threadData = requestQueue.front();
        requestQueue.pop();
        pthread_mutex_unlock(&queueMutex);

        int clientSocket = threadData.clientSocket;
        
        std::string folder = threadData.folderName + "_" + std::to_string(gettid());

       

    // Create the thread's folder
    std::string cdfolder = "mkdir " + folder;
    system(cdfolder.c_str());
    std::cout<<folder<<" created."<<std::endl;
    std::string cpfolder = "cp expected_output.txt " + folder + "/";
    system(cpfolder.c_str());

    while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            close(clientSocket);
            break;
        }

        // Create a temporary source code file in the thread's folder
        std::string tempFileName = folder + "/temp_source.cpp";
        std::ofstream tempFile(tempFileName);
        if (!tempFile.is_open()) {
            close(clientSocket);
            continue;
        }

        tempFile.write(buffer, bytesRead);
        tempFile.close();

        // Process the request (compile and run the temporary file)
        std::string response = compileAndRun(tempFileName.c_str(), folder);

        // Remove the temporary source code file
        remove(tempFileName.c_str());

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);

    // Remove the thread's folder
    std::string dfolder = "rm -r " + folder;
    system(dfolder.c_str());
    std::cout<<folder<<" deleted."<<std::endl;
    return NULL;*/
    
   // return NULL;
}

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
    /*for (int i = 0; i < thread_pool_size; i++) {
        pthread_join(threads[i], NULL);
    }*/

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
        std::cout<<"thread.cs = "<<threadData.clientSocket<<"\tcs = "<<clientSocket<<std::endl;
        
        pthread_mutex_lock(&queueMutex);
        requestQueue.push(threadData);
        pthread_mutex_unlock(&queueMutex);

        // Notify one of the worker threads to pick up the request
       
        if (requestQueue.size() <= thread_pool_size ) {
         pthread_cond_signal(&queueNotEmpty);
         }
       /*  pthread_mutex_lock(&queueMutex);
        if (requestQueue.size() >= thread_pool_size) {
            // Wait until the queue size decreases
           // while (requestQueue.size() >= thread_pool_size) {
                pthread_cond_wait(&queueNotEmpty, &queueMutex);
            //}
        }
        pthread_mutex_unlock(&queueMutex);
        */
    }
    
     
    close(serverSocket);
    return 0;
}

