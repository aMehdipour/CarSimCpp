#pragma once
#include "IntegratorBase.h"

class EulerIntegrator : public IntegratorBase{
public:
    virtual void integrate();
};
