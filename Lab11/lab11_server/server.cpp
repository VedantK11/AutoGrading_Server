#include "server.h"
#include "server_main.h"

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
#include <sys/resource.h>
#include <sys/wait.h>

// Function for calculating and displaying the average
void *calculateAverageRequests(void *arg)
{
    while (true)
    {

        // Calculate the average number of requests in the queue
        if (requestQueue.size() != 0)
        {
            averageRequests += requestQueue.size();
            avgCount++;

            std::cout << "Average Requests in Queue: " << averageRequests / avgCount << " requests" << std::endl;
        }

        // Sleep for a while before recalculating the average
        sleep(0.05); // Adjust the sleep duration as needed
    }
}

// Function to compile and execute the source code
// std::string compileAndRun(const char *sourceFileName, const std::string &folder)
// {
//     // Set the time limit for compilation & execution task of student code
//     // struct rlimit cpuLimit;

//     // cpuLimit.rlim_cur = 10; // 10 seconds time limit
//     // cpuLimit.rlim_max = 15; // 15 seconds hard limit
   
//     // prlimit(getpid(), RLIMIT_CPU, &cpuLimit, NULL);

//     struct rlimit cpuLimit;
//     cpuLimit.rlim_cur = 10;
//     cpuLimit.rlim_max = 15;

//     setrlimit(RLIMIT_CPU, &cpuLimit);

//     std::string response;

//     // Use the folder name in constructing file paths
//     std::string compileCommand = "g++ -o " + folder + "/executable " + folder + "/" + std::string(sourceFileName) + " > " + folder + "/compile_output.txt 2>&1";
//     int compileExitCode = system(compileCommand.c_str());

//     if (compileExitCode != 0)
//     {
//         std::ifstream compileOutputFile(folder + "/compile_output.txt");
//         std::ostringstream compileOutputContent;
//         compileOutputContent << compileOutputFile.rdbuf();
//         response = "COMPILER ERROR\n" + compileOutputContent.str();
//     }
//     else
//     {
//         // Execute the compiled program and capture both stdout and stderr
//         int runExitCode = system((folder + "/executable > " + folder + "/program_output.txt 2>&1").c_str());

//         if (runExitCode != 0)
//         {
//             std::ifstream runOutputFile(folder + "/program_output.txt");
//             std::ostringstream runOutputContent;
//             runOutputContent << runOutputFile.rdbuf();
//             response = "RUNTIME ERROR\n" + runOutputContent.str();
//         }
//         else
//         {
//             // Program executed successfully, compare its output with the expected output
//             std::ifstream programOutputFile(folder + "/program_output.txt");
//             std::ostringstream programOutputContent;
//             programOutputContent << programOutputFile.rdbuf();
//             std::string programOutput = programOutputContent.str();

//             std::ifstream expectedOutputFile(folder + "/expected_output.txt");
//             std::ostringstream expectedOutputContent;
//             expectedOutputContent << expectedOutputFile.rdbuf();
//             std::string expectedOutput = expectedOutputContent.str();

//             if (programOutput == expectedOutput)
//             {
//                 response = "PASS\n" + programOutput;
//             }
//             else
//             {
//                 // Handle output error
//                 std::ofstream programOutputFile(folder + "/program_output.txt");
//                 programOutputFile << programOutput;
//                 programOutputFile.close();

//                 system(("diff " + folder + "/program_output.txt " + folder + "/expected_output.txt > " + folder + "/diff_output.txt").c_str());

//                 std::ifstream diffOutputFile(folder + "/diff_output.txt");
//                 std::ostringstream diffOutputContent;
//                 diffOutputContent << diffOutputFile.rdbuf();
//                 response = "OUTPUT ERROR\n" + programOutput + "\n" + diffOutputContent.str();
//             }
//         }
//     }

//     return response;
// }


std::string compileAndRun(const char *sourceFileName, const std::string &folder)
{
    // Set the time limit for compilation
    // struct rlimit cpuLimit;
    // cpuLimit.rlim_cur = 10; // 10 seconds time limit for compilation
    // cpuLimit.rlim_max = 15; // 15 seconds hard limit for compilation
    // setrlimit(RLIMIT_CPU, &cpuLimit);

    std::string response;

    // Use the folder name in constructing file paths
    std::string compileCommand = "g++ -o " + folder + "/executable " + folder + "/" + std::string(sourceFileName) + " > " + folder + "/compile_output.txt 2>&1";
    int compileExitCode = system(compileCommand.c_str());

    if (compileExitCode != 0)
    {
        // Handle compilation error
        std::ifstream compileOutputFile(folder + "/compile_output.txt");
        std::ostringstream compileOutputContent;
        compileOutputContent << compileOutputFile.rdbuf();
        response = "COMPILER ERROR\n" + compileOutputContent.str();
    }
    else
    {
        // Fork a new process for running the executable
        pid_t childPid = fork();

        if (childPid == -1)
        {
            // Handle fork error
            response = "INTERNAL ERROR (fork)";
        }
        else if (childPid == 0) // Child process
        {
            // Set the time limit for the child process
            struct rlimit childCpuLimit;
            childCpuLimit.rlim_cur = 10;
            childCpuLimit.rlim_max = 15;
            setrlimit(RLIMIT_CPU, &childCpuLimit);
           std::ofstream outputFile(folder + "/program_output.txt");
           freopen((folder + "/program_output.txt").c_str(), "w", stdout);

            // Execute the compiled program
            execl((folder + "/executable").c_str(), (folder + "/executable").c_str(), nullptr);

            // If execl fails, print an error message to stderr
            std::cerr << "Failed to execute the program." << std::endl;
            exit(EXIT_FAILURE);
        }
        else // Parent process
        {
            // Wait for the child process to finish
            int status;
            pid_t result = waitpid(childPid, &status, 0);

            if (result == -1)
            {
                // Handle waitpid error
                response = "INTERNAL ERROR (waitpid)";
            }
            else if (WIFSIGNALED(status)) // Check if the child process was terminated by a signal
            {
                response = "TIME LIMIT EXCEEDED";
            }
            else
            {
                // Process the output as before
                std::ifstream programOutputFile(folder + "/program_output.txt");
                std::ostringstream programOutputContent;
                programOutputContent << programOutputFile.rdbuf();
                std::string programOutput = programOutputContent.str();

                std::ifstream expectedOutputFile(folder + "/expected_output.txt");
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
    }

    return response;
}

// Function for handling a client connection
void *handleClient(void *arg)
{
    //set memory limit of the threads which servers the student grading request
    

    while (true)
    {
        ThreadData threadData;

        // Dequeue a request from the shared queue
        pthread_mutex_lock(&queueMutex);
        while (requestQueue.empty())
        {
            pthread_cond_wait(&queueNotEmpty, &queueMutex);
        }
        threadData = requestQueue.front();
        requestQueue.pop();
        pthread_mutex_unlock(&queueMutex);

        requestStates[threadData.requestID] = 'P';

        // std::string folder = std::to_string(gettid());

        std::string cpfolder = "cp expected_output.txt " + threadData.folderName + "/";
        system(cpfolder.c_str());

        // Create a source code file name
        std::string tempFileName = threadData.requestID + ".cpp";

        // Process the request (compile and run the temporary file)
        std::string response = compileAndRun(tempFileName.c_str(), threadData.folderName);

        // Store the output in the log file (named as its Request ID)

        std::ofstream logFile(threadData.folderName + "/" + threadData.requestID + "_log.txt");
        logFile << response;
        logFile.close();

        requestStates[threadData.requestID] = 'C';
    }
}
