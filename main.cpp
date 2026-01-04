#include <iostream>
#include<filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"
#include "./src/app/fileHandling/IO.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::string directory;
    std::string action;

    std::cout << "Welcome to the File Cryption Utility!" << std::endl;
    std::cout << "Please enter the directory path: ";
    std::getline(std::cin, directory);
    std::cout << "Please enter the action (encrypt/decrypt): ";
    std::getline(std::cin, action);

    try {
        if(!fs::exists(directory) || !fs::is_directory(directory)) {
            ProcessManagement ProcessManagement;
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    //method to create task and submit to queue
                    std::string filePath = entry.path().string();
                    // IO -> Task -> ProcessManagement
                    IO io(filePath);
                    std::fstream f_stream = std::move(io.getFileStream());
                    if(f_stream.is_open()) {
                        Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
                        auto task = std::make_unique<Task>(std::move(f_stream),taskAction,filePath);
                        ProcessManagement.submitToQueue(std::move(task));
                    } else {
                        std::cerr << "Failed to open file: " << filePath << std::endl;
                    }
                }
            }
            ProcessManagement.executeTasks();
        }
        else {
            std::cerr << "The provided path is not a valid directory." << std::endl;
            return 1;
        }

    }catch (const std::exception& e) {
        std::cerr << "File System Error: " << e.what() << std::endl;
    }

}