#pragma once
#include <cmath>

namespace GlobalConstants {

const double GRAVITY = 9.81;
const double PI = M_PI;
const double TWO_PI = 2.0 * M_PI;
const double HALF_PI = 0.5 * M_PI;
const double DEG_TO_RAD = M_PI / 180.0;
const double RAD_TO_DEG = 180.0 / M_PI;
const double LBS_TO_KG = 0.45359237;
const double KG_TO_LBS = 1.0 / LBS_TO_KG;
const double INCHES_TO_METERS = 0.0254;
const double METERS_TO_INCHES = 1.0 / INCHES_TO_METERS;
const double SLUGS_FT_S2_TO_KG_M_S2 = 1.35582;
const double EPSILON = 1.0e-6;
const double MAX_STEERING_ANGLE = 1.;
const double MAX_STEERING_ANGLE_RAD = MAX_STEERING_ANGLE * DEG_TO_RAD;
static constexpr char MIMI[] = "I LOVE YOU";
static constexpr int MAX_STEPS = 50000;

}
