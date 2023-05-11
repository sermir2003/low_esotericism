#pragma once
#include "kernels.h"
#include <string>
#include <vector>

const std::string kTaskFileDefaultName = "sample";

class Task {
private:
    GaussKernels kernels_;   /* birth and death parameters */

    double b_;          /* birth rate */
    double s_;          /* competition rate */
    double d_;          /* death rate */

    double alpha_;      /* closure parameters */
    double beta_;
    double gamma_;

    int nodes_;        /* iteration count */
    int iter_;        /* nodes count */

    double radius_;          /* integration limit */
    double step_size_;       /* step between nodes */

    std::string research_name_;  /* path for storing data */
public:
    Task() = delete;
    Task(double sigma_m, double sigma_w);
    Task(const Task& other) = default;
    Task(Task&& other) = default;
    Task& operator=(const Task& other) = default;
    Task& operator=(Task&& other) = default;
    ~Task() = default;
    Task(const std::string& task_file_name = kTaskFileDefaultName);
    static void CreateTaskFile(const std::string& task_file_name = kTaskFileDefaultName);

    double m(double xi) const { return kernels_.m(xi); }
    double w(double xi) const { return kernels_.w(xi); }
    double b() const { return b_; }
    double s() const { return s_; }
    double d() const { return d_; }
    double alpha() const { return alpha_; }
    double beta() const { return beta_; }
    double gamma() const { return gamma_; }
    int nodes() const { return nodes_; }
    int iter() const { return iter_; }
    double radius() const { return radius_; }
    double step_size() const { return step_size_; }
    std::string ResearchName() const { return research_name_; }
};

struct Result {
    double N;
    double step_size;
    int nodes;
    std::vector<double> C;

    void SaveToFile(std::string path_result_file);
};

class TaskFileParseException : public std::runtime_error {
public:
    TaskFileParseException(const std::string& error_message) : std::runtime_error(error_message) {
    }
};
