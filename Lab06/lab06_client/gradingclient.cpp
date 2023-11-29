#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <serverIP:port> <sourceCodeFileTobeGraded>" << std::endl;
        return 1;
    }

    std::string serverIPPort = argv[1];
    std::string sourceFileName = argv[2];

    // Extract server IP and port from the command line argument
    size_t colonPos = serverIPPort.find(':');
    if (colonPos == std::string::npos) {
        std::cerr << "Invalid server IP:port format" << std::endl;
        return 1;
    }

    std::string serverIP = serverIPPort.substr(0, colonPos);
    int port = std::atoi(serverIPPort.substr(colonPos + 1).c_str());

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
        return 1;
    }

    
    // Read the content of the source code file
    std::ifstream sourceFile(sourceFileName);
    if (!sourceFile.is_open()) {
        std::cerr << "Error opening source code file: " << sourceFileName << std::endl;
        close(clientSocket);
        return 1;
    }

    std::string sourceCodeContent((std::istreambuf_iterator<char>(sourceFile)),
                                   std::istreambuf_iterator<char>());

    ssize_t fileSize=sourceCodeContent.size();

    // Send the source code file size to the server
    send(clientSocket, fileSize, sizeof(fileSize), 0);

    std::string buffer;
    
    recv(clientSocket, buffer,buffer.size() 0);

    // Send the source code content to the server
    send(clientSocket, sourceCodeContent.c_str(), sourceCodeContent.size(), 0);

    // Receive and display the server response
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
        std::cerr << "Error receiving response from server" << std::endl;
    } else {
        buffer[bytesRead] = '\0';
        std::cout << buffer << std::endl;
    }

    close(clientSocket);
    return 0;
}

