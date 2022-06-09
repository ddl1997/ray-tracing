#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <Eigen/Dense>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float flt_inf = std::numeric_limits<double>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

inline float radians_to_degrees(float radians) {
    return radians * 180.0 / pi;
}

inline float random_float()
{
    return rand() / (RAND_MAX + 1);
}

inline float random_float(float min, float max)
{
    return min + (max - min) * random_float();
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers
#include <Eigen/Dense>

#endif