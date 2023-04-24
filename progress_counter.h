#pragma once
#include <chrono>
#include <string>

class ProgressCounter {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    std::string action_name_;
    bool enabled_;

public:
    ProgressCounter(const std::string& action_name);
    void UpdateProgress(double progress);
    void FinishAction();
    std::string MillisecondsToFormat(uint64_t time);
    ~ProgressCounter();
};
