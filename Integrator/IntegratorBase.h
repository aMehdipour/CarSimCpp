#pragma once
#include <MathTools.h>

class IntegratorBase {
public:

    // Constructor.
    IntegratorBase(
        MathTools::StateVector& initialState,
        MathTools::DerivativeVector& initialDerivatives,
        double& initialTime,
        const double absoluteTolerance,
        const double relativeTolerance,
        bool flagDenseOutput) :

        stateVector_(initialState),
        errorEstimate1_(initialState.size()),
        stateVectorOut_(initialState.size()),
        derivativeVector_(initialDerivatives),
        time_(initialTime),
        timePrev_(initialTime),
        absoluteTolerance_(absoluteTolerance),
        relativeTolerance_(relativeTolerance),
        flagUseDenseOutput_(flagDenseOutput),
        numStates_(initialState.size()),
        numEquations_(initialDerivatives.size()) { }

    MathTools::StateVector stateVector_;
    MathTools::StateVector stateVectorOut_;
    MathTools::StateVector errorEstimate1_;

    MathTools::DerivativeVector derivativeVector_;

    double time_;
    double timePrev_;
    double absoluteTolerance_;
    double relativeTolerance_;
    double actualStepSize_;
    double predictedStepSize_;

    bool flagUseDenseOutput_;

    int numStates_;
    int numEquations_;
};
