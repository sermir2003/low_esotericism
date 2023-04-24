#pragma once
#include <vector>
#include "task.h"
#include "progress_counter.h"

class Solver {
private:
    const Task& p_;  /* biological data (p because of "problem", as it was called by Mikhail) */
    std::string calculation_name_;

    double N;
    std::vector<double> C;
    std::vector<double> m;
    std::vector<double> w;
    std::vector<double> conv_wC;
    std::vector<double> conv_mC;
    std::vector<double> conv_CwC;
    std::vector<double> multi_Cw;
private:
    void VectorMultiplication(const std::vector<double>& f, const std::vector<double>& g,
                          std::vector<double>& result);
    double DotProduct(const std::vector<double>& f, const std::vector<double>& g);
    double Convolution(const std::vector<double>& f, const std::vector<double>& g, double x_ind);
    void VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                           std::vector<double>& result);
    void RecalculateConvolutions();
public:
    Solver(const Task& task, const std::string& calculation_name);
    Result Solve();
};
