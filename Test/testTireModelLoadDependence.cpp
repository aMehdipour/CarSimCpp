#include "MFTireModel.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>
#include <vector>

using json = nlohmann::json;

int main() {
  std::cout << "Tire Model Load Dependence Test" << std::endl;
  std::cout << "===============================" << std::endl;

  MFTireModel tireModel;

  // Load configuration
  std::ifstream configFile("config.json");
  json config;
  configFile >> config;
  json tireParams = config["tire"];

  if (!tireModel.configure(tireParams)) {
    std::cerr << "Failed to configure tire model" << std::endl;
    return 1;
  }

  // Test parameters
  double vx = 30.0;
  double vy = 0.0;
  double camberAngle = 0.0;
  double slipAngle = 0.0;

  // Test different loads at specific slip ratios
  std::vector<double> testLoads = {50, 100, 150, 200, 250, 300}; // lbs
  std::vector<double> testSlipRatios = {-0.1, -0.05, 0.0, 0.05, 0.1};

  std::cout << "\nFx values for different loads and slip ratios:" << std::endl;
  std::cout << std::string(80, '=') << std::endl;

  // Header
  std::cout << std::setw(10) << "Load(lbs)";
  for (double sr : testSlipRatios) {
    std::cout << std::setw(12) << "SR=" << std::fixed << std::setprecision(2)
              << sr;
  }
  std::cout << std::endl;
  std::cout << std::string(80, '-') << std::endl;

  for (double load_lbs : testLoads) {
    double load_N = load_lbs * 4.44822;
    std::cout << std::setw(10) << std::fixed << std::setprecision(0)
              << load_lbs;

    for (double sr : testSlipRatios) {
      double Fx = 0.0, Fy = 0.0;
      tireModel.computeTireForces(Fx, Fy, vx, vy, camberAngle, sr, slipAngle,
                                  load_N);

      std::cout << std::setw(15) << std::fixed << std::setprecision(1) << Fx;
    }
    std::cout << std::endl;
  }

  std::cout << std::string(80, '=') << std::endl;

  // Now let's trace what happens for 50 lbs at slip ratio 0.1
  std::cout << "\nDetailed calculation for 50 lbs at slip ratio 0.1:"
            << std::endl;
  std::cout << std::string(50, '-') << std::endl;

  double load_N = 50 * 4.44822;
  double sr = 0.1;

  // Calculate intermediate values (approximations based on the formula)
  double FNOMIN = 1100.0;
  double dfz = (load_N - FNOMIN) / FNOMIN;

  std::cout << "Normal load: " << load_N << " N (" << load_N / 4.44822
            << " lbs)" << std::endl;
  std::cout << "FNOMIN: " << FNOMIN << " N" << std::endl;
  std::cout << "dfz = (Fz - FNOMIN) / FNOMIN = " << dfz << std::endl;
  std::cout << "Note: dfz = " << dfz << " is very negative for light loads!"
            << std::endl;

  return 0;
}