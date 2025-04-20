//------------------------------------------------------------------------------
// Controller: Constructor for adaptive step size controller.
#include "Dopr853Constants.h"
#include "PIController.h"
#include <cmath>

namespace Dopr853 {

//------------------------------------------------------------------------------
// Controller::Controller: Constructor for adaptive step size controller.
//------------------------------------------------------------------------------
Controller::Controller() 
: nextStepSize_(Dopr853::NEXT_STEP_SIZE), errorPrev_(Dopr853::ERROR_PREV), flagRejectStep_(false) {}

//------------------------------------------------------------------------------
// Controller::initialize
//------------------------------------------------------------------------------
void Controller::initialize() {
    nextStepSize_   = Dopr853::NEXT_STEP_SIZE;
    errorPrev_      = Dopr853::ERROR_PREV;
    flagRejectStep_ = false;
}

//------------------------------------------------------------------------------
// Controller::isStepSuccessful: Determine if the step is acceptable and compute new step size.
//------------------------------------------------------------------------------
bool Controller::isStepSuccessful(const double error, double &stepSize) {

    const double beta = 0.;//0.4 / 8.;
    const double alpha = 1.0/8.0 - beta * 0.2;
    const double safetyFactor = 0.9;
    const double minScaleFactor = 0.333;
    const double maxScaleFactor = 6.0;

    double scaleFactor;

    if (error <= 1.0) {
        // Step is acceptable
        if (error == 0.0)
            scaleFactor = maxScaleFactor;
        else {
            // Compute new step size scaling factor based on error
            scaleFactor = safetyFactor * std::pow(error, -alpha) * std::pow(errorPrev_, beta);
            scaleFactor = std::max(minScaleFactor, std::min(scaleFactor, maxScaleFactor));
        }

        // Set next step size
        if (flagRejectStep_)
            nextStepSize_ = stepSize * std::min(scaleFactor, 1.0);
        else
            nextStepSize_ = stepSize * scaleFactor;

        errorPrev_ = std::max(error, 1.0e-4);
        flagRejectStep_ = false;
        return true;
    } else {
        // Step is rejected, reduce step size
        scaleFactor = std::max(safetyFactor * std::pow(error, -alpha), minScaleFactor);
        stepSize *= scaleFactor;
        flagRejectStep_ = true;
        return false;
    }
}
}

