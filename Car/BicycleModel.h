#pragma once
#include <json.hpp>
#include <MathTools.h>

class BicycleModel{
public:
    template <typename TireModel>
    void computeDerivatives(MathTools::StateVector& state,
                            MathTools::DerivativeVector& derivatives,
                            TireModel& tireModel,
                            double t);
    double getSteeringInput(double t);
    void simulate();

    // Getters
    double getMass() {return mass_;}
    double getWheelbase() {return wheelbase_;}
    double getYawInertia() {return izz_;}
    double getFrontWeightBias() {return frontWeightBias_;}
    double getRearWeightBias() {return rearWeightBias_;}
    double getDistFrontAxleToCg() {return distFrontAxleToCg_;}
    double getDistRearAxleToCg() {return distRearAxleToCg_;}

private:
    double mass_;
    double wheelbase_;
    double izz_;
    double frontWeightBias_;
    double rearWeightBias_;
    double distFrontAxleToCg_;
    double distRearAxleToCg_;

};
