#include "MFTireModel.h"
#include <MathTools.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

int main() {
  std::cout << "Tire Model Detailed Debug - Tracing Fx Calculation"
            << std::endl;
  std::cout << "=================================================="
            << std::endl;

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
  double normalLoad = 100 * 4.44822; // 100 lbs in N
  double vx = 30.0;
  double vy = 0.0;
  double camberAngle = 0.0;
  double slipAngle = 0.0;

  std::cout << "\nTest conditions:" << std::endl;
  std::cout << "  Normal load: " << normalLoad << " N (" << normalLoad / 4.44822
            << " lbs)" << std::endl;
  std::cout << "  Velocity: vx=" << vx << " m/s, vy=" << vy << " m/s"
            << std::endl;
  std::cout << "  Slip angle: " << slipAngle << " rad" << std::endl;
  std::cout << "  Camber angle: " << camberAngle << " rad" << std::endl;

  // Test specific slip ratios around the discontinuity
  std::vector<double> testSlipRatios = {-0.01, -0.005, -0.001, 0.0,
                                        0.001, 0.005,  0.01};

  std::cout << "\nDetailed calculations for each slip ratio:" << std::endl;
  std::cout << std::string(80, '=') << std::endl;

  double SHX = 0.0; // Declare here for use after the loop

  for (double slipRatio : testSlipRatios) {
    std::cout << "\nSlip Ratio = " << std::setw(8) << slipRatio << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    // We need to trace the calculation step by step
    // Based on MFTireModel.cpp, the key calculations are:

    // From the debug output, we know:
    // PHX1 = -0.000114831, PHX2 = 0.00153695
    // For 100 lbs: dfz = (444.822 - 1100) / 1100 = -0.595616
    double dfz = (normalLoad - 1100.0) / 1100.0;
    SHX = (-0.000114831 + 0.00153695 * dfz) * 1.0; // LHX = 1.0
    double KAPPAX = slipRatio + SHX;

    std::cout << "  dfz = " << dfz << std::endl;
    std::cout << "  SHX = " << SHX << std::endl;
    std::cout << "  KAPPAX = slipRatio + SHX = " << slipRatio << " + " << SHX
              << " = " << KAPPAX << std::endl;
    std::cout << "  sign(KAPPAX) = " << MathTools::sign(KAPPAX) << std::endl;

    // PEX4 = -0.106218
    double PEX4 = -0.106218;
    double EX_factor = (1.0 - PEX4 * MathTools::sign(KAPPAX));
    std::cout << "  EX factor = (1 - PEX4 * sign(KAPPAX)) = (1 - " << PEX4
              << " * " << MathTools::sign(KAPPAX) << ") = " << EX_factor
              << std::endl;

    // Compute actual forces
    double Fx = 0.0, Fy = 0.0;
    tireModel.computeTireForces(Fx, Fy, vx, vy, camberAngle, slipRatio,
                                slipAngle, normalLoad);

    std::cout << "  Resulting Fx = " << Fx << " N" << std::endl;
    std::cout << "  Resulting Fy = " << Fy << " N" << std::endl;
  }

  std::cout << "\n" << std::string(80, '=') << std::endl;
  std::cout << "\nKey observations:" << std::endl;
  std::cout << "- SHX offset is approximately " << SHX << std::endl;
  std::cout << "- KAPPAX crosses zero at slip ratio ≈ " << -SHX << std::endl;
  std::cout << "- The sign(KAPPAX) discontinuity occurs at this point"
            << std::endl;

  return 0;
}