#pragma once

namespace Dopr853 {
    struct Controller {
        Controller();

        bool isStepSuccessful(const double err, double &h);
        void initialize();

        double nextStepSize_; // suggested next step size
        double errorPrev_;    // previous error
        bool flagRejectStep_; // flag indicating if the step was rejected

    };
}
