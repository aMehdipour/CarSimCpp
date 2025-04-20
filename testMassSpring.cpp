/*#include "DormandPrince853.h"*/
/*#include "MassSpringDamper.h"*/
/*#include <MathTools.h>*/
/*#include "Telemetry.h"*/
/*#include <iostream>*/
/**/
/*int main() {*/
/*    MathTools::StateVector state;*/
/*    MathTools::DerivativeVector derivatives;*/
/*    MathTools::DerivativeVector initialStateDerivatives;*/
/*    // System parameters*/
/*    double m = 1.0, k = 10.0, b = 0.5, F = 5.0, w = 2.0;*/
/*    MassSpringDamper system(m, k, b, F, w);*/
/**/
/*    // Initial conditions*/
/*    state << 1.0, 0.0;  // x0 = 1.0, v0 = 0.0*/
/*    derivatives << 0.0, 0.0;*/
/*    double t0 = 0.0, tf = 10.0, dt = 0.01;*/
/*    double t = t0;*/
/*    Dopr853::DormandPrince853<MassSpringDamper> integrator(state, derivatives, t0, 1e-6, 1e-6, true);*/
/**/
/*    // Setup telemetry logger*/
/*    Telemetry telemetry("Output/simulation_output.csv", {"Time", "Num_X", "Num_V", "Exact_X", "Exact_V", "Error_X", "Error_V"});*/
/**/
/*    // Simulation loop*/
/*    while (t < tf) {*/
/*        // Store exact solution*/
/*        Eigen::Vector2d exact = system.exact_solution(t, m, k, b, F, w, state(0), state(1));*/
/**/
/*        // Integrate system*/
/*        integrator.step(dt, system);*/
/*        t += dt;*/
/**/
/*        // Compute error*/
/*        double error_x = std::abs(state(0) - exact(0));*/
/*        double error_v = std::abs(state(1) - exact(1));*/
/**/
/**/
/*        // Log to file*/
/*        telemetry.log({t, state(0), state(1), exact(0), exact(1), error_x, error_v});*/
/**/
/*    }*/
/**/
/*    std::cout << "Simulation complete. Data saved to 'simulation_output.csv'.\n";*/
/*    return 0;*/
/*}*/
#include "DormandPrince853.h"
#include "MassSpringDamper.h"
#include "MathTools.h"
#include "Telemetry.h"
#include "Sim.h"
#include <iostream>
#include <filesystem>

int main() {
    // Create output directory if it doesn't exist
    std::filesystem::create_directories("Output");

    // System parameters
    double m = 1.0, k = 10.0, b = 0.5, F = 5.0, w = 2.0;
    MassSpringDamper system(m, k, b, F, w);

    // Initial conditions
    MathTools::StateVector initialState(1., 0.); // x0 = 1.0, v0 = 0.0
    
    // Integration parameters
    double t0 = 0.0, tf = 10.0;
    double initialStepSize = 0.01;
    double minStepSize = 1e-10;
    double absoluteTol = 1e-6;
    double relativeTol = 1e-6;
    
    // Number of output points
    int numOutputPoints = 1000;
    
    // Create output handler
    Output numericalOutput(numOutputPoints);
    
    // Create exact solution output
    Output exactOutput(numOutputPoints);
    exactOutput.initialize(2, t0, tf);  // Same dimensions as numerical
    
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
    
    for (int i = 0; i < numSteps; i++) {
        double t = timePoints[i];
        MathTools::StateVector exactState = system.exact_solution(t, m, k, b, F, w, initialState(0), initialState(1));
        exactOutput.save(t, exactState);
    }
    
    // Write results to separate files
    numericalOutput.writeToFile("Output/numerical_solution.csv");
    exactOutput.writeToFile("Output/exact_solution.csv");
    
    // Also create a comparison file with both solutions and error
    Telemetry comparison("Output/comparison.csv", 
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
    
    std::cout << "Simulation complete. Data saved to CSV files in the Output directory.\n";
    return 0;
}

