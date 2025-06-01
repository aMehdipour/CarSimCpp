#include "DormandPrince853.h"
#include "BicycleModel.h"
#include "MFTireModel.h"
#include "MathTools.h"
#include "Telemetry.h"
#include "Sim.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <json.hpp>

using json = nlohmann::json;
int main() {
    // std::cout << "CarSimCpp - Vehicle Dynamics Simulation" << std::endl;
    // std::cout << "======================================" << std::endl;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%m_%d_%Y_%H_%M_%S");
    auto str = oss.str();
    std::string outDir = "Output/" + str + "/";
    // std::string outDir = "Output/";

    // Create output directory if it doesn't exist
    std::filesystem::create_directories(outDir);

    double t0 = 0.0;
    double tf = 15.0;
    double initialStepSize = 0.01;
    double minStepSize = 1e-10;
    double absoluteTol = 1e-6;
    double relativeTol = 1e-6;

    // Number of output points
    int numOutputPoints = 1000;

    // Create output handler
    Output output(numOutputPoints, 2000);

    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open config.json" << std::endl;
        return 69;
    }

    json config;
    configFile >> config;

    BicycleModel<MFTireModel> bicycleModel;
    bicycleModel.configure(config);

    // Initial conditions
    MathTools::StateVector initialState(0., 0., 0., 0., 0., 10.); // x, y, psi, beta, r, vxBody
    MathTools::DerivativeVector initialDerivatives(0., 0., 0., 0., 0., 0.); // dx, dy, dpsi, dbeta, dr

    bicycleModel(t0, initialState, initialDerivatives);

    Sim< Dopr853::DormandPrince853<BicycleModel<MFTireModel>> > solver(
        initialState, initialDerivatives, t0, tf, absoluteTol, relativeTol,
        initialStepSize, minStepSize, output, bicycleModel);

    solver.integrate(output, bicycleModel);
    output.writeToFile(outDir + "sim_out.csv");

    // std::cout << "Simulation complete. Data saved to CSV files in the Output directory..." << std::endl;
    // std::cout << "Outputs written to: " << outDir << std::endl;
}
