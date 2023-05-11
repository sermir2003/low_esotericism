#include "solver.h"
#include "kernels.h"

void Solver::VectorMultiplication(const std::vector<double>& f, const std::vector<double>& g,
                            std::vector<double>& result) {
    for (int i = 0; i < p.nodes(); ++i) {
        result[i] = f[i] * g[i];
    }
}
double Solver::DotProduct(const std::vector<double>& f, const std::vector<double>& g) {
    double result = 0;
    result += f[0] * g[0] * 0.5;
    for (int i = 1; i < p.nodes(); ++i) {
        result += f[i] * g[i];
    }
    result *= p.step_size();  // integration weight
    result *= 2; // dimension ratio: calculated on [0, +inf), but (-inf, 0] symmetrical
    return result;
}
double Solver::ElementConvolution(const std::vector<double>& f, const std::vector<double>& g,
                                  int x_ind) {
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
void Solver::VectorConvolution(const std::vector<double>& f, const std::vector<double>& g,
                               std::vector<double>& result) {
    for (int i = 0; i < p.nodes(); ++i) {
        result[i] = ElementConvolution(f, g, i);
    }
}
double Solver::Solve() {
    Init();
    /* Neumann method */
//    double Y = 1;
    for (int iter = 1; iter <= p.iter(); ++iter) {
        dotProd_wQ = DotProduct(w, Q);
        N = (p.b() - p.d()) / (dotProd_wQ + p.s());
        for (int j = 0; j < p.nodes(); ++j) {
            Q[j] = (m[j] / N - w[j] + conv_mQ[j] + p.b() - p.d() -
                    N / (p.alpha() + p.gamma()) *
                        (p.alpha() * (p.b() - p.d()) / N +
                            p.beta() * conv_wQ[j] +
                            p.gamma() * ((p.b() - p.d()) / N + conv_wQ[j] + conv_QwQ[j]))) /
                    (p.d() + w[j] + N / (p.alpha() + p.gamma()) *
                                        (p.alpha() * (p.b() - p.d()) / N  + p.beta() * (conv_wQ[j] + p.s())));
//            Q[j] = (m[j] / N - w[j] + conv_mQ[j] + p.b() - p.d() -
//                    N / (p.alpha() + p.gamma()) *
//                        (p.alpha() * (p.b() - p.d()) / N +
//                         p.beta() * (conv_wQ[j] + conv_QwQ[j]) +
//                         p.gamma() * ((p.b() - p.d()) / N + conv_wQ[j] + conv_QwQ[j]))) /
//                   (p.d() + w[j] + N / (p.alpha() + p.gamma()) *
//                                       p.alpha() * (p.b() - p.d()) / N  + p.beta() * p.s());
//            Q[j] = (Y * m[j] / (p.b() - p.d()) - w[j] + conv_mQ[j] - p.alpha() * (p.b() - p.d()) / (2 * Y)
//                                                                        * (2 * conv_wQ[j] + conv_QwQ[j])) /
//                   (w[j] + p.b() - p.alpha() / 2 * (p.b() - p.d()) * (1 - p.s() / Y) + p.alpha() * (p.b() - p.d()) / (2 * Y) * conv_wQ[j]);
        }
//        Y = DotProduct(Q, w);
        RecalculateConvolutions();
    }
    return N;
}
void Solver::RecalculateConvolutions() {
    VectorMultiplication(Q, w, multi_Qw);
    VectorConvolution(w, Q, conv_wQ);
    VectorConvolution(m, Q, conv_mQ);
    VectorConvolution(multi_Qw, Q, conv_QwQ);
}
Solver::Solver(const Task& task) : p(task),
      N(10),
      dotProd_wQ(0),
      Q(p.nodes()),
      m(p.nodes()),
      w(p.nodes()),
      conv_wQ(p.nodes()),
      conv_mQ(p.nodes()),
      conv_QwQ(p.nodes()),
      multi_Qw(p.nodes()) {
}
void Solver::Init() {
    N = 179;
    double x = 0;
    for (int i = 0; i < p.nodes(); ++i) {
        m[i] = p.m(x) * p.b();
        w[i] = p.w(x) * p.s();
        Q[i] = w[i];
        x += p.step_size();
    }
    dotProd_wQ = DotProduct(w, Q);
}
