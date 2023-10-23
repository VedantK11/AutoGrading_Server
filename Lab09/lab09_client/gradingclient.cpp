// #include <iostream>
// #include <fstream>
// #include <cstring>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <errno.h>
// #include <arpa/inet.h>
// #include <sys/time.h>   // For gettimeofday
// #include <sys/select.h> // For select

// int main(int argc, char *argv[])
// {
//     if (argc != 6)
//     {
//         std::cerr << "Usage: " << argv[0] << " <serverIP:port>  <sourceCodeFileTobeGraded>  <loopNum> <sleepTimeSeconds> <timeout-seconds>" << std::endl;
//         return 1;
//     }

//     std::string serverIPPort = argv[1];
//     std::string sourceFileName = argv[2];
//     int loopNum = std::atoi(argv[3]);
//     int sleepTimeSeconds = std::atoi(argv[4]);
//     int timeoutSeconds = std::atoi(argv[5]);

//     double successful_totalTime = 0.0;
//     double totalTime_of_loop = 0.0;
//     int successfulResponses = 0;
//     double responseTime = 0;
//     int totalRequests = loopNum;
//     int successfulRequests = 0;
//     int timeoutRequests = 0;
//     int errorRequests = 0;

//     // Extract server IP and port from the command line argument
//     size_t colonPos = serverIPPort.find(':');
//     if (colonPos == std::string::npos)
//     {
//         std::cerr << "Invalid server IP:port format" << std::endl;
//         return 1;
//     }

//     std::string serverIP = serverIPPort.substr(0, colonPos);
//     int port = std::atoi(serverIPPort.substr(colonPos + 1).c_str());

//     // // Create a socket
//     // int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     // if (clientSocket == -1) {
//     //     perror("Socket creation error");
//     //     return 1;
//     // }

//     // // Connect to the server
//     // struct sockaddr_in serverAddress;
//     // serverAddress.sin_family = AF_INET;
//     // serverAddress.sin_port = htons(port);
//     // if (inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr) <= 0) {
//     //     perror("Invalid server address");
//     //     close(clientSocket);
//     //     return 1;
//     // }

//     // if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//     //     perror("Connection error");
//     //     close(clientSocket);
//     //     errorRequests=loopNum;
//     //     //totalRequests=loopNum;
//     //     std::cout << "Average Response Time: " << 0.0 << " seconds" << std::endl;
//     //     std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
//     // std::cout << "Time Taken for Completing the Loop: " << totalTime_of_loop << " seconds" << std::endl;
//     // printf("Total Requests Sent: %d\n", totalRequests);
//     // printf("Successful Requests (Goodput): %d\n", successfulRequests);
//     // printf("Timeout Requests: %d\n", timeoutRequests);
//     // printf("Error Requests: %d\n", errorRequests);

//     //     return 1;
//     // }

//     // Read the content of the source code file
//     std::ifstream sourceFile(sourceFileName);
//     if (!sourceFile.is_open())
//     {
//         std::cerr << "Error opening source code file: " << sourceFileName << std::endl;
//         // close(clientSocket);
//         return 1;
//     }

//     std::string sourceCodeContent((std::istreambuf_iterator<char>(sourceFile)),
//                                   std::istreambuf_iterator<char>());

//     // Perform the request-response loop
//     for (int i = 0; i < loopNum; i++)
//     {

//         // Create a socket
//         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//         if (clientSocket == -1)
//         {
//             perror("Socket creation error");
//             return 1;
//         }

//         // Connect to the server
//         struct sockaddr_in serverAddress;
//         serverAddress.sin_family = AF_INET;
//         serverAddress.sin_port = htons(port);
//         if (inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr) <= 0)
//         {
//             perror("Invalid server address");
//             close(clientSocket);
//             return 1;
//         }

//         if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
//         {
//             perror("Connection error");
//             close(clientSocket);
//             errorRequests = loopNum;
//             // totalRequests=loopNum;
//             std::cout << "Average Response Time: " << 0.0 << " seconds" << std::endl;
//             std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
//             std::cout << "Time Taken for Completing the Loop: " << totalTime_of_loop << " seconds" << std::endl;
//             printf("Total Requests Sent: %d\n", totalRequests);
//             printf("Successful Requests (Goodput): %d\n", successfulRequests);
//             printf("Timeout Requests: %d\n", timeoutRequests);
//             printf("Error Requests: %d\n", errorRequests);

//             return 1;
//         }

//         // Measure the time just before sending the request (Tsend)
//         struct timeval start, end;
//         gettimeofday(&start, NULL);

//         // Send the source code content to the server
//         send(clientSocket, sourceCodeContent.c_str(), sourceCodeContent.size(), 0);

//         // Use select to wait for the socket to be readable with a timeout
//         fd_set read_fds;
//         FD_ZERO(&read_fds);
//         FD_SET(clientSocket, &read_fds);

//         struct timeval timeout;
//         timeout.tv_sec = timeoutSeconds;
//         timeout.tv_usec = 0;

//         int selectResult = select(clientSocket + 1, &read_fds, NULL, NULL, &timeout);

//         if (selectResult == -1)
//         {
//             perror("Select error");
//             close(clientSocket);
//             return 1;
//         }
//         else if (selectResult == 0)
//         {
//             std::cout << "Request timeout" << std::endl;
//             timeoutRequests++;
//             // close(clientSocket);
//             // return 1;
//             continue;
//         }

//         // Receive and display the server response
//         char buffer[1024];
//         ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

//         // Measure the time just after getting the response (Trecv)
//         gettimeofday(&end, NULL);
//         double responseTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

//         close(clientSocket);

//         if (bytesRead <= 0)
//         {
//             std::cout << "Error receiving response from server" << std::endl;
//             errorRequests++;
//         }
//         else
//         {
//             buffer[bytesRead] = '\0';
//             std::cout << "Response successful: " << buffer << std::endl;
//             //  std::cout << "Response Time: " << responseTime << " seconds" << std::endl;
//             successful_totalTime += responseTime;
//             successfulResponses++;
//             successfulRequests++;
//         }
//         sleep(sleepTimeSeconds);

//         totalTime_of_loop += responseTime + sleepTimeSeconds;
//         // close(clientSocket);
//     }

//     if (successfulResponses == 0)
//     {
//         std::cout << "Average Response Time: " << (successful_totalTime) << " seconds" << std::endl;
//     }
//     else
//     {
//         std::cout << "Average Response Time: " << (successful_totalTime / successfulResponses) << " seconds" << std::endl;
//     }
//     std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
//     std::cout << "Time Taken for Completing the Loop: " << totalTime_of_loop << " seconds" << std::endl;
//     printf("Total Requests Sent: %d\n", totalRequests);
//     printf("Successful Requests (Goodput): %d\n", successfulRequests);
//     printf("Timeout Requests: %d\n", timeoutRequests);
//     printf("Error Requests: %d\n", errorRequests);

//     // close(clientSocket);
//     return 0;
// }


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

    double successful_totalTime = 0.0;
    double totalTime_of_loop = 0.0;
    int successfulResponses = 0;
    double responseTime = 0;
    int totalRequests = loopNum;
    int successfulRequests = 0;
    int timeoutRequests = 0;
    int errorRequests = 0;

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
            // if (errno == EINPROGRESS || errno == EALREADY || errno == EWOULDBLOCK || errno == EINTR) {
            //     std::cout << "Request timeout" << std::endl;
            //     timeoutRequests++;
            //     close(clientSocket);
            //     continue;
            // }
            perror("Connection error");
            close(clientSocket);
            errorRequests++;
            continue;
        }

        // Measure the time just before sending the request (Tsend)
        struct timeval start, end;
        gettimeofday(&start, NULL);

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
            gettimeofday(&end, NULL);
            double responseTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

            buffer[bytesRead] = '\0';
            std::cout << "Response successful: " << buffer << std::endl;
            successful_totalTime += responseTime;
            successfulResponses++;
            successfulRequests++;
        }

        close(clientSocket);
        sleep(sleepTimeSeconds);
        totalTime_of_loop += responseTime + sleepTimeSeconds;
    }

    if (successfulResponses == 0) {
        std::cout << "Average Response Time: " << (successful_totalTime) << " seconds" << std::endl;
    } else {
        std::cout << "Average Response Time: " << (successful_totalTime / successfulResponses) << " seconds" << std::endl;
    }
    std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
    std::cout << "Time Taken for Completing the Loop: " << totalTime_of_loop << " seconds" << std::endl;
    printf("Total Requests Sent: %d\n", totalRequests);
    printf("Successful Requests (Goodput): %d\n", successfulRequests);
    printf("Timeout Requests: %d\n", timeoutRequests);
    printf("Error Requests: %d\n", errorRequests);

    return 0;
}

