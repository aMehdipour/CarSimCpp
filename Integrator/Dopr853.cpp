#include "Dopr853Constants.h"
#include "Dopr853.h"

//------------------------------------------------------------------------------
// Constructor: Initialize state, allocate working arrays.
namespace Dopr853 {
template <class D>
Integrator<D>::Integrator(Vecdouble_IO &yy, Vecdouble_IO &dydxx, double &xx,
                                  const double atoll, const double rtoll, bool dens)
    : y(yy), dydx(dydxx), x(xx),
      yerr(yy.size()), yerr2(yy.size()),
      k2(yy.size()), k3(yy.size()), k4(yy.size()), k5(yy.size()),
      k6(yy.size()), k7(yy.size()), k8(yy.size()), k9(yy.size()), k10(yy.size()),
      rcont1(yy.size()), rcont2(yy.size()), rcont3(yy.size()), rcont4(yy.size()),
      rcont5(yy.size()), rcont6(yy.size()), rcont7(yy.size()), rcont8(yy.size())
{
    const double EPS = std::numeric_limits<double>::epsilon();
}

//------------------------------------------------------------------------------
// Step: Try a step with trial step size htry, adjust if necessary.
template <class D>
void Integrator<D>::step(const double htry, D &computeDerivatives) {
    Vecdouble dydxnew(y.size());
    double h = htry;
    for (;;) {
        dy(h, computeDerivatives); // Compute one complete step
        double err = error(h);
        if (con.success(err, h))
            break;
        if (fabs(h) <= fabs(x) * std::numeric_limits<double>::epsilon())
            throw std::runtime_error("stepsize underflow in Integrator");
    }
    computeDerivatives(x + h, y, dydxnew);
    if (true)  // if dense output is desired, always prepare it here
        prepare_dense(h, dydxnew, computeDerivatives);
    dydx = dydxnew;
    xold = x;
    x += h;
    con.hnext = h;
}

//------------------------------------------------------------------------------
// dy: Compute one step (all 12 stages) of the integration.
template <class D>
void Integrator<D>::dy(const double h, D &computeDerivatives) {
    Vecdouble ytemp(y.size());
    int i;
    // Stage 2
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * a21 * dydx[i];
    computeDerivatives(x + c2 * h, ytemp, k2);
    
    // Stage 3
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a31 * dydx[i] + a32 * k2[i]);
    computeDerivatives(x + c3 * h, ytemp, k3);
    
    // Stage 4
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a41 * dydx[i] + a43 * k3[i]);
    computeDerivatives(x + c4 * h, ytemp, k4);
    
    // Stage 5
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a51 * dydx[i] + a53 * k3[i] + a54 * k4[i]);
    computeDerivatives(x + c5 * h, ytemp, k5);
    
    // Stage 6
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a61 * dydx[i] + a64 * k4[i] + a65 * k5[i]);
    computeDerivatives(x + c6 * h, ytemp, k6);
    
    // Stage 7
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a71 * dydx[i] + a74 * k4[i] + a75 * k5[i] + a76 * k6[i]);
    computeDerivatives(x + c7 * h, ytemp, k7);
    
    // Stage 8
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a81 * dydx[i] + a84 * k4[i] + a85 * k5[i] + a86 * k6[i] + a87 * k7[i]);
    computeDerivatives(x + c8 * h, ytemp, k8);
    
    // Stage 9
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a91 * dydx[i] + a94 * k4[i] + a95 * k5[i] + a96 * k6[i] +
                                a97 * k7[i] + a98 * k8[i]);
    computeDerivatives(x + c9 * h, ytemp, k9);
    
    // Stage 10
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a101 * dydx[i] + a104 * k4[i] + a105 * k5[i] + a106 * k6[i] +
                                a107 * k7[i] + a108 * k8[i] + a109 * k9[i]);
    computeDerivatives(x + c10 * h, ytemp, k10);
    
    // Stage 11 (reuse k2)
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a111 * dydx[i] + a114 * k4[i] + a115 * k5[i] + a116 * k6[i] +
                                a117 * k7[i] + a118 * k8[i] + a119 * k9[i] + a1110 * k10[i]);
    computeDerivatives(x + c11 * h, ytemp, k2);
    
    double xph = x + h;
    // Stage 12 (reuse k3)
    for (i = 0; i < y.size(); i++)
        ytemp[i] = y[i] + h * (a121 * dydx[i] + a124 * k4[i] + a125 * k5[i] + a126 * k6[i] +
                                a127 * k7[i] + a128 * k8[i] + a129 * k9[i] + a1210 * k10[i] + a1211 * k2[i]);
    computeDerivatives(xph, ytemp, k3);
    
    // Final combination to produce the 8th order solution
    for (i = 0; i < y.size(); i++) {
        k4[i] = b1 * dydx[i] + b6 * k6[i] + b7 * k7[i] + b8 * k8[i] +
                b9 * k9[i] + b10 * k10[i] + b11 * k2[i] + b12 * k3[i];
        y[i] = y[i] + h * k4[i];
    }
    // Compute two error estimators.
    for (i = 0; i < y.size(); i++) {
        yerr[i] = k4[i] - bhh1 * dydx[i] - bhh2 * k9[i] - bhh3 * k3[i];
        yerr2[i] = er1 * dydx[i] + er6 * k6[i] + er7 * k7[i] + er8 * k8[i] +
                   er9 * k9[i] + er10 * k10[i] + er11 * k2[i] + er12 * k3[i];
    }
}

//------------------------------------------------------------------------------
// prepare_dense: Compute dense output coefficients for interpolation.
template <class D>
void Integrator<D>::prepare_dense(const double h, Vecdouble_I &dydxnew, D &derivs) {
    int i;
    double ydiff, bspl;
    Vecdouble ytemp(y.size());
    for (i = 0; i < y.size(); i++) {
        rcont1[i] = y[i];
        // Here ydiff should be (y_out - y); assuming y now holds y_out.
        ydiff = 0.0;  
        rcont2[i] = ydiff;
        bspl = h * dydx[i] - ydiff;
        rcont3[i] = bspl;
        rcont4[i] = ydiff - h * dydxnew[i] - bspl;
        rcont5[i] = d41 * dydx[i] + d46 * k6[i] + d47 * k7[i] + d48 * k8[i] +
                    d49 * k9[i] + d410 * k10[i] + d411 * k2[i] + d412 * k3[i];
        rcont6[i] = d51 * dydx[i] + d56 * k6[i] + d57 * k7[i] + d58 * k8[i] +
                    d59 * k9[i] + d510 * k10[i] + d511 * k2[i] + d512 * k3[i];
        rcont7[i] = d61 * dydx[i] + d66 * k6[i] + d67 * k7[i] + d68 * k8[i] +
                    d69 * k9[i] + d610 * k10[i] + d611 * k2[i] + d612 * k3[i];
        rcont8[i] = d71 * dydx[i] + d76 * k6[i] + d77 * k7[i] + d78 * k8[i] +
                    d79 * k9[i] + d710 * k10[i] + d711 * k2[i] + d712 * k3[i];
    }
    for (i = 0; i < y.size(); i++) {
        ytemp[i] = y[i] + h * (a141 * dydx[i] + a147 * k7[i] + a148 * k8[i] +
                                a149 * k9[i] + a1410 * k10[i] + a1411 * k2[i] +
                                a1412 * k3[i] + a1413 * dydxnew[i]);
    }
    derivs(x + c14 * h, ytemp, k10);
    for (i = 0; i < y.size(); i++) {
        ytemp[i] = y[i] + h * (a151 * dydx[i] + a156 * k6[i] + a157 * k7[i] +
                                a158 * k8[i] + a1511 * k2[i] + a1512 * k3[i] +
                                a1513 * dydxnew[i] + a1514 * k10[i]);
    }
    derivs(x + c15 * h, ytemp, k2);
    for (i = 0; i < y.size(); i++) {
        ytemp[i] = y[i] + h * (a161 * dydx[i] + a166 * k6[i] + a167 * k7[i] +
                                a168 * k8[i] + a169 * k9[i] + a1613 * dydxnew[i] +
                                a1614 * k10[i] + a1615 * k2[i]);
    }
    derivs(x + c16 * h, ytemp, k3);
    for (i = 0; i < y.size(); i++) {
        rcont5[i] = h * (rcont5[i] + d413 * dydxnew[i] + d414 * k10[i] +
                         d415 * k2[i] + d416 * k3[i]);
        rcont6[i] = h * (rcont6[i] + d513 * dydxnew[i] + d514 * k10[i] +
                         d515 * k2[i] + d516 * k3[i]);
        rcont7[i] = h * (rcont7[i] + d613 * dydxnew[i] + d614 * k10[i] +
                         d615 * k2[i] + d616 * k3[i]);
        rcont8[i] = h * (rcont8[i] + d713 * dydxnew[i] + d714 * k10[i] +
                         d715 * k2[i] + d716 * k3[i]);
    }
}

//------------------------------------------------------------------------------
// dense_out: Return interpolated solution at x (within the current step of size h) for component i.
template <class D>
double Integrator<D>::dense_out(const int i, const double x, const double h) {
    double s = (x - xold) / h;
    double s1 = 1.0 - s;
    return rcont1[i] + s * (rcont2[i] + s1 * (rcont3[i] + s * (rcont4[i] +
           s1 * (rcont5[i] + s * (rcont6[i] + s1 * (rcont7[i] + s * rcont8[i]))))));
}

//------------------------------------------------------------------------------
// error: Compute and return the error estimate for the current step.
template <class D>
double Integrator<D>::error(const double h) {
    double err = 0.0, err2 = 0.0, sk, deno;
    for (int i = 0; i < y.size(); i++) {
        sk = std::max(atol, fabs(y[i])) + rtol * std::max(fabs(y[i]), fabs(y[i]));
        err  += (yerr[i] / sk) * (yerr[i] / sk);
        err2 += (yerr2[i] / sk) * (yerr2[i] / sk);
    }
    deno = err + 0.01 * err2;
    if (deno <= 0.0)
        deno = 1.0;
    return fabs(h) * err * sqrt(1.0 / (y.size() * deno));
}

//------------------------------------------------------------------------------
// Controller: Constructor for adaptive step size controller.
template <class D>
Integrator<D>::Controller::Controller() : hnext(0), errold(1.0e-4), reject(false) {}

//------------------------------------------------------------------------------
// Controller::success: Determine if the step is acceptable and compute new step size.
template <class D>
bool Integrator<D>::Controller::success(const double err, double &h) {
    static const double beta = 0.0, alpha = 1.0/8.0 - beta * 0.2, safe = 0.9;
    static const double minscale = 0.333, maxscale = 6.0;
    double scale;
    if (err <= 1.0) {
        if (err == 0.0)
            scale = maxscale;
        else {
            scale = safe * pow(err, -alpha) * pow(errold, beta);
            scale = std::max(minscale, std::min(scale, maxscale));
        }
        if (reject)
            hnext = h * std::min(scale, 1.0);
        else
            hnext = h * scale;
        errold = std::max(err, 1.0e-4);
        reject = false;
        return true;
    } else {
        scale = std::max(safe * pow(err, -alpha), minscale);
        h *= scale;
        reject = true;
        return false;
    }

}
}
