#pragma once
#include "Integrator.h"

class EulerIntegrator : public Integrator{
public:
    virtual void integrate();
};
