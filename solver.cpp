#include "solver.h"
//double integration_weight(int i, int nodes, double step_size) {
//    return (i == 0 || i == nodes - 1 ? step_size / 2 : step_size);
//}
void Solver::VectorMultiplication(const std::vector<double>& f, const std::vector<double>& g,
                            std::vector<double>& result) {
    for (int i = 0; i < p_.nodes(); ++i) {
        result[i] = f[i] * g[i];
    }
}
double Solver::DotProduct(const std::vector<double>& f, const std::vector<double>& g) {
    double result = 0;
    for (int i = 0; i < p_.nodes(); ++i) {
        result += f[i] * g[i];
    }
    result *= p_.step_size();  // integration weight
    result *= 2; // dimension ratio: calculated on [0, +inf), but (-inf, 0] symmetrical
    return result;
}
double Solver::Convolution(const std::vector<double>& f, const std::vector<double>& g,
                           double x_ind) {
    double result = 0;
    for (int y_ind = -p_.nodes() + 1; y_ind < p_.nodes(); ++y_ind) {
        int x_minus_y_ind = x_ind - y_ind;
        int y_ind_norm = (y_ind >= 0 ? y_ind : -y_ind);
        int x_minus_y_ind_norm = (x_minus_y_ind >= 0 ? x_minus_y_ind : -x_minus_y_ind);
        result += f[x_minus_y_ind_norm] * g[y_ind_norm];
    }
    result *= p_.step_size();  // integration weight
    return result;
}
void Solver::VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                               std::vector<double>& result) {
    for (int i = 0; i < p_.nodes(); ++i) {
        result[i] = Convolution(f, g, i);
    }
}
Result Solver::Solve() {
    /* Neumann method */
    ProgressCounter progress_counter(calculation_name_);
    for (int iter = 1; iter <= p_.iter(); ++iter) {
        N = (p_.b() - p_.d()) / (DotProduct(C, w) + p_.s());
        RecalculateConvolutions();
        for(int j = 0; j < p_.iter(); ++j) {
            C[j] = (m[j] / N - w[j] + conv_mC[j] + p_.b() - p_.d() -
                    N / (p_.alpha() + p_.gamma()) *
                        (p_.alpha() * (p_.b() - p_.d()) / N +
                         p_.beta() * (conv_wC[j] + conv_CwC[j]) +
                         p_.gamma() * ((p_.b() - p_.d()) / N + conv_wC[j] + conv_CwC[j]))) /
                   (p_.d() + w[j] + N / (p_.alpha() + p_.gamma()) *
                                       p_.alpha() * (p_.b() - p_.d()) / N  + p_.beta() * p_.s());
        }
        progress_counter.UpdateProgress(1.0 * iter / p_.iter());
    }
    progress_counter.FinishAction();

    /* correcting second moment */
    for (int i = 0; i < p_.nodes(); ++i) {
        C[i] = N * N * (C[i] + 1);
    }

    /* save results */
    Result res;
    res.N = N;
    res.step_size = p_.step_size();
    res.nodes = p_.nodes();
    res.C = C;
    return res;
}
void Solver::RecalculateConvolutions() {
    VectorMultiplication(C, w, multi_Cw);
    VectorConvolution(w, C, conv_wC);
    VectorConvolution(m, C, conv_mC);
    VectorConvolution(multi_Cw, w, conv_CwC);
}
Solver::Solver(const Task& task, const std::string& calculation_name) :
      p_(task),
      calculation_name_(calculation_name),
      N(0),
      C(p_.nodes()),
      m(p_.nodes()),
      w(p_.nodes()),
      conv_wC(p_.nodes()),
      conv_mC(p_.nodes()),
      conv_CwC(p_.nodes()),
      multi_Cw(p_.nodes()) {
    double x = 0;
    for (int i = 0; i < p_.nodes(); ++i) {
        m[i] = p_.m(x) * p_.b();
        w[i] = p_.w(x) * p_.s();
        C[i] = w[i];
        x += p_.step_size();
    }
}
