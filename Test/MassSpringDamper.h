#pragma once

#include "MathTools.h"

class MassSpringDamper {
public:
    double m, k, b, F, w;

    MassSpringDamper(double mass, double stiffness, double damping, double force, double omega)
        : m(mass), k(stiffness), b(damping), F(force), w(omega) {}

    // Define system dynamics for the integrator
    void operator()(double t, MathTools::StateVector& state, MathTools::DerivativeVector& dstate_dt) const {
        double x = state(0);
        double v = state(1);

        dstate_dt(0) = v;
        dstate_dt(1) = (F * std::sin(w * t) - k * x - b * v) / m;
    }

    MathTools::StateVector exact_solution(double t, double m, double k, double b, double F, double w, 
                                   double x0, double v0) {
        double wn = std::sqrt(k / m);
        double zeta = b / (2 * std::sqrt(k * m));
        double wd = wn * std::sqrt(1 - zeta * zeta);

        double X = (F / m) / std::sqrt((wn * wn - w * w) * (wn * wn - w * w) + (2 * zeta * wn * w) * (2 * zeta * wn * w));
        double phi = std::atan2(2 * zeta * wn * w, wn * wn - w * w);

        double C1 = x0 - X * std::sin(phi);
        double C2 = (v0 + zeta * wn * C1 + w * X * std::cos(phi)) / wd;

        double x = std::exp(-zeta * wn * t) * (C1 * std::cos(wd * t) + C2 * std::sin(wd * t)) + X * std::sin(w * t + phi);
        double v = -zeta * wn * std::exp(-zeta * wn * t) * (C1 * std::cos(wd * t) + C2 * std::sin(wd * t))
            + wd * std::exp(-zeta * wn * t) * (-C1 * std::sin(wd * t) + C2 * std::cos(wd * t))
            + X * w * std::cos(w * t + phi);

        return MathTools::StateVector(x, v);
    }

};
