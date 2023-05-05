#include "task.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
#include "kernels.h"

using Json = nlohmann::json;

Json file_content = {
    {"Integration area radius", 20},
    {"Iteration count", 1000},
    {"Grid count", 5001},
    {"alpha", 1},
    {"beta", 1},
    {"gamma", 1},
    {"d", 0},
    {"d'", 1},
    {"b", 1},
    {"Kernels", {
            {"type", "Gauss"},
            {"sigma m", 1},
            {"sigma w", 1},
            {"limit m", -1},
            {"limit w", -1},
        }
    },
};

void Task::CreateTaskFile(const std::string& task_file_name) {
    std::ofstream file(task_file_name + ".json", std::ios::out);
    file << std::setw(4) << file_content << std::endl;
}
Task::Task(const std::string& task_file_name) {
    std::ifstream file(task_file_name + ".json", std::ios::in);
    Json data_json;
    file >> data_json;
    try {
        radius_ = data_json["Integration area radius"].get<double>();
        iter_ = data_json["Iteration count"].get<int>();
        nodes_ = data_json["Grid count"].get<int>();
        alpha_ = data_json["alpha"].get<double>();
        beta_ = data_json["beta"].get<double>();
        gamma_ = data_json["gamma"].get<double>();
        d_ = data_json["d"].get<double>();
        s_ = data_json["d'"].get<double>();
        b_ = data_json["b"].get<double>();
        research_name_ = task_file_name;
        kernels_ = MakeKernels(data_json["Kernels"].get<Json>());
        step_size_ = radius_ / (nodes_ - 1);
    } catch (...) {
        throw TaskFileParseException("Task file parse exception");
    }
}

void Result::SaveToFile(std::string path_result_file) {
    {
        std::ofstream file_N(path_result_file + "_N.txt", std::ios::out);
        file_N << std::fixed << std::setprecision(8) << N << "\n";
    }
    {
        std::ofstream file_C(path_result_file + "_C.csv", std::ios::out);
        file_C << "x,y\n";
        double x = 0;
        for (int i = 0; i < nodes; ++i) {
            file_C << std::fixed << std::setprecision(8) << x << ',' << C[i] << '\n';
            x += step_size;
        }
    }
}
