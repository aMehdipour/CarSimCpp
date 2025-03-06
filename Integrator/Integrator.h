#pragma once

class Integrator {
public:
    virtual void integrate() = 0;
    virtual ~Integrator() = default;
};
