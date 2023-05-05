#pragma once
#include <memory>
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

class Kernels {
public:
    virtual double m(double xi) const = 0;
    virtual double w(double xi) const = 0;
    virtual ~Kernels();
};

std::unique_ptr<Kernels> MakeKernels(const Json& kernel_data);

class GaussKernels : public Kernels {
private:
    double sigma_m_;
    double sigma_w_;
    double limit_m_;
    double limit_w_;
public:
    GaussKernels(double sigma_m, double sigma_w);
    GaussKernels(double sigma_m, double sigma_w, double limit_m, double limit_w);
    double m(double xi) const override;
    double w(double xi) const override;
};
