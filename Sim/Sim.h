#pragma once
#include "GlobalConstants.h"
#include "MathTools.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

template <typename IntegratorType>
class Sim {
public:
    template <class DerivativeFunction, class OutputHandler>
    Sim(MathTools::StateVector& initialState, MathTools::DerivativeVector& initialDerivatives,
           const double startTime, const double endTime,
           const double absoluteTolerance, const double relativeTolerance,
           const double initialStepSize, const double minStepSize,
           OutputHandler& output, DerivativeFunction& derivativeFunction);

    template <class DerivativeFunction, class OutputHandler>
    void integrate(OutputHandler&  output, DerivativeFunction& derivativeFunction);
private:

    int numSuccessfulSteps_;
    int numFailedSteps_;
    int numStates_;
    int stepCount_;

    bool flagUseDenseOutput_;

    double startTime_;
    double endTime_;
    double currentTime_;
    double stepSize_;
    double minStepSize_;

    MathTools::StateVector initialState_;
    MathTools::StateVector stateVector_;
    MathTools::DerivativeVector derivativeVector_;

    IntegratorType integrator_;
};

// Constructor
template <class IntegratorType>
template <class DerivativeFunction, class OutputHandler>
Sim<IntegratorType>::Sim(
    MathTools::StateVector& initialState, MathTools::DerivativeVector& initialDerivatives,
    const double startTime, const double endTime,
    const double absoluteTolerance, const double relativeTolerance,
    const double initialStepSize, const double minStepSize,
    OutputHandler& output, DerivativeFunction& derivativeFunction)
    :
    numStates_(initialState.size()),
    stateVector_(initialState),
    initialState_(initialState),
    derivativeVector_(initialDerivatives),
    currentTime_(startTime),
    numSuccessfulSteps_(0),
    numFailedSteps_(0),
    startTime_(startTime),
    endTime_(endTime),
    minStepSize_(minStepSize),
    flagUseDenseOutput_(output.getUseDenseOutput()),
    integrator_(stateVector_, derivativeVector_, currentTime_,
                absoluteTolerance, relativeTolerance, flagUseDenseOutput_)
{
    stepSize_    = (endTime > startTime) ? initialStepSize : -initialStepSize;
    stateVector_ = initialState_;
    output.initialize(numStates_, startTime, endTime);
}

template <class IntegratorType>
template <class DerivativeFunction, class OutputHandler>
void Sim<IntegratorType>::integrate(OutputHandler& output, DerivativeFunction& derivativeFunction) {
    derivativeFunction(currentTime_, stateVector_, derivativeVector_);

    if (flagUseDenseOutput_) {
        output.processOutput(-1, currentTime_, stateVector_, integrator_, stepSize_);
    } else {
        output.save(currentTime_, stateVector_);
    }

    for (stepCount_ = 0; stepCount_ < GlobalConstants::MAX_STEPS; ++stepCount_) {
        if ((currentTime_ + stepSize_ * 1.001 - endTime_) * (endTime_ - startTime_) > 0.) {
            stepSize_ = endTime_ - currentTime_;
        }

        integrator_.step(stepSize_, derivativeFunction);

        if (integrator_.stepSizeController_.nextStepSize_ == stepSize_) {
            ++numSuccessfulSteps_;
        } else {
            ++numFailedSteps_;
        }

        currentTime_ += integrator_.stepSizeController_.nextStepSize_;

        if (flagUseDenseOutput_) {
            output.processOutput(stepCount_, currentTime_, stateVector_, integrator_, stepSize_);
        } else {
            output.save(currentTime_, stateVector_);
        }

        if ((currentTime_ - endTime_) * (endTime_ - startTime_) >= 0.) {
            initialState_ = stateVector_;

            if (output.getMaxPoints() > 0
             && std::abs(output.getLastSavedTime() - endTime_) > 100. * std::abs(endTime_) * GlobalConstants::EPSILON) {
                output.save(currentTime_, stateVector_);
            }

            return;
        }

        if (std::abs(integrator_.stepSizeController_.nextStepSize_) <= minStepSize_) {
            std::cerr << "[Sim.h] ERROR Step size too small!!! (That's what she said)" << std::endl;
            return;
        }

        stepSize_ = integrator_.stepSizeController_.nextStepSize_;
    }

    std::cerr << "[Sim.h] ERROR Reached max integration steps!!!" << std::endl;
    return;
}
