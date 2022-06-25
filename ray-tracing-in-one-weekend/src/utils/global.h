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
//**************************************************************************************************

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

inline float radians_to_degrees(float radians) {
    return radians * 180.0 / pi;
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Random Functions
//**************************************************************************************************

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

// Vector Functions
//**************************************************************************************************

inline Eigen::Vector3f gamma_correction(Eigen::Vector3f vec, float gamma)
{
    float power = 1.0 / gamma;
    return Eigen::Vector3f(powf(vec.x(), power), powf(vec.y(), power), powf(vec.z(), power));
}

inline bool near_zero(Eigen::Vector3f vec)
{
    const auto s = 1e-8;
    return (fabs(vec.x()) < s) && (fabs(vec.y()) < s) && (fabs(vec.z()) < s);
}

inline Eigen::Vector3f reflect(const Eigen::Vector3f& v, const Eigen::Vector3f& n)
{
    return v - 2 * v.dot(n) * n;
}

inline Eigen::Vector3f multi_respectively(const Eigen::Vector3f v1, const Eigen::Vector3f v2)
{
    return Eigen::Vector3f(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline Eigen::Vector3f refract(const Eigen::Vector3f& uv, const Eigen::Vector3f& n, double etai_over_etat) {
    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Eigen::Vector3f r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Eigen::Vector3f r_out_parallel = -sqrt(fabs(1.0 - powf(r_out_perp.norm(), 2))) * n;
    return r_out_perp + r_out_parallel;
}


#endif