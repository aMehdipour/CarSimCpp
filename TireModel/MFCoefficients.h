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

    double UNLOADED_RADIUS_ = 0.;
    double WIDTH_           = 0.;
    double ASPECT_RATIO_    = 0.;
    double RIM_RADIUS_      = 0.;
    double RIM_WIDTH_       = 0.;
    double MASS_            = 0.;
    double IXX_             = 0.;
    double IYY_             = 0.;
    double BELT_MASS_       = 0.;
    double BELT_IXX_        = 0.;
    double BELT_IYY_        = 0.;
    double FNOMIN_          = 0.;
    double LONGVL_          = 0.;
    double LFZO_            = 0.;
    double LCX_             = 0.;
    double LMUX_            = 0.;
    double LEX_             = 0.;
    double LKX_             = 0.;
    double LHX_             = 0.;
    double LVX_             = 0.;
    double LCY_             = 0.;
    double LMUY_            = 0.;
    double LEY_             = 0.;
    double LKY_             = 0.;
    double LHY_             = 0.;
    double LRES_            = 0.;
    double LXAL_            = 0.;
    double LYKA_            = 0.;
    double LKYC_            = 0.;
    double LKZC_            = 0.;
    double LMY_             = 0.;
    double LVY_             = 0.;
    double PCX1_            = 0.;
    double PDX1_            = 0.;
    double PDX2_            = 0.;
    double PDX3_            = 0.;
    double PEX1_            = 0.;
    double PEX2_            = 0.;
    double PEX3_            = 0.;
    double PEX4_            = 0.;
    double PKX1_            = 0.;
    double PKX2_            = 0.;
    double PKX3_            = 0.;
    double PHX1_            = 0.;
    double PHX2_            = 0.;
    double PVX1_            = 0.;
    double PVX2_            = 0.;
    double PPX1_            = 0.;
    double PPX2_            = 0.;
    double PPX3_            = 0.;
    double PPX4_            = 0.;
    double PCY1_            = 0.;
    double PDY1_            = 0.;
    double PDY2_            = 0.;
    double PDY3_            = 0.;
    double PDYP1_           = 0.;
    double PDYP2_           = 0.;
    double PDYP3_           = 0.;
    double PDYP4_           = 0.;
    double PEY1_            = 0.;
    double PEY2_            = 0.;
    double PEY3_            = 0.;
    double PEY4_            = 0.;
    double PEY5_            = 0.;
    double PHY1_            = 0.;
    double PHY2_            = 0.;
    double PHY3_            = 0.;
    double PKY1_            = 0.;
    double PKY2_            = 0.;
    double PKY3_            = 0.;
    double PKY4_            = 0.;
    double PKY5_            = 0.;
    double PKY6_            = 0.;
    double PKY7_            = 0.;
    double PPY1_            = 0.;
    double PPY2_            = 0.;
    double PPY3_            = 0.;
    double PPY4_            = 0.;
    double PPY5_            = 0.;
    double PKYP1_           = 0.;
    double PPZ1_            = 0.;
    double PVY1_            = 0.;
    double PVY2_            = 0.;
    double PVY3_            = 0.;
    double PVY4_            = 0.;
    double QBZ1_            = 0.;
    double QBZ10_           = 0.;
    double QBZ2_            = 0.;
    double QBZ3_            = 0.;
    double QBZ4_            = 0.;
    double QBZ5_            = 0.;
    double QCZ1_            = 0.;
    double QDZ1_            = 0.;
    double QDZ10_           = 0.;
    double QDZ11_           = 0.;
    double QDZ2_            = 0.;
    double QDZ3_            = 0.;
    double QDZ4_            = 0.;
    double QDZ6_            = 0.;
    double QDZ7_            = 0.;
    double QDZ8_            = 0.;
    double QDZ9_            = 0.;
    double QEZ1_            = 0.;
    double QEZ2_            = 0.;
    double QEZ3_            = 0.;
    double QEZ4_            = 0.;
    double QEZ5_            = 0.;
    double QHZ1_            = 0.;
    double QHZ2_            = 0.;
    double QHZ3_            = 0.;
    double QHZ4_            = 0.;
    double QSX1_            = 0.;
    double QSX10_           = 0.;
    double QSX11_           = 0.;
    double QSX12_           = 0.;
    double QSX13_           = 0.;
    double QSX14_           = 0.;
    double QSX2_            = 0.;
    double QSX3_            = 0.;
    double QSY1_            = 0.;
    double QSY2_            = 0.;
    double RBX1_            = 0.;
    double RBX2_            = 0.;
    double RBY1_            = 0.;
    double RBY2_            = 0.;
    double RBY3_            = 0.;
    double RCX1_            = 0.;
    double RCY1_            = 0.;
    double REX1_            = 0.;
    double REX2_            = 0.;
    double REY1_            = 0.;
    double REY2_            = 0.;
    double RHX1_            = 0.;
    double RHY1_            = 0.;
    double RHY2_            = 0.;
    double RVY1_            = 0.;
    double RVY2_            = 0.;
    double RVY3_            = 0.;
    double RVY4_            = 0.;
    double RVY5_            = 0.;
    double RVY6_            = 0.;
    double SSZ1_            = 0.;
    double SSZ2_            = 0.;
    double SSZ3_            = 0.;
    double SSZ4_            = 0.;


};

