#pragma once
#include <cstring>
#include <unordered_map>
#include <string>

struct MFCoefficients {

public:
    // Default constructor with initialization
    MFCoefficients() {
        // Initialize all parameters to zero
        std::memset(this, 0, sizeof(MFCoefficients));
    }

    void initializeFromTir(std::unordered_map<std::string, double> coefficients);

    double UNLOADED_RADIUS_;
    double WIDTH_;
    double ASPECT_RATIO_;
    double RIM_RADIUS_;
    double RIM_WIDTH_;
    double MASS_;
    double IXX_;
    double IYY_;
    double BELT_MASS_;
    double BELT_IXX_;
    double BELT_IYY_;
    double FNOMIN_;
    double LFZO_;
    double LCX_;
    double LMUX_;
    double LEX_;
    double LKX_;
    double LHX_;
    double LVX_;
    double LCY_;
    double LMUY_;
    double LEY_;
    double LKY_;
    double LHY_;
    double LRES_;
    double LXAL_;
    double LYKA_;
    double LKYC_;
    double LKZC_;
    double LMY_;
    double PCX1_;
    double PDX1_;
    double PDX2_;
    double PEX1_;
    double PEX2_;
    double PEX3_;
    double PEX4_;
    double PKX1_;
    double PKX2_;
    double PKX3_;
    double PHX1_;
    double PHX2_;
    double PVX1_;
    double PVX2_;
    double PCY1_;
    double PDY1_;
    double PDY2_;
    double PDY3_;
    double PDYP1_;
    double PDYP2_;
    double PDYP3_;
    double PDYP4_;
    double PEY1_;
    double PEY2_;
    double PEY3_;
    double PEY4_;
    double PHY1_;
    double PHY2_;
    double PKY1_;
    double PKY2_;
    double PKY3_;
    double PKYP1_;
    double PPZ1_;
    double PVY1_;
    double PVY2_;
    double PVY3_;
    double PVY4_;
    double QBZ1_;
    double QBZ10_;
    double QBZ2_;
    double QBZ3_;
    double QBZ4_;
    double QBZ5_;
    double QCZ1_;
    double QDZ1_;
    double QDZ10_;
    double QDZ11_;
    double QDZ2_;
    double QDZ3_;
    double QDZ4_;
    double QDZ6_;
    double QDZ7_;
    double QDZ8_;
    double QDZ9_;
    double QEZ1_;
    double QEZ2_;
    double QEZ3_;
    double QEZ4_;
    double QEZ5_;
    double QHZ1_;
    double QHZ2_;
    double QHZ3_;
    double QHZ4_;
    double QSX1_;
    double QSX10_;
    double QSX11_;
    double QSX12_;
    double QSX13_;
    double QSX14_;
    double QSX2_;
    double QSX3_;
    double QSY1_;
    double QSY2_;
    double RBX1_;
    double RBX2_;
    double RBY1_;
    double RBY2_;
    double RBY3_;
    double RCX1_;
    double RCY1_;
    double REX1_;
    double REX2_;
    double REY1_;
    double REY2_;
    double RHX1_;
    double RHY1_;
    double RHY2_;
    double RVY1_;
    double RVY2_;
    double RVY3_;
    double RVY4_;
    double RVY5_;
    double RVY6_;
    double SSZ1_;
    double SSZ2_;
    double SSZ3_;
    double SSZ4_;


};

