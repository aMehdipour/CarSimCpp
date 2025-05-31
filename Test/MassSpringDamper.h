#pragma onc

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
        // Natural frequency and damping ratio
        const double wn   = std::sqrt(k / m);
        const double zeta = b / (2.0 * std::sqrt(k * m));

        // Handle all damping cases uniformly
        double expTerm   = 0.0;
        double homX      = 0.0;
        double homV      = 0.0;

        // Forced response quantities (valid for any ζ)
        const double X = (F / m) /
            std::sqrt((wn * wn - w * w) * (wn * wn - w * w) +
                      (2.0 * zeta * wn * w) * (2.0 * zeta * wn * w));
        const double phi = std::atan2(2.0 * zeta * wn * w, wn * wn - w * w); // lead angle

        if (std::fabs(zeta - 1.0) < 1e-6) {
            // Critical damping (ζ ≈ 1)
            const double A = x0 + X * std::sin(phi);
            const double B = v0 + wn * A - X * w * std::cos(phi);

            expTerm = std::exp(-wn * t);
            homX    = expTerm * (A + B * t);
            homV    = expTerm * (B - wn * (A + B * t));
        }
        else if (zeta < 1.0) {
            // Under‑damped (ζ < 1)
            const double wd = wn * std::sqrt(1.0 - zeta * zeta);

            const double C1 = x0 + X * std::sin(phi);
            const double C2 = (v0 + zeta * wn * C1 - X * w * std::cos(phi)) / wd;

            expTerm = std::exp(-zeta * wn * t);
            homX    = expTerm * (C1 * std::cos(wd * t) + C2 * std::sin(wd * t));
            homV    = expTerm * (-zeta * wn * (C1 * std::cos(wd * t) + C2 * std::sin(wd * t)) +
                wd * (-C1 * std::sin(wd * t) + C2 * std::cos(wd * t)));
        }
        else {
            // Over‑damped (ζ > 1)
            const double s   = std::sqrt(zeta * zeta - 1.0);
            const double r1  = -wn * (zeta - s);
            const double r2  = -wn * (zeta + s);

            const double C1 = (v0 - r2 * (x0 + X * std::sin(phi))) / (r1 - r2);
            const double C2 = x0 + X * std::sin(phi) - C1;

            homX = C1 * std::exp(r1 * t) + C2 * std::exp(r2 * t);
            homV = C1 * r1 * std::exp(r1 * t) + C2 * r2 * std::exp(r2 * t);
        }

        // Particular (steady‑state) response
        const double partX = X * std::sin(w * t - phi);
        const double partV = X * w * std::cos(w * t - phi);

        return MathTools::StateVector(homX + partX, homV + partV);
    }

};
