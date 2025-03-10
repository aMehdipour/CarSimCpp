#pragma once

class TireModel {
public:
    virtual ~TireModel() {}
    virtual void computeLateralForce(double& Fx, double& Fy, double vx, double vy, double camberAngle, double slipRatio, double slipAngle, double normalLoad) const = 0;

};
