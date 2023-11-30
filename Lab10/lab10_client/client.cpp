#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <new|status> <serverIP:port> <sourceCodeFileTobeGraded|requestID>" << std::endl;
        return 1;
    }

    std::string operation = argv[1];
    std::string serverIPPort = argv[2];
    std::string sourceOrRequest = argv[3];

    // Extract server IP and port from the command line argument
    size_t colonPos = serverIPPort.find(':');
    if (colonPos == std::string::npos)
    {
        std::cerr << "Invalid server IP:port format" << std::endl;
        return 1;
    }

    std::string serverIP = serverIPPort.substr(0, colonPos);
    int port = std::atoi(serverIPPort.substr(colonPos + 1).c_str());

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Socket creation error");
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr) <= 0)
    {
        perror("Invalid server address");
        close(clientSocket);
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Connection error");
        close(clientSocket);
        return 1;
    }

    if (operation == "new")
    {

        // Send the operation type to the server
        send(clientSocket, operation.c_str(), operation.size(), 0);
        std::cout << operation << std::endl;
        char serverMsg[1024];
        memset(serverMsg,0,sizeof(serverMsg));
        recv(clientSocket,serverMsg,sizeof(serverMsg),0);
        
        // Read the content of the source code file
        std::ifstream sourceFile(sourceOrRequest);
        if (!sourceFile.is_open())
        {
            std::cerr << "Error opening source code file: " << sourceOrRequest << std::endl;
            close(clientSocket);
            return 1;
        }

        std::string sourceCodeContent((std::istreambuf_iterator<char>(sourceFile)),
                                      std::istreambuf_iterator<char>());

                                      ssize_t fileSize = sourceCodeContent.size();

        // Send the source code file size to the server
        send(clientSocket, &fileSize, sizeof(fileSize), 0);

        char message[50];

        recv(clientSocket, message, sizeof(message), 0);

        // Send the source code content to the server
        send(clientSocket, sourceCodeContent.c_str(), sourceCodeContent.size(), 0);
        std::cout << "File sent for grading\n";
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer;
    }
    else if (operation == "status")
    {
        // Send the operation type to the server
        send(clientSocket, operation.c_str(), operation.size(), 0);
        // std::cout << operation << std::endl;
        char serverMsg[1024];
        memset(serverMsg,0,sizeof(serverMsg));
        recv(clientSocket,serverMsg,sizeof(serverMsg),0);
        // Send the request ID to the server
        send(clientSocket, sourceOrRequest.c_str(), sourceOrRequest.size(), 0);
        std::cout << sourceOrRequest << std::endl;
        
        // Poll the server for the status until it receives the "processing is done" response
        char buffer[1024];
        ssize_t bytesRead;
        // do {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;
        }
        // sleep(1); // Adjust the polling interval as needed
        // } while (bytesRead > 0 && std::string(buffer).find("processing is done") == std::string::npos);
    }

    close(clientSocket);
    return 0;
}
