#include "solver.h"
#include "kernels.h"

void LinearSolver::VectorMultiplication(const std::vector<double>& f, const std::vector<double>& g,
                            std::vector<double>& result) {
    for (int i = 0; i < p.nodes(); ++i) {
        result[i] = f[i] * g[i];
    }
}
double LinearSolver::DotProduct(const std::vector<double>& f, const std::vector<double>& g) {
    double result = 0;
    result += f[0] * g[0] * 0.5;
    for (int i = 1; i < p.nodes(); ++i) {
        result += f[i] * g[i];
    }
    result *= p.step_size();  // integration weight
    result *= 2; // dimension ratio: calculated on [0, +inf), but (-inf, 0] symmetrical
    return result;
}
double LinearSolver::Convolution(const std::vector<double>& f, const std::vector<double>& g,
                           double x_ind) {
    double result = 0;
    for (int y_ind = -p.nodes() + 1; y_ind < p.nodes(); ++y_ind) {
        int x_minus_y_ind = x_ind - y_ind;
        int y_ind_abs = abs(y_ind);
        int x_minus_y_ind_abs = abs(x_minus_y_ind);
        if (x_minus_y_ind_abs < p.nodes()) {
            result += f[x_minus_y_ind_abs] * g[y_ind_abs];
        }
    }
    result *= p.step_size();  // integration weight
    return result;
}
void LinearSolver::VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                               std::vector<double>& result) {
    for (int i = 0; i < p.nodes(); ++i) {
        result[i] = Convolution(f, g, i);
    }
}
Result LinearSolver::Solve() {
    /* Neumann method */
    ProgressCounter progress_counter(calculation_name_);
    for (int iter = 1; iter <= p.iter(); ++iter) {
        N = (p.b() - p.d()) / (dotProd_wQ + p.s());
        RecalculateConvolutions();
        for(int j = 0; j < p.nodes(); ++j) {
            double N2 = N * N;
            double N3 = N * N * N;
            double alpha_gamma = p.alpha() + p.gamma();
            double denominator =
                N2 * (
                         p.d() + w[j]
                        + p.alpha() / alpha_gamma * N * (dotProd_wQ + p.s())
                        + p.beta() / alpha_gamma * N * (conv_wQ[j] + p.s())
                         );
            Q[j] =
                (m[j] * N + N2 * (conv_mQ[j] + p.b())
                 - p.gamma() / alpha_gamma * N3 * (conv_QwQ[j] + dotProd_wQ + conv_wQ[j] + p.s())
                 + p.beta() * p.s() / alpha_gamma * N3 - denominator) / denominator;
        }
        progress_counter.UpdateProgress(1.0 * iter / p.iter());
    }
    progress_counter.FinishAction();

    /* save results */
    Result res;
    res.N = N;
    res.step_size = p.step_size();
    res.nodes = p.nodes();
    res.C.resize(p.nodes());
    /* correcting second moment */
    for (int i = 0; i < p.nodes(); ++i) {
        res.C[i] = N * N * (Q[i] + 1);
    }
    return res;
}
void LinearSolver::RecalculateConvolutions() {
    dotProd_wQ = DotProduct(w, Q);
    VectorMultiplication(Q, w, multi_Qw);
    VectorConvolution(w, Q, conv_wQ);
    VectorConvolution(m, Q, conv_mQ);
    VectorConvolution(multi_Qw, w, conv_QwQ);
}
LinearSolver::LinearSolver(const Task& task, const std::string& calculation_name) : p(task),
      calculation_name_(calculation_name),
      N(0),
      dotProd_wQ(0),
      Q(p.nodes()),
      m(p.nodes()),
      w(p.nodes()),
      conv_wQ(p.nodes()),
      conv_mQ(p.nodes()),
      conv_QwQ(p.nodes()),
      multi_Qw(p.nodes()) {
    double x = 0;
    for (int i = 0; i < p.nodes(); ++i) {
        m[i] = p.m(x) * p.b();
        w[i] = p.w(x) * p.s();
        Q[i] = w[i];
        x += p.step_size();
    }
}
