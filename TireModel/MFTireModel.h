#pragma once
#include "MFCoefficients.h"
#include "TireModel.h"
#include "TirParser.h"
#include <json.hpp>

using json = nlohmann::json;

class MFTireModel : public TireModel {
    
public:
    bool configure(const json config);
    
    void computeTireForces(double& Fx, double& Fy, double vx, double vy, double camberAngle, double slipRatio, double slipAngle, double normalLoad) const override;

private:
    MFCoefficients coeffs_;
};
