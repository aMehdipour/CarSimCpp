#pragma once

class LinearTireModel {
public:
    LinearTireModel(double stiffnessFront, double stiffnessRear) :
    corneringStiffnessFront_(stiffnessFront),
    corneringStiffnessRear_(stiffnessRear) { }

    double computeLateralForce(double slipAngle, double normalLoad);

private:
    double corneringStiffnessFront_;
    double corneringStiffnessRear_;
};
