#pragma once
#include "Integrator.h"

class RK4Integrator : public Integrator{
public:
    virtual void integrate();
};
