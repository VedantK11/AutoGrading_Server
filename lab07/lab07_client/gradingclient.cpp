#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h> // For gettimeofday

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <serverIP:port> <sourceCodeFileTobeGraded>" << std::endl;
        return 1;
    }

    std::string serverIPPort = argv[1];
    std::string sourceFileName = argv[2];
    int loopNum = std::atoi(argv[3]);
    int sleepTimeSeconds = std::atoi(argv[4]);

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
        // close(clientSocket);
        return 1;
    }

    std::string sourceCodeContent((std::istreambuf_iterator<char>(sourceFile)),
                                   std::istreambuf_iterator<char>());
                                   
   double successful_totalTime = 0.0;
    double totalTime_of_loop = 0.0;
    int successfulResponses = 0;

    // Perform the request-response loop
    for (int i = 0; i < loopNum; i++) {
    
    
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation error");
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

	

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Connection error");
        close(clientSocket);
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
    
// Measure the time just after getting the response (Trecv)
        gettimeofday(&end, NULL);
        double responseTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

   
    	if (bytesRead <= 0) {
            std::cout << "Error receiving response from server" << std::endl;
        } 
        else {
            buffer[bytesRead] = '\0';
            std::cout << "Response successful: " << buffer << std::endl;
          //  std::cout << "Response Time: " << responseTime << " seconds" << std::endl;
            successful_totalTime += responseTime;
            successfulResponses++;
        }
        close(clientSocket);
         sleep(sleepTimeSeconds);
	totalTime_of_loop += responseTime + sleepTimeSeconds;
    }
    if(successfulResponses==0)
    {
     std::cout << "Average Response Time: " << (successful_totalTime) << " seconds" << std::endl;
     std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
    
    }
    else{
    std::cout << "Average Response Time: " << (successful_totalTime / successfulResponses) << " seconds" << std::endl;
    std::cout << "Number of Successful Responses: " << successfulResponses << std::endl;
    }
    std::cout << "Time Taken for Completing the Loop: " << totalTime_of_loop << " seconds" << std::endl;

    //close(clientSocket);
    return 0;
}

