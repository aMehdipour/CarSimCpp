#pragma once
#include "CarConstants.h"
#include "GlobalConstants.h"
#include "MFTireModel.h"
#include <MathTools.h>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;
template <class TireModel> class BicycleModel {
public:
    void simulate() { std::cout << "Simulating Bicycle Model\n"; }

    void configure(const json &config) {
        json carParams = config["car"];
        json tireParams = config["tire"];
        // Load parameters from the configuration
        mass_            = carParams["mass"];
        mass_           *= GlobalConstants::LBS_TO_KG;
        wheelbase_       = carParams["wheelbase"];
        wheelbase_      *= GlobalConstants::INCHES_TO_METERS;
        frontWeightBias_ = carParams["frontWeightBias"];
        izz_             = carParams["Izz"];
        izz_            *= GlobalConstants::SLUGS_FT_S2_TO_KG_M_S2;

        // Calculate derived parameters
        distFrontAxleToCg_ = wheelbase_ * frontWeightBias_;
        distRearAxleToCg_  = wheelbase_ * (1.0 - frontWeightBias_);
        rearWeightBias_    = 1.0 - frontWeightBias_;

        // Configure the tire Model
        std::cout << "Bicycle Model configured\n"
                  << "---- Mass:              " << mass_ << "\n"
                  << "---- Wheelbase:         " << wheelbase_ << "\n"
                  << "---- Front weight bias: " << frontWeightBias_ << "\n"
                  << "---- Rear weight bias: "  << rearWeightBias_ << "\n"
                  << "---- Izz:               " << izz_ << "\n"
                  << "---- DistFrontAxleToCg: " << distFrontAxleToCg_ << "\n"
                  << "---- DistRearAxleToCg:  " << distRearAxleToCg_ << "\n"
                  << std::endl;

        if (!tireModel_.configure(tireParams)) {
            throw std::runtime_error("Tire model configuration failed.");
        }
        std::cout << "Tire Model configured." << std::endl;
    }

    void operator()(double t,
                    MathTools::StateVector &state,
                    MathTools::DerivativeVector &derivatives) {

        assert(state.size() == CarConstants::NUM_STATES);

        // Unpack the state...
        double x = state(0);
        double y = state(1);
        double psi = state(2);
        double beta = state(3);
        double r = state(4);
        double vxBody = state(5);
        double vyBody = vxBody * tan(beta); // tan(beta) here because beta = atan(vyBody / vxBody)
        double delta = getSteeringInput(t);

        // Compute slip angles for front and rear.
        assert(vxBody > 0.0);
        double slipAngleFront = delta - (beta + (distFrontAxleToCg_ * r) / vxBody);
        double slipAngleRear = beta - (distRearAxleToCg_ * r) / vxBody;

        // Compute normal forces on front and rear tires.
        double Fzf =
            mass_ * GlobalConstants::GRAVITY * distRearAxleToCg_ / wheelbase_;
        double Fzr =
            mass_ * GlobalConstants::GRAVITY * distFrontAxleToCg_ / wheelbase_;

        // Construct the rotation matrix from body-fixed to tire frame.
        MathTools::Mat2 rotTireFromBody;
        rotTireFromBody(0, 0) = cos(delta);
        rotTireFromBody(0, 1) = sin(delta);
        rotTireFromBody(1, 0) = -sin(delta);
        rotTireFromBody(1, 1) = cos(delta);

        // We must add the contribution of the yaw rate to the body-fixed
        // velocities.
        MathTools::Vec2 wheelCenterVelF(vxBody, vyBody + r * distFrontAxleToCg_);
        MathTools::Vec2 wheelCenterVelR(vxBody, vyBody - r * distRearAxleToCg_);
        wheelCenterVelF = rotTireFromBody * wheelCenterVelF;

        // Extract tire forces
        double Fyf;
        double Fyr;
        double Fxf = 0.0; // Placeholder for longitudinal force on front tireModel
        double Fxr = 0.0; // Placeholder for longitudinal force on rear tireModel

        tireModel_.computeTireForces(Fxf, Fyf, wheelCenterVelF(0),
                                     wheelCenterVelF(1), 0.0, 0.0, slipAngleFront,
                                     -Fzf);
        tireModel_.computeTireForces(Fxr, Fyr, wheelCenterVelR(0),
                                     wheelCenterVelR(1), 0.0, 0.0, slipAngleRear,
                                     -Fzr);

        // Force longitudinal to zero for now.
        Fxf = 0.0;
        Fxr = 0.0;

        // Kinematic equations (transforming body-fixed velocities into inertial
        // frame):
        double xDot = vxBody * cos(psi) - vyBody * sin(psi);
        double yDot = vxBody * sin(psi) + vyBody * cos(psi);
        double psiDot = r; // The derivative of heading is the yaw rate.

        // Dynamics for the lateral (sideslip) motion.
        // The lateral force balance (in the body-fixed frame) gives:
        //   m * u * (betaDot + r) = F_yf + F_yr
        // Solving for betaDot gives:
        double betaDot = (Fyf + Fyr) / (mass_ * vxBody) - r;

        // Yaw moment balance gives:
        //   I_z * r_dot = a * F_yf - b * F_yr
        double rDot = (distFrontAxleToCg_ * Fyf - distRearAxleToCg_ * Fyr) / izz_;

        // Longitudinal dynamics (for now, placeholder: no acceleration).
        double vxBodyDot = 0.0;

        // Fill in the derivative vector.
        derivatives(0) = xDot;
        derivatives(1) = yDot;
        derivatives(2) = psiDot;
        derivatives(3) = betaDot;
        derivatives(4) = rDot;
        derivatives(5) = vxBodyDot;
    }

    double getSteeringInput(double t) {
        return GlobalConstants::MAX_STEERING_ANGLE_RAD * sin(0.5 * t);
    }

    // Getters
    double getMass() { return mass_; }
    double getWheelbase() { return wheelbase_; }
    double getYawInertia() { return izz_; }
    double getFrontWeightBias() { return frontWeightBias_; }
    double getRearWeightBias() { return rearWeightBias_; }
    double getDistFrontAxleToCg() { return distFrontAxleToCg_; }
    double getDistRearAxleToCg() { return distRearAxleToCg_; }

private:
    double mass_;
    double wheelbase_;
    double izz_;
    double frontWeightBias_;
    double rearWeightBias_;
    double distFrontAxleToCg_;
    double distRearAxleToCg_;

    TireModel tireModel_;
};
