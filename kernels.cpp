#include "kernels.h"
#include <cmath>

GaussKernels::GaussKernels(double sigma_m, double sigma_w) :
      sigma_squared_m(sigma_m * sigma_m),
      sigma_squared_w(sigma_w * sigma_w) {
}
double GaussKernels::m(double xi) const {
    return exp(-xi * xi / (2 * sigma_squared_m)) / sqrt(2 * M_PI * sigma_squared_m);
}
double GaussKernels::w(double xi) const {
    return exp(-xi * xi / (2 * sigma_squared_w)) / sqrt(2 * M_PI * sigma_squared_w);
}
