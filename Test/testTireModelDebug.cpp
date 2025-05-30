#include "MFTireModel.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

int main() {
  std::cout << "Tire Model Debug Test - Zero Slip Ratio Region" << std::endl;
  std::cout << "==============================================" << std::endl;

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
  double vy = 0.0; // Zero slip angle
  double camberAngle = 0.0;
  double slipAngle = 0.0;

  // Open debug output file
  std::ofstream debugFile("tire_debug_zero_region.csv");
  debugFile << "SlipRatio,Fx_N,Fy_N,KAPPAX,Sign_KAPPAX" << std::endl;

  // Test very fine resolution around zero
  std::cout << "\nTesting slip ratio range: -0.05 to 0.05" << std::endl;
  std::cout << std::setw(15) << "Slip Ratio" << std::setw(15) << "Fx (N)"
            << std::setw(15) << "Fy (N)" << std::endl;
  std::cout << std::string(45, '-') << std::endl;

  // Very fine steps around zero
  for (double sr = -0.05; sr <= 0.05; sr += 0.001) {
    double Fx = 0.0, Fy = 0.0;

    tireModel.computeTireForces(Fx, Fy, vx, vy, camberAngle, sr, slipAngle,
                                normalLoad);

    // Calculate KAPPAX for debugging (approximation since we don't have SHX)
    double KAPPAX = sr; // Assuming SHX is small
    int sign_kappax = (KAPPAX > 0) ? 1 : ((KAPPAX < 0) ? -1 : 0);

    debugFile << sr << "," << Fx << "," << Fy << "," << KAPPAX << ","
              << sign_kappax << std::endl;

    // Print key points
    if (std::abs(sr) < 0.001 || std::abs(sr - 0.01) < 0.0001 ||
        std::abs(sr + 0.01) < 0.0001 || std::abs(sr - 0.02) < 0.0001 ||
        std::abs(sr + 0.02) < 0.0001) {
      std::cout << std::fixed << std::setprecision(6) << std::setw(15) << sr
                << std::setw(15) << Fx << std::setw(15) << Fy << std::endl;
    }
  }

  debugFile.close();

  // Test exact values that showed problems
  std::cout << "\n\nTesting specific problematic values:" << std::endl;
  std::cout << std::string(45, '-') << std::endl;

  std::vector<double> testPoints = {-0.05,  -0.04, -0.03, -0.02, -0.01,
                                    -0.001, 0.0,   0.001, 0.01,  0.02};

  for (double sr : testPoints) {
    double Fx = 0.0, Fy = 0.0;
    tireModel.computeTireForces(Fx, Fy, vx, vy, camberAngle, sr, slipAngle,
                                normalLoad);

    std::cout << "SR = " << std::setw(7) << sr << " : Fx = " << std::setw(10)
              << Fx << " N" << std::endl;
  }

  std::cout << "\nDebug data written to tire_debug_zero_region.csv"
            << std::endl;

  return 0;
}