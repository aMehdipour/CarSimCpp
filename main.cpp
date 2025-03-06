#include "BicycleModel.h"
#include "HighFidelity.h"
#include "EulerIntegrator.h"
#include "RK4Integrator.h"
#include "Sim.h"

// Define SelectedCarModel and SelectedIntegrator
using SelectedCarModel = CAR_MODEL;
using SelectedIntegrator = INTEGRATOR_MODEL;

int main() {
    Sim<SelectedCarModel, SelectedIntegrator> sim;
    sim.run();
    return 0;
}

