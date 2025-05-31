#include "DormandPrince853.h"
#include "MassSpringDamper.h"
#include "MathTools.h"
#include "Telemetry.h"
#include "Sim.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <sstream>

int main() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%m_%d_%Y_%H_%M_%S");
    auto str = oss.str();
    std::string outDir = "Output/" + str + "/";

    // Create output directory if it doesn't exist
    std::filesystem::create_directories(outDir);

    // System parameters
    double m = 1.0, k = 10.0, b = 0.5, F = 5.0, w = 2.0;
    MassSpringDamper system(m, k, b, F, w);

    // Initial conditions
    MathTools::StateVector initialState(0., 0.); // x0 = 1.0, v0 = 0.0
    MathTools::DerivativeVector derivatives(0., 0.);

    // Integration parameters
    double t0 = 0.0, tf = 10.0;
    double initialStepSize = 0.01;
    double minStepSize = 1e-10;
    double absoluteTol = 1e-6;
    double relativeTol = 1e-6;

    // Number of output points
    int numOutputPoints = 1000;

    // Create output handler
    Output numericalOutput(numOutputPoints, 2000);

    // Create exact solution output
    Output exactOutput(numOutputPoints, 2000);
    exactOutput.initialize(2, t0, tf);  // Same dimensions as numerical

    /*system(t0, initialState, derivatives);*/

    // Create ODE integrator
    Sim<Dopr853::DormandPrince853<MassSpringDamper>> solver(
        initialState, t0, tf, absoluteTol, relativeTol,
        initialStepSize, minStepSize, numericalOutput, system
    );

    // Solve the ODE
    solver.integrate(numericalOutput, system);

    // Now compute exact solution at the same time points
    const std::vector<double>& timePoints = numericalOutput.getTimePoints();
    const int numSteps = numericalOutput.getSavedCount();

    std::cout << "Integration complete. Saved " << numSteps << " points." << std::endl;

    for (int i = 0; i < numSteps; i++) {
        double t = timePoints[i];
        MathTools::StateVector exactState = system.exact_solution(t, m, k, b, F, w, initialState(0), initialState(1));
        exactOutput.save(t, exactState);
    }

    // Write results to separate files
    numericalOutput.writeToFile(outDir + "numerical_solution.csv");
    exactOutput.writeToFile(outDir + "exact_solution.csv");

    // Also create a comparison file with both solutions and error
    Telemetry comparison(outDir + "comparison.csv",
                        {"Time", "Num_X", "Num_V", "Exact_X", "Exact_V", "Error_X", "Error_V"});

    const std::vector<std::vector<double>>& numStates = numericalOutput.getStatePoints();
    const std::vector<std::vector<double>>& exactStates = exactOutput.getStatePoints();

    for (int i = 0; i < numSteps; i++) {
        double t = timePoints[i];
        double num_x = numStates[0][i];
        double num_v = numStates[1][i];
        double exact_x = exactStates[0][i];
        double exact_v = exactStates[1][i];
        double error_x = std::abs(num_x - exact_x);
        double error_v = std::abs(num_v - exact_v);

        comparison.log({t, num_x, num_v, exact_x, exact_v, error_x, error_v});
    }

    std::cout << "Simulation complete. Data saved to CSV files in the Output directory..." << std::endl;
    std::cout << "Outputs written to: " << outDir << std::endl;
}
