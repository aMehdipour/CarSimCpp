#pragma once
#include <memory>

template <typename CarModel, typename IntegratorType>
class Sim {
private:
    CarModel car_;
    IntegratorType integrator_;

public:
    Sim() 
        : car_(), 
        integrator_() {}

    void run() {
        car_.simulate();
        integrator_.integrate();
    }};
