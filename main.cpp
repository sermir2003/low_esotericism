#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <thread>
#include "cmd_args_parser.h"
#include "task.h"
#include "solver.h"

void ThreadWorker(std::vector<std::pair<double, double>> data, unsigned left, unsigned right,
                  std::ofstream& result_file, std::mutex& write_mutex) {
    {
        std::lock_guard<std::mutex> lock(write_mutex);
        std::cout << std::fixed << std::setprecision(8)
                  << "thread [" << left << ", " << right << "] started " << std::endl;
    }
    if (left >= right) {
        return;
    }
    for (unsigned i = left; i < right; ++i) {
        unsigned j = i - left + 1;
        try {
            Task task(data[i].first, data[i].second);
            Solver solver(task);
            double result = solver.Solve();
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                result_file << std::fixed << std::setprecision(8)
                            << data[i].first << " " << data[i].second << " " << result << std::endl;
                std::cout << std::fixed << std::setprecision(8)
                          << "thread [" << left << ", " << right << "] successfully " << j
                          << " (" << j * 100.0 / (right - left) << "%)" << std::endl;
            }
        } catch (...) {
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                result_file << std::fixed << std::setprecision(8)
                            << data[i].first << " " << data[i].second << " -179" << std::endl;
                std::cout << std::fixed << std::setprecision(8)
                          << "thread [" << left << ", " << right << "] failed " << j
                          << " (" << j * 100.0 / (right - left) << "%)" << std::endl;
            }
        }
    }
}

int main() {
    size_t n = 40;
    double begin = 0.01;
    double end = 0.2;
    double step_size = (end - begin) / (n - 1);
    std::vector<std::pair<double, double>> data(n * n);
    {
        double x = 0;
        for (size_t i = 0; i < n; ++i) {
            double y = 0;
            for (size_t j = 0; j < n; ++j) {
                y += step_size;
                data[i * n + j] = {x, y};
            }
            x += step_size;
        }
    }
    unsigned cnt_threads = 14;
    unsigned bound = data.size();
    unsigned seg_size = (bound + cnt_threads - 1) / cnt_threads;
    {
        std::ofstream result_file("02-005-001-3001-500.csv", std::ios::out);
        std::mutex write_mutex;
        result_file << "x y z" << std::endl;
        std::vector<std::jthread> workers;
        for (unsigned i = 0; i < cnt_threads; ++i) {
            unsigned left = seg_size * i;
            unsigned right = std::min(seg_size * (i + 1), bound);
            workers.emplace_back(ThreadWorker, data, left, right, std::ref(result_file), std::ref(write_mutex));
        }
    }
    return 0;
}
