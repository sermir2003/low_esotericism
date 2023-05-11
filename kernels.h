#pragma once
#include <memory>
#include <nlohmann/json.hpp>

struct GaussKernels {
    double sigma_squared_m;
    double sigma_squared_w;

    GaussKernels() {}
    GaussKernels(double sigma_m, double sigma_w);
    double m(double xi) const;
    double w(double xi) const;
};
