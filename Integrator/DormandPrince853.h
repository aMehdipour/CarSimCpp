#pragma once
#include <MathTools.h>
#include "Dopr853Constants.h"
#include "PIController.h"
#include "IntegratorBase.h"

namespace Dopr853 {

template <class DerivativeFunction>
struct DormandPrince853 : IntegratorBase {

    // Constructor.
    DormandPrince853(MathTools::StateVector& initialState, MathTools::DerivativeVector& initialStateDerivatives,
               double& initialTime, const double absoluteTolerance_, const double relativeTolerance_, bool flagDenseOutput);

    // Performs one integration step with a trial step size htry.
    void step(const double trialStepSize, DerivativeFunction& computeDerivatives);

    // Compute one integration step (all stages) with step size h.
    void computeStages(const double stepSize, DerivativeFunction& computeDerivatives);

    // Prepare dense output coefficients.
    void prepareDenseOutput(const double stepSize, MathTools::DerivativeVector& newDerivatives, DerivativeFunction& computeDerivatives);

    // Compute the dense output for the i-th component at x (within the current step of size h).
    double getDenseOutput(const int idx, const double time, const double stepSize);

    // Compute the error estimate for the current step.
    double computeError(const double stepSize);

    // Error estimators.
    MathTools::StateVector errorEstimate2_;

    // Intermediate slopes for the stages.
    MathTools::StateVector k2_, k3_, k4_, k5_, k6_, k7_, k8_, k9_, k10_;

    // Dense output coefficients.
    MathTools::StateVector denseCoeff1_, denseCoeff2_, denseCoeff3_, denseCoeff4_, denseCoeff5_, denseCoeff6_, denseCoeff7_, denseCoeff8_;

    Controller stepSizeController_;
};

//==============================================================================
// IMPLEMENTATION SECTION
//==============================================================================
//
//------------------------------------------------------------------------------
// Constructor: Initialize state, allocate working arrays.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
DormandPrince853<DerivativeFunction>::DormandPrince853(
    MathTools::StateVector& initialState, 
    MathTools::DerivativeVector& initialDerivatives, 
    double& initialTime,
    const double absoluteTol, 
    const double relativeTol, 
    bool enableDenseOutput ) :

    IntegratorBase(initialState, initialDerivatives, initialTime, absoluteTol, relativeTol, enableDenseOutput),
    errorEstimate2_(numStates_),
    k2_(numStates_), 
    k3_(numStates_), 
    k4_(numStates_), 
    k5_(numStates_),
    k6_(numStates_), 
    k7_(numStates_), 
    k8_(numStates_), 
    k9_(numStates_), 
    k10_(numStates_),
    denseCoeff1_(numStates_), 
    denseCoeff2_(numStates_), 
    denseCoeff3_(numStates_), 
    denseCoeff4_(numStates_),
    denseCoeff5_(numStates_), 
    denseCoeff6_(numStates_), 
    denseCoeff7_(numStates_), 
    denseCoeff8_(numStates_) { }

//------------------------------------------------------------------------------
// step: Try a step with trial step size, adjust if necessary.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
void DormandPrince853<DerivativeFunction>::step(const double stepSize, DerivativeFunction& derivativeFunction) {
    MathTools::DerivativeVector newDerivatives(stateVector_.size());
    double trialStepSize = stepSize;

    for (;;) {
        // Compute one complete step
        computeStages(trialStepSize, derivativeFunction); 

        // Check if error is acceptable
        double error = computeError(trialStepSize);
        if (stepSizeController_.isStepSuccessful(error, trialStepSize))
            break;

        // Check for step size underflow
        if (std::fabs(trialStepSize) <= std::fabs(time_) * Dopr853::EPS)
            throw std::runtime_error("Step size underflow in Dopr853 Integrator");
    }

    // Compute derivatives at the new state
    derivativeFunction(time_ + trialStepSize, stateVector_, newDerivatives);

    // Prepare dense output for interpolation if needed
    if (flagUseDenseOutput_)
        prepareDenseOutput(trialStepSize, newDerivatives, derivativeFunction);

    // Update for next step
    derivativeVector_ = newDerivatives;
    timePrev_ = time_;
    time_ += trialStepSize;
    stepSizeController_.nextStepSize_ = trialStepSize;
}

//------------------------------------------------------------------------------
// computeStages: Compute one step (all ks) of the integration.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
void DormandPrince853<DerivativeFunction>::computeStages(const double stepSize, DerivativeFunction& derivativeFunction) {
    const unsigned int stateSize = stateVector_.size();
    unsigned int i;
    MathTools::StateVector tempState(stateSize);

    // k 2
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * a21 * derivativeVector_[i];
    derivativeFunction(time_ + c2 * stepSize, tempState, k2_);

    // k 3
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a31 * derivativeVector_[i] + a32 * k2_[i]);
    derivativeFunction(time_ + c3 * stepSize, tempState, k3_);

    // k 4
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a41 * derivativeVector_[i] + a43 * k3_[i]);
    derivativeFunction(time_ + c4 * stepSize, tempState, k4_);

    // k 5
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a51 * derivativeVector_[i] + a53 * k3_[i] + a54 * k4_[i]);
    derivativeFunction(time_ + c5 * stepSize, tempState, k5_);

    // k 6
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a61 * derivativeVector_[i] + a64 * k4_[i] + a65 * k5_[i]);
    derivativeFunction(time_ + c6 * stepSize, tempState, k6_);

    // k 7
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a71 * derivativeVector_[i] + a74 * k4_[i] + a75 * k5_[i] + a76 * k6_[i]);
    derivativeFunction(time_ + c7 * stepSize, tempState, k7_);

    // k 8
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a81 * derivativeVector_[i] + a84 * k4_[i] + a85 * k5_[i] + a86 * k6_[i] + a87 * k7_[i]);
    derivativeFunction(time_ + c8 * stepSize, tempState, k8_);

    // k 9
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a91 * derivativeVector_[i] + a94 * k4_[i] + a95 * k5_[i] + a96 * k6_[i] +
            a97 * k7_[i] + a98 * k8_[i]);
    derivativeFunction(time_ + c9 * stepSize, tempState, k9_);

    // k 10
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a101 * derivativeVector_[i] + a104 * k4_[i] + a105 * k5_[i] + a106 * k6_[i] +
            a107 * k7_[i] + a108 * k8_[i] + a109 * k9_[i]);
    derivativeFunction(time_ + c10 * stepSize, tempState, k10_);

    // k 11 (reuse k2_)
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a111 * derivativeVector_[i] + a114 * k4_[i] + a115 * k5_[i] + a116 * k6_[i] +
            a117 * k7_[i] + a118 * k8_[i] + a119 * k9_[i] + a1110 * k10_[i]);
    derivativeFunction(time_ + c11 * stepSize, tempState, k2_);

    double newTimeVariable = time_ + stepSize;

    // k 12 (reuse k3_)
    for (i = 0; i < stateSize; i++)
        tempState[i] = stateVector_[i] + stepSize * (a121 * derivativeVector_[i] + a124 * k4_[i] + a125 * k5_[i] + a126 * k6_[i] +
            a127 * k7_[i] + a128 * k8_[i] + a129 * k9_[i] + a1210 * k10_[i] + a1211 * k2_[i]);
    derivativeFunction(newTimeVariable, tempState, k3_);

    // Final combination to produce the 8th order solution
    MathTools::StateVector finalCombination(stateSize);
    for (i = 0; i < stateSize; i++) {
        finalCombination[i] = b1 * derivativeVector_[i] + b6 * k6_[i] + b7 * k7_[i] + b8 * k8_[i] +
            b9 * k9_[i] + b10 * k10_[i] + b11 * k2_[i] + b12 * k3_[i];
        stateVector_[i] = stateVector_[i] + stepSize * finalCombination[i];
    }

    // Compute two error estimators
    for (i = 0; i < stateSize; i++) {
        errorEstimate1_[i] = finalCombination[i] - bhh1 * derivativeVector_[i] - bhh2 * k9_[i] - bhh3 * k3_[i];
        errorEstimate2_[i] = er1 * derivativeVector_[i] + er6 * k6_[i] + er7 * k7_[i] + er8 * k8_[i] +
            er9 * k9_[i] + er10 * k10_[i] + er11 * k2_[i] + er12 * k3_[i];
    }
}

//------------------------------------------------------------------------------
// prepareDenseOutput: Compute dense output coefficients for interpolation.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
void DormandPrince853<DerivativeFunction>::prepareDenseOutput(const double stepSize, 
                                                        MathTools::DerivativeVector& newDerivatives, 
                                                        DerivativeFunction& derivativeFunction) {
    int i;
    double stateDifference, bspline;
    MathTools::StateVector tempState(stateVector_.size());

    for (i = 0; i < stateVector_.size(); i++) {
        denseCoeff1_[i] = stateVector_[i];
        //TODO: Verify this is correct
        stateDifference = stateVectorOut_[i] - stateVector_[i];
        denseCoeff2_[i] = stateDifference;
        bspline = stepSize * derivativeVector_[i] - stateDifference;
        denseCoeff3_[i] = bspline;
        denseCoeff4_[i] = stateDifference - stepSize * newDerivatives[i] - bspline;
        denseCoeff5_[i] = d41 * derivativeVector_[i] + d46 * k6_[i] + d47 * k7_[i] + d48 * k8_[i] +
            d49 * k9_[i] + d410 * k10_[i] + d411 * k2_[i] + d412 * k3_[i];
        denseCoeff6_[i] = d51 * derivativeVector_[i] + d56 * k6_[i] + d57 * k7_[i] + d58 * k8_[i] +
            d59 * k9_[i] + d510 * k10_[i] + d511 * k2_[i] + d512 * k3_[i];
        denseCoeff7_[i] = d61 * derivativeVector_[i] + d66 * k6_[i] + d67 * k7_[i] + d68 * k8_[i] +
            d69 * k9_[i] + d610 * k10_[i] + d611 * k2_[i] + d612 * k3_[i];
        denseCoeff8_[i] = d71 * derivativeVector_[i] + d76 * k6_[i] + d77 * k7_[i] + d78 * k8_[i] +
            d79 * k9_[i] + d710 * k10_[i] + d711 * k2_[i] + d712 * k3_[i];
    }

    // Additional ks for dense output
    for (i = 0; i < stateVector_.size(); i++) {
        tempState[i] = stateVector_[i] + stepSize * (a141 * derivativeVector_[i] + a147 * k7_[i] + a148 * k8_[i] +
            a149 * k9_[i] + a1410 * k10_[i] + a1411 * k2_[i] +
            a1412 * k3_[i] + a1413 * newDerivatives[i]);
    }
    derivativeFunction(time_ + c14 * stepSize, tempState, k10_);

    for (i = 0; i < stateVector_.size(); i++) {
        tempState[i] = stateVector_[i] + stepSize * (a151 * derivativeVector_[i] + a156 * k6_[i] + a157 * k7_[i] +
            a158 * k8_[i] + a1511 * k2_[i] + a1512 * k3_[i] +
            a1513 * newDerivatives[i] + a1514 * k10_[i]);
    }
    derivativeFunction(time_ + c15 * stepSize, tempState, k2_);

    for (i = 0; i < stateVector_.size(); i++) {
        tempState[i] = stateVector_[i] + stepSize * (a161 * derivativeVector_[i] + a166 * k6_[i] + a167 * k7_[i] +
            a168 * k8_[i] + a169 * k9_[i] + a1613 * newDerivatives[i] +
            a1614 * k10_[i] + a1615 * k2_[i]);
    }
    derivativeFunction(time_ + c16 * stepSize, tempState, k3_);

    // Finalize dense output coefficients
    for (i = 0; i < stateVector_.size(); i++) {
        denseCoeff5_[i] = stepSize * (denseCoeff5_[i] + d413 * newDerivatives[i] + d414 * k10_[i] +
            d415 * k2_[i] + d416 * k3_[i]);
        denseCoeff6_[i] = stepSize * (denseCoeff6_[i] + d513 * newDerivatives[i] + d514 * k10_[i] +
            d515 * k2_[i] + d516 * k3_[i]);
        denseCoeff7_[i] = stepSize * (denseCoeff7_[i] + d613 * newDerivatives[i] + d614 * k10_[i] +
            d615 * k2_[i] + d616 * k3_[i]);
        denseCoeff8_[i] = stepSize * (denseCoeff8_[i] + d713 * newDerivatives[i] + d714 * k10_[i] +
            d715 * k2_[i] + d716 * k3_[i]);
    }
}

//------------------------------------------------------------------------------
// getDenseOutput: Return interpolated solution at time (within the current step of size h) for component i.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
double DormandPrince853<DerivativeFunction>::getDenseOutput(const int index, const double time, const double stepSize) {
    double normalizedTime = (time - timePrev_) / stepSize;
    double oneMinusTime = 1.0 - normalizedTime;

    double output = denseCoeff1_[index] + normalizedTime * (
        denseCoeff2_[index] + oneMinusTime * (
        denseCoeff3_[index] + normalizedTime * (
        denseCoeff4_[index] + oneMinusTime * (
        denseCoeff5_[index] + normalizedTime * (
        denseCoeff6_[index] + oneMinusTime * (
        denseCoeff7_[index] + normalizedTime * denseCoeff8_[index]
    )
    )
    )
    )
    )
    );

    return output;
}   

//------------------------------------------------------------------------------
// computeError: Compute and return the error estimate for the current step.
//------------------------------------------------------------------------------
template <class DerivativeFunction>
double DormandPrince853<DerivativeFunction>::computeError(const double stepSize) {
    double errorNorm = 0.0, errorNorm2 = 0.0;
    double scaleFactor, denominator;

    for (int i = 0; i < numStates_; i++) {
        // Compute scale factor using absolute and relative tolerances
        scaleFactor = std::max(absoluteTolerance_, relativeTolerance_ * std::max(std::fabs(stateVector_[i]), std::fabs(stateVector_[i])));

        // Compute error norms (scaled)
        errorNorm += (errorEstimate1_[i] / scaleFactor) * (errorEstimate1_[i] / scaleFactor);
        errorNorm2 += (errorEstimate2_[i] / scaleFactor) * (errorEstimate2_[i] / scaleFactor);
    }

    // Compute denominator for combined error estimate
    denominator = errorNorm + 0.01 * errorNorm2;
    if (denominator <= 0.0)
        denominator = 1.0;

    // Return combined and scaled error estimate
    return std::fabs(stepSize) * errorNorm * std::sqrt(1.0 / (stateVector_.size() * denominator));
}
}
