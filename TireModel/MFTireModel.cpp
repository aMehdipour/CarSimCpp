#include "MFTireModel.h"
#include "MathTools.h"
#include <cassert>
#include <cmath>
#include <iostream>

bool MFTireModel::configure(const json config) {
    std::string tirFile = config["tirPath"];
    std::unordered_map<std::string, double> coefficients;
    bool success = TirParser::parseTirFile(tirFile, coefficients);

    coeffs_.initializeFromTir(coefficients);

    return success;
}

void MFTireModel::computeTireForces(double &Fx, double &Fy, double u, double v,
                                    double camberAngle, double slipRatio,
                                    double slipAngle, double normalLoad) const {

    double epsilon = std::numeric_limits<double>::epsilon();

    // NOTE: PHY3 is not in the .tir, so I'm using a placeholder value of 0.0
    // TODO: Find Vc, Vcx, Vcy, Vs, and zetas
    double zeta0     = 1.;                          // TODO: Placeholder
    double zeta1     = 1.;                          // TODO: Placeholder
    double zeta2     = 1.;                          // TODO: Placeholder
    double zeta3     = 1.;                          // TODO: Placeholder
    double zeta4     = 1.;                          // TODO: Placeholder
    double Vs        = sqrt(pow(u, 2) + pow(v, 2)); // TODO: Placeholder
    double Vcx       = u;                           // TODO: Placeholder
    double Vcy       = v;                           // TODO: Placeholder
    double Fz0Prime  = coeffs_.FNOMIN_ * coeffs_.LFZO_;
    double dfz       = (normalLoad - Fz0Prime) / Fz0Prime;
    double alphaStar = tan(slipAngle) * MathTools::sign(Vcx);
    double gammaStar = sin(camberAngle);
    double lmuyStar  = coeffs_.LMUY_ * (1. + Vs / coeffs_.LONGVL_);
    double lmuxStar  = coeffs_.LMUX_ * (1. + Vs / coeffs_.LONGVL_);

    // Longitudinal force
    double SVX = normalLoad * (coeffs_.PVX1_ + coeffs_.PVX2_ * dfz) * (abs(Vcx) / (epsilon + abs(Vcx))) * coeffs_.LVX_ * coeffs_.LMUX_ * zeta1;
    double SHX = (coeffs_.PHX1_ + coeffs_.PHX2_ * dfz) * coeffs_.LHX_;
    double MUX = (coeffs_.PDX1_ + coeffs_.PDX2_ * dfz) * coeffs_.LMUX_ / (1. + Vs / coeffs_.LONGVL_);
    double DX = MUX * normalLoad * zeta1;
    /*assert(DX > 0.);*/
    double CX = coeffs_.PCX1_ * coeffs_.LCX_;
    /*assert(CX > 0.);*/
    double KAPPAX = slipRatio + SHX;
    double EX = (coeffs_.PEX1_ + coeffs_.PEX2_ * dfz + coeffs_.PEX3_ * pow(dfz, 2.)) * (1. - coeffs_.PEX4_ * MathTools::sign(KAPPAX)) * coeffs_.LEX_;
    /*assert(EX <= 1.);*/
    double KXKAPPA = normalLoad * (coeffs_.PKX1_ + coeffs_.PKX2_ * dfz) * exp(coeffs_.PKX3_ * dfz) * coeffs_.LKX_;
    double BX = KXKAPPA / (CX * DX + epsilon);
    double FX0 = DX * sin(CX * atan(BX * KAPPAX - EX * (BX * KAPPAX - atan(BX * KAPPAX)))) + SVX;

    // Lateral force
    double SVY = normalLoad * ((coeffs_.PVY1_ + coeffs_.PVY2_ * dfz) * coeffs_.LVY_ + (coeffs_.PVY3_ + coeffs_.PVY4_ * dfz) * gammaStar * coeffs_.LKYC_) * coeffs_.LMUY_ * zeta2;
    double SHY = (coeffs_.PHY1_ + coeffs_.PHY2_ * dfz) * coeffs_.LHY_ + coeffs_.PHY3_ * gammaStar * zeta0 + zeta4 - 1.;
    double KYALPHA0 = coeffs_.PKY1_ * Fz0Prime * sin(coeffs_.PKY4_ * atan(normalLoad / (coeffs_.PKY2_ * Fz0Prime))) * zeta3 * coeffs_.LYKA_;
    /*double KYALPHA  = KYALPHA0 * (1. - coeffs_.PKY3_ * pow(gammaStar, 2.)) *
   * zeta3;*/
    double KYALPHA = KYALPHA0;
    /*double KYGAMMA0 = (coeffs_.PHY3_ * KYALPHA0 + normalLoad * (coeffs_.PVY3_ +
   * coeffs_.PVY4_ * dfz)) * coeffs_.LKYC_;*/
    double KYGAMMA0 = normalLoad * (coeffs_.PKY6_ + coeffs_.PKY7_ * dfz) * coeffs_.LKYC_;
    double MUY = (coeffs_.PDY1_ + coeffs_.PDY2_ * dfz) * (1. - coeffs_.PDY3_ * pow(gammaStar, 2.)) * coeffs_.LMUY_ / (1. + Vs / coeffs_.LONGVL_);
    double ALPHAY = alphaStar + SHY;
    double CY = coeffs_.PCY1_ * coeffs_.LCY_;
    /*assert(CY > 0.);*/
    double DY = MUY * normalLoad * zeta2;
    double EY = (coeffs_.PEY1_ + coeffs_.PEY2_ * dfz) * (1. - (coeffs_.PEY3_ + coeffs_.PEY4_ * gammaStar) * MathTools::sign(ALPHAY)) * coeffs_.LEY_;
    if (EY > 1.) {
        EY = 1.;
    }
    /*assert(EY <= 1.);*/
    double BY = KYALPHA / (CY * DY + epsilon);
    double FY0 = DY * sin(CY * atan(BY * ALPHAY - EY * (BY * ALPHAY - atan(BY * ALPHAY)))) + SVY;

    // Longitudinal force (combined slip)
    double SHXALPHA = coeffs_.RHX1_;
    double EXALPHA = coeffs_.REX1_ + coeffs_.REX2_ * dfz;
    /*assert(std::abs(EXALPHA) <= 1.);*/
    /*EXALPHA         = EXALPHA <= 1. ? EXALPHA : 1.;*/
    double CXALPHA = coeffs_.RCX1_;
    double BXALPHA = coeffs_.RBX1_ * cos(atan(coeffs_.RBX2_ * slipRatio)) * coeffs_.LXAL_;
    double ALPHAS = alphaStar + SHXALPHA;
    double GXALPHA0 = cos(CXALPHA * atan(BXALPHA * SHXALPHA - EXALPHA * (BXALPHA * SHXALPHA - atan(BXALPHA * SHXALPHA))));
    /*assert(GXALPHA0 > 0.);*/
    double GXALPHA = cos(CXALPHA * atan(BXALPHA * ALPHAS - EXALPHA * (BXALPHA * ALPHAS - atan(BXALPHA * ALPHAS)))) / GXALPHA0;
    if (GXALPHA <= 0.) {
        // std::cerr << "GXALPHA is negative: " << GXALPHA << std::endl;
    }
    /*GXALPHA           = GXALPHA > 0. ? GXALPHA : 0.;*/

    // Lateral force (combined slip)
    double DVYKAPPA = MUY * normalLoad * (coeffs_.RVY1_ + coeffs_.RVY2_ * dfz + coeffs_.RVY3_ * gammaStar) * cos(atan(coeffs_.RVY4_ * alphaStar)) * zeta2;
    double SVYKAPPA = DVYKAPPA * sin(coeffs_.RVY5_ * atan(coeffs_.RVY6_ * slipRatio));
    double SHYKAPPA = coeffs_.RHY1_ + coeffs_.RHY2_ * dfz;
    double EYKAPPA  = coeffs_.REY1_ + coeffs_.REY2_ * dfz;
    /*assert(EYKAPPA <= 1.);*/
    double CYKAPPA = coeffs_.RCY1_;
    double BYKAPPA = coeffs_.RBY1_ * cos(atan(coeffs_.RBY2_ * (alphaStar - coeffs_.RBY3_))) * coeffs_.LYKA_;
    /*assert(BYKAPPA > 0.);*/
    double KAPPAS = slipRatio * SHYKAPPA;
    double GYKAPPA0 = cos(CYKAPPA * atan(BYKAPPA * SHYKAPPA - EYKAPPA * (BYKAPPA * SHYKAPPA - atan(BYKAPPA * SHYKAPPA))));
    double GYKAPPA = cos(CYKAPPA * atan(BYKAPPA * KAPPAS - EYKAPPA * (BYKAPPA * KAPPAS - atan(BYKAPPA * KAPPAS)))) / GYKAPPA0;
    /*assert(GYKAPPA0 > 0.);*/

    // Finally... calculate the forces
    Fx = GXALPHA * FX0;
    /*if (MathTools::sign(slipRatio) != MathTools::sign(Fx)) {*/
    /*    std::cerr << "Fx sign mismatch: slipRatio = " << slipRatio*/
    /*              << ", Fx = " << Fx*/
    /*              << ", dfz = " << dfz*/
    /*              << std::endl;*/
    /*}*/
    Fy = GYKAPPA * FY0 + SVYKAPPA;

    /*assert(Fy != FY0);*/
}
