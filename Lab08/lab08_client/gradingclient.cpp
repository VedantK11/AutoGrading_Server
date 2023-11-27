#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>   // For gettimeofday
#include <sys/select.h> // For select
#include <errno.h>      // For errno

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <serverIP:port>  <sourceCodeFileTobeGraded>  <loopNum> <sleepTimeSeconds> <timeout-seconds>" << std::endl;
        return 1;
    }

    std::string serverIPPort = argv[1];
    std::string sourceFileName = argv[2];
    int loopNum = std::atoi(argv[3]);
    int sleepTimeSeconds = std::atoi(argv[4]);
    int timeoutSeconds = std::atoi(argv[5]);

    int successfulResponses = 0;
    double responseTime = 0;
    int totalRequests = loopNum;
    // int successfulRequests = 0;
    int timeoutRequests = 0;
    int errorRequests = 0;
    double successful_totalTime = 0.0;
    double totalTime = 0.0;

    // Extract server IP and port from the command line argument
    size_t colonPos = serverIPPort.find(':');
    if (colonPos == std::string::npos) {
        std::cerr << "Invalid server IP:port format" << std::endl;
        return 1;
    }

    std::string serverIP = serverIPPort.substr(0, colonPos);
    int port = std::atoi(serverIPPort.substr(colonPos + 1).c_str());

    // Read the content of the source code file
    std::ifstream sourceFile(sourceFileName);
    if (!sourceFile.is_open()) {
        std::cerr << "Error opening source code file: " << sourceFileName << std::endl;
        return 1;
    }

    std::string sourceCodeContent((std::istreambuf_iterator<char>(sourceFile)),
                                  std::istreambuf_iterator<char>());


    struct timeval start, end,response_start,response_end;
    gettimeofday(&start, NULL);

    // Perform the request-response loop
    for (int i = 0; i < loopNum; i++) {
        // Create a socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            perror("Socket creation error");
            return 1;
        }

        // Set the receive timeout option
        struct timeval timeout;
        timeout.tv_sec = timeoutSeconds;
        timeout.tv_usec = 0;
        if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
            perror("Set receive timeout error");
            close(clientSocket);
            return 1;
        }

        // Connect to the server
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr) <= 0) {
            perror("Invalid server address");
            close(clientSocket);
            return 1;
        }

        if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
            perror("Connection error");
            close(clientSocket);
            errorRequests++;
            continue;
        }

        // Measure the time just before sending the request (Tsend)
      gettimeofday(&response_start,NULL);


        // Send the source code content to the server
        send(clientSocket, sourceCodeContent.c_str(), sourceCodeContent.size(), 0);

        // Receive and display the server response
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        // Check for a receive timeout error
        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "Request timeout" << std::endl;
                timeoutRequests++;
            } else {
                perror("Error receiving response from server");
                errorRequests++;
            }
        } else if (bytesRead == 0) {
            // Connection closed by the server
            std::cout << "Connection closed by the server" << std::endl;
            errorRequests++;
        } else {
            // Measure the time just after getting the response (Trecv)
            gettimeofday(&response_end, NULL);
            double responseTime = (response_end.tv_sec - response_start.tv_sec) + (response_end.tv_usec - response_start.tv_usec) / 1000000.0;

            buffer[bytesRead] = '\0';
            std::cout << "Response successful: " << buffer << std::endl;
            successful_totalTime += responseTime;
            successfulResponses++;
        }

        close(clientSocket);
        sleep(sleepTimeSeconds);
    }

 // Measure the time just after getting the response (Trecv)
    gettimeofday(&end, NULL);
     totalTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    if (successfulResponses == 0) {
        std::cout << "Average Response Time: " << (successful_totalTime) << " seconds" << std::endl;
    } else {
        std::cout << "Average Response Time: " << (successful_totalTime / successfulResponses) << " seconds" << std::endl;
    }

    std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
    std::cout << "Total time of the client: " << totalTime << " seconds" << std::endl;
    std::cout << "Throughput: " << successfulResponses/totalTime << " requests/second" <<std::endl;
    printf("Total Requests Sent: %d\n", totalRequests);
    // printf("Successful Requests (Goodput): %d\n", successfulRequests);
   
    //time out rate
    std::cout << "Timeout Rate: " << timeoutRequests/totalTime << " requests/second" <<std::endl;
    
    //error  rate
    std::cout << "Error Rate: " << errorRequests/totalTime << " requests/second" <<std::endl;

    return 0;
}
