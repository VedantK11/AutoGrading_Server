#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Function to compile and execute the source code
std::string compileAndRun(const char *sourceFileName)
{
    std::string response;

    // Compile the source code and capture both stdout and stderr
    std::string compileCommand = "g++ -o executable " + std::string(sourceFileName) + " > compile_output.txt 2>&1";
    int compileExitCode = system(compileCommand.c_str());

    if (compileExitCode != 0)
    {
        std::ifstream compileOutputFile("compile_output.txt");
        // compileOutputFile>>str;
        std::ostringstream compileOutputContent;
        compileOutputContent << compileOutputFile.rdbuf();
        response = "COMPILER ERROR\n" + compileOutputContent.str();
    }
    else
    {
        // Execute the compiled program and capture both stdout and stderr
        int runExitCode = system("./executable > program_output.txt 2>&1");

        if (runExitCode != 0)
        {
            std::ifstream runOutputFile("program_output.txt");
            std::ostringstream runOutputContent;
            runOutputContent << runOutputFile.rdbuf();
            response = "RUNTIME ERROR\n" + runOutputContent.str();
        }
        else
        {
            // Program executed successfully, compare its output with the expected output
            std::ifstream programOutputFile("program_output.txt");
            std::ostringstream programOutputContent;
            programOutputContent << programOutputFile.rdbuf();
            std::string programOutput = programOutputContent.str();

            std::ifstream expectedOutputFile("expected_output.txt");
            std::ostringstream expectedOutputContent;
            expectedOutputContent << expectedOutputFile.rdbuf();
            std::string expectedOutput = expectedOutputContent.str();

            if (programOutput == expectedOutput)
            {
                response = "PASS\n" + programOutput;
            }
            else
            {
                // Handle output error
                std::ofstream programOutputFile("program_output.txt");
                programOutputFile << programOutput;
                programOutputFile.close();

                system("diff program_output.txt expected_output.txt > diff_output.txt");

                std::ifstream diffOutputFile("diff_output.txt");
                std::ostringstream diffOutputContent;
                diffOutputContent << diffOutputFile.rdbuf();
                response = "OUTPUT ERROR\n" + programOutput + "\n" + diffOutputContent.str();
            }
        }
    }

    return response;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Socket creation error");
        return 1;
    }

    // Bind to the specified port
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

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        perror("Listen error");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true)
    {
        // Accept a connection from a client
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1)
        {
            perror("Accept error");
            continue;
        }

        // **Receive client request (source code file name)**
        //create tempfile name
        std::string tempFileName = "temp_source.cpp";

        //receive file size
        size_t fileSize;
        ssize_t sizeReceived = recv(clientSocket, &fileSize, sizeof(fileSize), 0);

        if (sizeReceived != sizeof(fileSize))
        {
            std::cerr << "Error receiving file size" << std::endl;
            send(clientSocket, "Error receiving file size", sizeof("Error receiving file size"), 0);
            close(clientSocket);
            return 1;
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
            return 1;
        }

        const int bufferSize = 1024;
        char buffer[bufferSize];
        size_t remainingBytes = fileSize;

        while (remainingBytes > 0)
        {
            int bytesReceived = recv(clientSocket, buffer, std::min(static_cast<size_t>(bufferSize), remainingBytes), 0);

            if (bytesReceived <= 0)
            {
                std::cerr << "Error receiving data" << std::endl;
                break;
            }

            outputFile.write(buffer, bytesReceived);
            remainingBytes -= bytesReceived;
        }

        outputFile.close();

        // Compile and run the source code  
        std::string response = compileAndRun(tempFileName.c_str());

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);

        // Remove the temporary source code file
        remove(tempFileName.c_str());
    }

    close(serverSocket);
    return 0;
}
