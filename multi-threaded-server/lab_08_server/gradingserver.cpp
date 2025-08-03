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
#include <chrono>
#include <thread>

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData {
    int clientSocket;
    unsigned long threadId;
    std::string folderName;
};

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
    ThreadData* threadData = (ThreadData*)arg;
    int clientSocket = threadData->clientSocket;
    std::string folder=std::to_string(threadData->threadId);

    // Create the thread's folder
    std::string cdfolder = "mkdir " + folder;
    system(cdfolder.c_str());
    std::string cpfolder = "cp expected_output.txt " + folder + "/";
    system(cpfolder.c_str());

        std::string tempFileName = folder + "/temp_source.cpp";
       
         //receive file size
        size_t fileSize;
        ssize_t sizeReceived = recv(clientSocket, &fileSize, sizeof(fileSize), 0);

        if (sizeReceived != sizeof(fileSize))
        {
            std::cerr << "Error receiving file size" << std::endl;
            send(clientSocket, "Error receiving file size", sizeof("Error receiving file size"), 0);
            close(clientSocket);
            return NULL;
        }
        else{
            send(clientSocket, "Send the file", sizeof("Send the file"), 0);
        }

        
        // recieve file

        std::ofstream outputFile(tempFileName);
        if (!outputFile.is_open())
        {
            std::cerr << "Error opening file for writing." << std::endl;
            close(clientSocket);
            return NULL ;
        }

        const int bufferSize = 1024;
        char buf[bufferSize];
        size_t remainingBytes = fileSize;

        while (remainingBytes > 0)
        {
            int bytesReceived = recv(clientSocket, buf, std::min(static_cast<size_t>(bufferSize), remainingBytes), 0);

            if (bytesReceived <= 0)
            {
                std::cerr << "Error receiving data" << std::endl;
                break;
            }

            outputFile.write(buf, bytesReceived);
            remainingBytes -= bytesReceived;
        }

        outputFile.close();

        // Process the request (compile and run the temporary file)
        std::string response = compileAndRun(tempFileName.c_str(), folder);

        // Remove the temporary source code file
        remove(tempFileName.c_str());

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.size(), 0);
    //}

    close(clientSocket);

    // Remove the thread's folder
    std::string dfolder = "rm -r " + folder;
    system(dfolder.c_str());
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

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

    if (listen(serverSocket, 5) == -1) {
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

        ThreadData* threadData = (ThreadData*)malloc(sizeof(ThreadData));
        threadData->clientSocket = clientSocket;

        pthread_t thread;
        threadData->threadId = thread;
        std::cout << "Thread id: " << thread << " client socket id: " << clientSocket << " folder name: " << threadData->threadId << std::endl;

        if (pthread_create(&thread, NULL, handleClient, threadData) != 0) {
            std::cerr << "Failed to create a thread for the client." << std::endl;
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}
