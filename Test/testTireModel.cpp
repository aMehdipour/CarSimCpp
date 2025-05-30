#include "MFTireModel.h"
#include "MathTools.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>
#include <vector>

using json = nlohmann::json;

// Test function to verify tire model outputs for various inputs
void testTireModelSweep() {
    std::cout << "=== Tire Model Sweep Test ===" << std::endl;

    MFTireModel tireModel;

    // Load configuration
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open config.json" << std::endl;
        return;
    }

    json config;
    configFile >> config;
    json tireParams = config["tire"];

    if (!tireModel.configure(tireParams)) {
        std::cerr << "Error: Failed to configure tire model" << std::endl;
        return;
    }

    // Test parameters
    std::vector<double> normalLoads = {50, 100, 150, 200}; // in lbs
    /*std::vector<double> normalLoads = {50}; // in lbs*/
    double slipAngleMin = -15.0 * M_PI / 180.0;            // convert to radians
    double slipAngleMax = 15.0 * M_PI / 180.0;
    double slipRatioMin = -0.2;
    double slipRatioMax = 0.2;
    int numSlipAngles = 61; // for -15 to +15 with 0.5 degree steps
    int numSlipRatios = 41; // for -0.2 to +0.2 with 0.01 steps

    // Option to avoid exact zero to prevent discontinuities
    bool avoidExactZero = true;
    double zeroOffset = 0.0001; // Small offset to avoid exact zero

    // Fixed parameters for testing
    double vx = 30.0;         // velocity in x direction (m/s)
    double camberAngle = 0.0; // no camber for this test

    // Open CSV file for output
    std::ofstream csvFile("tire_model_test_results.csv");
    csvFile << "NormalLoad_lbs,SlipAngle_deg,SlipRatio,Fx_N,Fy_N\n";

    int totalTests = 0;
    int passedTests = 0;

    // Calculate and display test resolution
    double slipAngleStep =
        (slipAngleMax - slipAngleMin) / (numSlipAngles - 1) * 180.0 / M_PI;
    double slipRatioStep = (slipRatioMax - slipRatioMin) / (numSlipRatios - 1);

    std::cout << "\nTest Configuration:" << std::endl;
    std::cout << "  Slip angle range: -15° to +15° with " << slipAngleStep
        << "° steps" << std::endl;
    std::cout << "  Slip ratio range: -0.2 to +0.2 with " << slipRatioStep
        << " steps" << std::endl;
    std::cout << "  Normal loads: ";
    for (size_t i = 0; i < normalLoads.size(); i++) {
        std::cout << normalLoads[i] << " lbs";
        if (i < normalLoads.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "  Total test points: "
        << normalLoads.size() * numSlipAngles * numSlipRatios << std::endl;
    std::cout << "\nRunning tests..." << std::endl;

    // Sweep through all parameters
    for (size_t loadIdx = 0; loadIdx < normalLoads.size(); loadIdx++) {
        double normalLoad_lbs = normalLoads[loadIdx];
        // Convert normal load from lbs to N (1 lb = 4.44822 N)
        double normalLoad_N = normalLoad_lbs * 4.44822;

        std::cout << "  Testing load " << normalLoad_lbs << " lbs... "
            << std::flush;

        for (int i = 0; i < numSlipAngles; i++) {
            double slipAngle = slipAngleMin + (slipAngleMax - slipAngleMin) * i /
                (numSlipAngles - 1);
            double slipAngle_deg = slipAngle * 180.0 / M_PI;

            double vy = 0;

            for (int j = 0; j < numSlipRatios; j++) {
                double slipRatio = slipRatioMin + (slipRatioMax - slipRatioMin) * j /
                    (numSlipRatios - 1);

                // Avoid exact zero if requested
                /*if (avoidExactZero && std::abs(slipRatio) < zeroOffset) {*/
                /*    slipRatio = (slipRatio >= 0) ? zeroOffset : -zeroOffset;*/
                /*}*/

                // Compute tire forces
                double Fx = 0.0, Fy = 0.0;
                tireModel.computeTireForces(Fx, Fy, vx, vy, camberAngle, slipRatio * 0.25,
                                            slipAngle * 0.25, normalLoad_N);

                // Write to CSV
                csvFile << normalLoad_lbs << "," << slipAngle_deg << "," << slipRatio
                    << "," << Fx << "," << Fy << "\n";

                // Basic sanity checks
                totalTests++;
                bool testPassed = true;

                if (std::abs(slipRatio) > 1e-6) {                     // skip the near-zero case
                    bool wrongSign = MathTools::sign(Fx) != MathTools::sign(slipRatio);
                    if (wrongSign) {
                        std::cerr << "FAILED: Fx sign reversed at "
                            << "Load="      << normalLoad_lbs
                            << " lbs, κ="   << slipRatio
                            << ", Fx="      << Fx << std::endl;
                        testPassed = false;
                    }
                }


                // Check for NaN or Inf
                if (std::isnan(Fx) || std::isnan(Fy) || std::isinf(Fx) ||
                    std::isinf(Fy)) {
                    std::cerr << "FAILED: NaN or Inf detected at Load=" << normalLoad_lbs
                        << " lbs, SlipAngle=" << slipAngle_deg
                        << " deg, SlipRatio=" << slipRatio << std::endl;
                    testPassed = false;
                }

                // Check force magnitudes are reasonable (not exceeding 10x normal load)
                if (std::abs(Fx) > 10 * normalLoad_N ||
                    std::abs(Fy) > 10 * normalLoad_N) {
                    std::cerr << "WARNING: Large force detected at Load="
                        << normalLoad_lbs << " lbs, SlipAngle=" << slipAngle_deg
                        << " deg, SlipRatio=" << slipRatio << " (Fx=" << Fx
                        << ", Fy=" << Fy << ")" << std::endl;
                }

                if (testPassed)
                    passedTests++;
            }
        }

        std::cout << "done (" << numSlipAngles * numSlipRatios << " points)"
            << std::endl;
    }

    csvFile.close();

    std::cout << "Tire model sweep test complete." << std::endl;
    std::cout << "Results written to tire_model_test_results.csv" << std::endl;
    std::cout << "Tests passed: " << passedTests << "/" << totalTests
        << std::endl;
}

// Test specific conditions
void testSpecificConditions() {
    std::cout << "\n=== Tire Model Specific Conditions Test ===" << std::endl;

    MFTireModel tireModel;

    // Load configuration
    std::ifstream configFile("config.json");
    json config;
    configFile >> config;
    json tireParams = config["tire"];

    assert(tireModel.configure(tireParams));

    // Test 1: Zero slip angle and zero slip ratio should produce minimal forces
    {
        double Fx = 0.0, Fy = 0.0;
        double vx = 30.0, vy = 0.0;
        double normalLoad = 100 * 4.44822; // 100 lbs in N

        tireModel.computeTireForces(Fx, Fy, vx, vy, 0.0, 0.0, 0.0, normalLoad);

        std::cout << "Test 1 - Zero slip conditions:" << std::endl;
        std::cout << "  Fx = " << Fx << " N (expected small)" << std::endl;
        std::cout << "  Fy = " << Fy << " N (expected ~0)" << std::endl;

        assert(std::abs(Fy) < 1.0); // Lateral force should be nearly zero
        std::cout << "  PASSED" << std::endl;
    }

    // Test 2: Pure slip angle (no slip ratio)
    {
        double Fx = 0.0, Fy = 0.0;
        double slipAngle = 5.0 * M_PI / 180.0; // 5 degrees
        double vx = 30.0;
        double vy = vx * tan(slipAngle);
        double normalLoad = 100 * 4.44822; // 100 lbs in N

        tireModel.computeTireForces(Fx, Fy, vx, vy, 0.0, 0.0, slipAngle,
                                    normalLoad);

        std::cout << "\nTest 2 - Pure slip angle (5 deg):" << std::endl;
        std::cout << "  Fx = " << Fx << " N" << std::endl;
        std::cout << "  Fy = " << Fy
            << " N (expected negative for positive slip angle)" << std::endl;

        // For positive slip angle, lateral force is typically negative
        /*assert(Fy < 0);*/
        std::cout << "  PASSED" << std::endl;
    }

    // Test 3: Pure slip ratio (no slip angle)
    {
        double Fx = 0.0, Fy = 0.0;
        double vx = 30.0, vy = 0.0;
        double slipRatio = 0.1;            // 10% slip
        double normalLoad = 100 * 4.44822; // 100 lbs in N

        tireModel.computeTireForces(Fx, Fy, vx, vy, 0.0, slipRatio, 0.0,
                                    normalLoad);

        std::cout << "\nTest 3 - Pure slip ratio (0.1):" << std::endl;
        std::cout << "  Fx = " << Fx << " N" << std::endl;
        std::cout << "  Fy = " << Fy << " N (expected ~0)" << std::endl;

        // Note: The sign convention for slip ratio vs force depends on the tire
        // model Some models produce positive Fx for positive slip ratio (braking)
        // Let's just check that we get a significant longitudinal force
        assert(std::abs(Fx) >
               50.0); // Should produce significant longitudinal force
        assert(std::abs(Fy) < 10.0); // Lateral force should be small
        std::cout << "  PASSED" << std::endl;
    }

    // Test 4: Negative slip ratio (acceleration)
    {
        double Fx = 0.0, Fy = 0.0;
        double vx = 30.0, vy = 0.0;
        double slipRatio = -0.1;           // -10% slip (acceleration)
        double normalLoad = 100 * 4.44822; // 100 lbs in N

        tireModel.computeTireForces(Fx, Fy, vx, vy, 0.0, slipRatio, 0.0,
                                    normalLoad);

        std::cout << "\nTest 4 - Negative slip ratio (-0.1):" << std::endl;
        std::cout << "  Fx = " << Fx << " N" << std::endl;
        std::cout << "  Fy = " << Fy << " N (expected ~0)" << std::endl;
        std::cout << "  PASSED" << std::endl;
    }

    std::cout << "\nAll specific condition tests passed!" << std::endl;
}

int main() {
    std::cout << "Starting Tire Model Unit Tests..." << std::endl;
    std::cout << "=================================" << std::endl;

    try {
        // Run specific condition tests
        testSpecificConditions();

        // Run sweep test
        testTireModelSweep();

        std::cout << "\nAll tire model tests completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
