#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../fileHandling/IO.hpp"

enum class Action
{
    ENCRYPT,
    DECRYPT
};

struct Task
{
    /* data */
    std::string filePath;
    std::fstream f_stream;
    Action action;

    Task(std::fstream &&stream, Action act, std::string filePath) : f_stream(stream), action(act), filePath(filePath);

    std::string toString()
    {
        std::ostringstream OSS;
        OSS << filePath << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
        return OSS.str();
    };

    static Task fromString(const std::string &taskData)
    {
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;
        if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr))
        {
            Action action = actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT;
            IO io(filePath);
            std::fstream f_stream = std::move(io.getFileStream());
            if (f_stream.is_open())
            {
                return Task(std::move(f_stream), action, filePath);
            }
            else
            {
                throw std::runtime_error("Unable to open the file: " + filePath);
            }
        }
        else
        {
            throw std::runtime_error("Invalid Task data Format");
        }
    }
};

;

#endif