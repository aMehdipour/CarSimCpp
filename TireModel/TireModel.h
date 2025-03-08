#pragma once

class TireModel {
public:
    virtual ~TireModel() {}
    virtual double computeLateralForce(double slipAngle, double normalLoad) const = 0;

};
