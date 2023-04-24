#pragma once
#include <string>
#include "task.h"

enum class InputRequestType {
    CREATE_TASK_FILE,
    SOLVE,
    HELP,
};

struct InputRequest {
    virtual InputRequestType GetType() = 0;
    virtual ~InputRequest() {
    }
};

struct RequestCreateTaskFile : public InputRequest {
    InputRequestType GetType() override {
        return InputRequestType::CREATE_TASK_FILE;
    }
    std::string path_task_file;
    RequestCreateTaskFile(const std::string& path_task_file)
        : path_task_file(path_task_file) {
    }
};

struct RequestSolve : public InputRequest {
    InputRequestType GetType() override {
        return InputRequestType::SOLVE;
    }
    std::string path_task_file;
    RequestSolve(const std::string& path_task_file)
        : path_task_file(path_task_file) {
    }
};

struct RequestHelp : public InputRequest {
    InputRequestType GetType() override {
        return InputRequestType::HELP;
    }
    RequestHelp() = default;
};
