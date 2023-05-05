#pragma once
#include <vector>
#include "task.h"
#include "progress_counter.h"
#include <fftw3.h>

class Solver {
public:
    virtual Result Solve() = 0;
    virtual ~Solver() {
    }
};

class LinearSolver : public Solver {
private:
    const Task& p;  /* biological data (p because of "problem", as it was called by Mikhail) */
    std::string calculation_name_;

    double N;
    double dotProd_wQ;
    std::vector<double> Q;
    std::vector<double> m;
    std::vector<double> w;
    std::vector<double> conv_wQ;
    std::vector<double> conv_mQ;
    std::vector<double> conv_QwQ;
    std::vector<double> multi_Qw;
private:
    void VectorMultiplication(const std::vector<double>& f, const std::vector<double>& g,
                          std::vector<double>& result);
    double DotProduct(const std::vector<double>& f, const std::vector<double>& g);
    double Convolution(const std::vector<double>& f, const std::vector<double>& g, double x_ind);
    void VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                           std::vector<double>& result);
    void RecalculateConvolutions();
public:
    LinearSolver(const Task& task, const std::string& calculation_name);
    Result Solve() override;
};
