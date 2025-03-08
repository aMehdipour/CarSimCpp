#pragma once
#include <MathTools.h>

namespace Dopr853 {

template <class D>
struct Integrator {
    typedef D Dtype;

    // Current state and derivatives.
    MathTools::StateVector stateVector;     // current state vector
    MathTools::DerivativeVector derivativeVector;  // derivative at the current state
    double x;        // current independent variable (e.g., time)
    double xold;     // previous x value

    // Error estimators.
    MathTools::StateVector stateError1;
    MathTools::StateVector stateError2; // Second error estimator

    // Intermediate slopes for the stages.
    MathTools::StateVector k2, k3, k4, k5, k6, k7, k8, k9, k10;

    // Dense output coefficients.
    MathTools::StateVector rcont1, rcont2, rcont3, rcont4, rcont5, rcont6, rcont7, rcont8;

    // Constructor.
    Integrator(MathTools::StateVector &yy, MathTools::DerivativeVector &dydxx, double &xx,
               const double atoll, const double rtoll, bool dens);

    // Performs one integration step with a trial step size htry.
    void step(const double htry, D &derivs);

    // Compute one integration step (all stages) with step size h.
    void dy(const double h, D &derivs);

    // Prepare dense output coefficients.
    void prepare_dense(const double h, MathTools::DerivativeVector &dydxnew, D &derivs);

    // Compute the dense output for the i-th component at x (within the current step of size h).
    double dense_out(const int i, const double x, const double h);

    // Compute the error estimate for the current step.
    double error(const double h);

    // Nested Controller structure for adaptive step size control.
    struct Controller {
        double hnext;   // suggested next step size
        double errold;  // previous error
        bool reject;    // flag indicating if the step was rejected

        Controller();
        bool success(const double err, double &h);
    };
};

}
