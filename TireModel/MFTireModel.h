#pragma once
#include "MFCoefficients.h"
#include "TireModel.h"
#include "TirParser.h"
#include <json.hpp>

using json = nlohmann::json;

class MFTireModel : public TireModel {
    
public:
    bool configure(const json config);
    
    double computeLateralForce(double slipAngle, double normalLoad) const override;

private:
    std::unordered_map<std::string, double> coefficients_;
    MFCoefficients mfCoefficients_;
};
