#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Common Headers
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
    return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max)
{
    return min + (max - min) * random_float();
}

inline Eigen::Vector3f random_vec3f()
{
    return Eigen::Vector3f(random_float(), random_float(), random_float());
}

inline Eigen::Vector3f random_vec3f(float min, float max)
{
    return Eigen::Vector3f(random_float(min, max), random_float(min, max), random_float(min, max));
}

inline Eigen::Vector3f random_in_unit_sphere()
{
    while (true) {
        auto p = random_vec3f(-1, 1);
        if (p.norm() >= 1) continue;
        return p;
    }
}

// TODO: 半球面cos分布（证明）
inline Eigen::Vector3f random_in_hemisphere(const Eigen::Vector3f& normal)
{
    Eigen::Vector3f in_unit_sphere = random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline Eigen::Vector3f gamma_correction(Eigen::Vector3f vec, float gamma)
{
    float power = 1.0 / gamma;
    return Eigen::Vector3f(powf(vec.x(), power), powf(vec.y(), power), powf(vec.z(), power));
}

#endif