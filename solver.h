#pragma once
#include <vector>
#include "task.h"
#include "progress_counter.h"
#include <fftw3.h>

class Solver {
private:
    const Task& p;  /* biological data (p because of "problem", as it was called by Mikhail) */

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
    double ElementConvolution(const std::vector<double>& f, const std::vector<double>& g, int x_ind);
    void VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                           std::vector<double>& result);
    void RecalculateConvolutions();
public:
    explicit Solver(const Task& task);
    void Init();
    double Solve();
};
