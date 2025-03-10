#pragma once
#include "TireModel.h"

class LinearTireModel : TireModel {
public:
    LinearTireModel(double corneringStiffnessFront, double corneringStiffnessRear) :
    corneringStiffnessFront_(corneringStiffnessFront),
    corneringStiffnessRear_(corneringStiffnessRear) { }

    double computeLateralForce(double slipRatio, double slipAngle, double normalLoad);

private:
    double corneringStiffnessFront_;
    double corneringStiffnessRear_;
};
