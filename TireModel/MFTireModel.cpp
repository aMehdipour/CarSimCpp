#include "MFTireModel.h"

bool MFTireModel::configure(const json config) {
    std::string tirFile = config["tirPath"];
    bool success = TirParser::parseTirFile(tirFile, coefficients_);

    mfCoefficients_.initializeFromTir(coefficients_);

    return success;
}

double MFTireModel::computeLateralForce(double slipAngle, double normalLoad) const {
    return 0.0;
}


