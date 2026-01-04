#include "ProcessManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"
#include <iostream>
#include <cstring>

// Conditional includes for cross-platform compatibility
#ifdef _WIN32
#include <windows.h> // For Windows process management (e.g., CreateProcess, WaitForSingleObject)
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

ProcessManagement::ProcessManagement()
{
}
bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    taskQueue.push(std::move(task));
    return true;
}

void ProcessManagement::executeTasks()
{
    while (!taskQueue.empty())
    {
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();
        std::cout << "Executing Task: " << taskToExecute->toString() << std::endl;
        executeCryption(taskToExecute->toString());
    }
}
