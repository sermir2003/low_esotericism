#include "kernels.h"
#include <cmath>

Kernels::~Kernels() {
}
std::unique_ptr<Kernels> MakeKernels(const Json& kernel_data) {
    std::string type = kernel_data["type"].get<std::string>();
    if (type == "Gauss") {
        return std::make_unique<GaussKernels>(
            kernel_data["sigma m"].get<double>(),
            kernel_data["sigma w"].get<double>(),
            kernel_data["limit m"].get<double>(),
            kernel_data["limit w"].get<double>()
                );
    }
    throw std::runtime_error("unknown kernels");
}
GaussKernels::GaussKernels(double sigma_m, double sigma_w) : sigma_m_(sigma_m), sigma_w_(sigma_w),
    limit_m_(-1), limit_w_(-1) {
}
GaussKernels::GaussKernels(double sigma_m, double sigma_w, double limit_m, double limit_w) :
      sigma_m_(sigma_m), sigma_w_(sigma_w), limit_m_(limit_m), limit_w_(limit_w) {
}
double GaussKernels::m(double xi) const {
    if (limit_w_ > 0 && xi > limit_m_) {
        return 0;
    }
    return std::exp(-xi * xi / (2 * sigma_m_ * sigma_m_)) / sqrt(2 * M_PI * sigma_m_ * sigma_m_);
}
double GaussKernels::w(double xi) const {
    if (limit_w_ > 0 && xi > limit_w_) {
        return 0;
    }
    return std::exp(-xi * xi / (2 * sigma_w_ * sigma_w_)) / sqrt(2 * M_PI * sigma_w_ * sigma_w_);
}
