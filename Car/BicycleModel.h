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
        json inputParams = config["input"];
        // Load parameters from the configuration
        mass_            = carParams["mass"];
        mass_           *= GlobalConstants::LBS_TO_KG;
        wheelbase_       = carParams["wheelbase"];
        wheelbase_      *= GlobalConstants::INCHES_TO_METERS;
        frontWeightBias_ = carParams["frontWeightBias"];
        // izz_             = carParams["Izz"];
        // izz_            *= GlobalConstants::SLUGS_FT_S2_TO_KG_M_S2;
        izz_             = 200.0;

        // Calculate derived parameters
        distFrontAxleToCg_ = wheelbase_ * (1.0 - frontWeightBias_);
        distRearAxleToCg_  = wheelbase_ * frontWeightBias_;
        rearWeightBias_    = 1.0 - frontWeightBias_;

        // Configure the tire Model
        // std::cout << "Bicycle Model configured\n"
        //           << "---- Mass:              " << mass_ << "\n"
        //           << "---- Wheelbase:         " << wheelbase_ << "\n"
        //           << "---- Front weight bias: " << frontWeightBias_ << "\n"
        //           << "---- Rear weight bias:  "  << rearWeightBias_ << "\n"
        //           << "---- Izz:               " << izz_ << "\n"
        //           << "---- DistFrontAxleToCg: " << distFrontAxleToCg_ << "\n"
        //           << "---- DistRearAxleToCg:  " << distRearAxleToCg_ << "\n"
        //           << std::endl;
        // assert(false);

        if (!tireModel_.configure(tireParams)) {
            throw std::runtime_error("Tire model configuration failed.");
        }

        if (inputParams["type"] == "Sine") {
            flagUseSineSteer_ = true;
        } else if (inputParams["type"] == "Ramp") {
            flagRampSteer_ = true;
        }
        // std::cout << "Tire Model configured." << std::endl;
        std::cout << "Time,x,y,psi,beta,r,vxBody,"
          << "xDot,yDot,psiDot,betaDot,rDot,vxBodyDot,"
          << "deltaDeg,slipAngleFrontDeg,slipAngleRearDeg,"
          << "Fyf,Fyr,Fxf,Fxr,Fzf,Fzr"
          << std::endl;

    }

    void operator()(double t,
                    MathTools::StateVector &state,
                    MathTools::DerivativeVector &derivatives) {

        assert(state.size() == CarConstants::NUM_STATES);

        // Unpack the state...
        double x      = state(0);
        double y      = state(1);
        double psi    = state(2);
        double beta   = state(3);
        double r      = state(4);
        double vxBody = state(5);
        double vyBody = vxBody * tan(beta); // tan(beta) here because beta = atan(vyBody / vxBody)
        double delta  = getSteeringInput(t);

        // Compute slip angles for front and rear.
        assert(vxBody > 0.0);
        double slipAngleFront = -delta + ((distFrontAxleToCg_ * r) / vxBody) + beta;
        double slipAngleRear  = -(distRearAxleToCg_ * r) / vxBody + beta;

        // Compute normal forces on front and rear tires.
        double Fzf = mass_ * GlobalConstants::GRAVITY * frontWeightBias_;
        double Fzr = mass_ * GlobalConstants::GRAVITY * rearWeightBias_;

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
        // MathTools::Vec2 wheelCenterVelF(vxBody, vyBody);
        // MathTools::Vec2 wheelCenterVelR(vxBody, vyBody);
        wheelCenterVelF    = rotTireFromBody * wheelCenterVelF;
        // slipAngleFront     = -atan2(wheelCenterVelF(1), wheelCenterVelF(0));
        // slipAngleRear      = -atan2(wheelCenterVelR(1), wheelCenterVelR(0));
        // slipAngleFront *= 0.25;
        // slipAngleRear  *= 0.25;

        // Extract tire forces
        double Fyf;
        double Fyr;
        double Fxf = 0.0; // Placeholder for longitudinal force on front tireModel
        double Fxr = 0.0; // Placeholder for longitudinal force on rear tireModel

        tireModel_.computeTireForces(Fxf, Fyf, wheelCenterVelF(0),
                                     wheelCenterVelF(1), 0.0, 0.0, slipAngleFront,
                                     Fzf);
        tireModel_.computeTireForces(Fxr, Fyr, wheelCenterVelR(0),
                                     wheelCenterVelR(1), 0.0, 0.0, slipAngleRear,
                                     Fzr);

        // Force longitudinal to zero for now.
        Fxf = 0.0;
        Fxr = 0.0;

        Fyf *= 2.; // Two tires lumped together
        Fyr *= 2.; // Two tires lumped together

        // Kinematic equations (transforming body-fixed velocities into inertial
        // frame):
        double xDot = vxBody * cos(psi) - vyBody * sin(psi);
        double yDot = vxBody * sin(psi) + vyBody * cos(psi);
        double psiDot = r; // The derivative of heading is the yaw rate.

        // Dynamics for the lateral (sideslip) motion.
        // The lateral force balance (in the body-fixed frame) gives:
        //   m * u * (betaDot + r) = F_yf + F_yr
        // Solving for betaDot gives:
        double betaDot = (Fyf + Fyr) / (mass_ * std::sqrt(vxBody * vxBody)) - r;

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

        // std::cout << "Time=" << t
        //           << " | x=" << x << ", y=" << y << ", psi=" << psi
        //           << ", beta=" << beta << ", r=" << r << ", vxBody=" << vxBody
        //           << " | xDot=" << xDot << ", yDot=" << yDot << ", psiDot=" << psiDot
        //           << ", betaDot=" << betaDot << ", rDot=" << rDot << ", vxBodyDot=" << vxBodyDot
        //           << " | deltaDeg=" << (delta * GlobalConstants::RAD_TO_DEG)
        //           << " | slipAngleFrontDeg=" << (slipAngleFront * GlobalConstants::RAD_TO_DEG)
        //           << ", slipAngleRearDeg=" << (slipAngleRear * GlobalConstants::RAD_TO_DEG)
        //           << " | Fyf=" << Fyf << ", Fyr=" << Fyr
        //           << ", Fxf=" << Fxf << ", Fxr=" << Fxr
        //           << " | Fzf=" << Fzf << ", Fzr=" << Fzr
        //           << std::endl;
        std::cout << t << "," << x << "," << y << "," << (psi * GlobalConstants::RAD_TO_DEG) << "," << (beta * GlobalConstants::RAD_TO_DEG) << "," << r << "," << vxBody << ","
          << xDot << "," << yDot << "," << (psiDot * GlobalConstants::RAD_TO_DEG) << "," << (betaDot * GlobalConstants::RAD_TO_DEG) << "," << (rDot * GlobalConstants::RAD_TO_DEG) << "," << vxBodyDot << ","
          << (delta * GlobalConstants::RAD_TO_DEG) << ","
          << (slipAngleFront * GlobalConstants::RAD_TO_DEG) << "," << (slipAngleRear * GlobalConstants::RAD_TO_DEG) << ","
          << Fyf << "," << Fyr << "," << Fxf << "," << Fxr << "," << Fzf << "," << Fzr
          << std::endl;

}

    double getSteeringInput(double t) {
        if (flagUseSineSteer_) {
            return GlobalConstants::MAX_STEERING_ANGLE_RAD * sin(0.5 * t);
        } else if (flagRampSteer_) {
            if (t < 1.0) {
                return GlobalConstants::MAX_STEERING_ANGLE_RAD * t;
            } else {
                return GlobalConstants::MAX_STEERING_ANGLE_RAD;
            }
        } else {
            return 0.0; // No steering input
        }
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

    bool flagUseSineSteer_ = false;
    bool flagRampSteer_ = false; // Use ramp steering input

    TireModel tireModel_;
};
