#pragma once
#include <Eigen/Dense>
#include <cstdio>
#include <CarConstants.h>

namespace MathTools {

template <typename T, int R, int C>
using Matrix = Eigen::Matrix<T, R, C>;

template <typename T, int N>
using Vector = Eigen::Vector<T, N>;

// Set up common matrix and vector types
typedef Matrix<double, 2, 2> Mat2;
typedef Vector<double, 2> Vec2;
typedef Matrix<double, 3, 3> Mat3;
typedef Vector<double, 3> Vec3;
typedef Matrix<double, 4, 4> Mat4;
typedef Vector<double, 4> Vec4;
typedef Vector<double, CarConstants::NUM_STATES_BICYCLE> StateVector;
typedef Vector<double, CarConstants::NUM_STATES_BICYCLE> DerivativeVector;
typedef Vector<double, CarConstants::NUM_INPUT_STATES> InputVector;
typedef Vector<double, CarConstants::NUM_CONTROL_INPUTS> ControlInputVector;
}
