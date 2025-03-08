#include "BicycleModel.h"
#include <cmath>
#include <iostream>
#include "CarConstants.h"
#include "GlobalConstants.h"

void BicycleModel::simulate() {
    std::cout << "Simulating Bicycle Model\n";
}

template <typename TireModel>
void BicycleModel::computeDerivatives(MathTools::StateVector& state,
                                      MathTools::DerivativeVector& derivatives,
                                      TireModel& tireModel,
                                      double t) {
    assert(state.size() == CarConstants::NUM_STATES_BICYCLE);

    // Unpack the state...
    double x     = state(0);
    double y     = state(1);
    double psi   = state(2);
    double beta  = state(3);
    double r     = state(4);
    double vx    = state(5);
    double vy    = vx * sin(beta);
    double delta = getSteeringInput(t);

    // Compute slip angles for front and rear.
    double slipAngleFront = delta - (beta + (distFrontAxleToCg_ * r) / vx);
    double slipAngleRear  = beta - (distRearAxleToCg_ * r) / vx;

    // Compute normal forces on front and rear tires.
    double Fzf = mass_ * GlobalConstants::GRAVITY * distRearAxleToCg_ / wheelbase_;
    double Fzr = mass_ * GlobalConstants::GRAVITY * distFrontAxleToCg_ / wheelbase_;
    
    // Extract tire forces
    double Fyf = tireModel.computeTireForces(slipAngleFront, Fzf);
    double Fyr = tireModel.computeTireForces(slipAngleRear, Fzr);

    // Sticking with lateral only for now
    double Fx = 0.0;

    // Kinematic equations (transforming body-fixed velocities into inertial frame):
    double x_dot   = vx * cos(psi) - vy * sin(psi);
    double y_dot   = vx * sin(psi) + vy * cos(psi);
    double psi_dot = r;  // The derivative of heading is the yaw rate.
    
    // Dynamics for the lateral (sideslip) motion.
    // The lateral force balance (in the body-fixed frame) gives:
    //   m * u * (dot(beta) + r) = F_yf + F_yr
    // Solving for dot(beta):
    double beta_dot = (Fyf + Fyr) / (mass_ * vx) - r;
    
    // Yaw moment balance gives:
    //   I_z * r_dot = a * F_yf - b * F_yr
    double r_dot = (distFrontAxleToCg_ * Fyf - distRearAxleToCg_ * Fyr) / izz_;
    
    // Longitudinal dynamics (for now, placeholder: no acceleration).
    double vx_dot = 0.0;
    
    // Fill in the derivative vector.
    derivatives(0) = x_dot;
    derivatives(1) = y_dot;
    derivatives(2) = psi_dot;
    derivatives(3) = beta_dot;
    derivatives(4) = r_dot;
    derivatives(5) = vx_dot;
}

double BicycleModel::getSteeringInput(double t) {
    return GlobalConstants::DEG_TO_RAD * 15 * sin( 0.5 * t );
}
