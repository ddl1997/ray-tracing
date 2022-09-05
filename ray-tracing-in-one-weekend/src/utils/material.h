#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "../ray/ray.h"
//#include "../ray/hittable.h"

struct HitRecord;

class Material {
public:
    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Eigen::Vector3f& attenuation, Ray& scattered
    ) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Eigen::Vector3f& a) : albedo(a) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Eigen::Vector3f& attenuation, Ray& scattered
    ) const override {
        Eigen::Vector3f scatter_direction = rec.normal + random_vec3f().normalized();

        if (near_zero(scatter_direction))
            scatter_direction = rec.normal;
        scattered = Ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo;
        return true;
    }

public:
    Eigen::Vector3f albedo;
};

class Metal : public Material {
public:
    Metal(const Eigen::Vector3f& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Eigen::Vector3f& attenuation, Ray& scattered
    ) const override {
        Eigen::Vector3f reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return true;
    }

public:
    Eigen::Vector3f albedo;
    float fuzz; // 模糊系数
};

// 绝缘体
class Dielectric : public Material {
public:
    Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Eigen::Vector3f& attenuation, Ray& scattered
    ) const override {
        attenuation = Eigen::Vector3f(1.0, 1.0, 1.0);
        // 若为front_face，则光线从（一般为）空气进入该材质物体，折射率之比为空气折射率/材质折射率，即为ir的倒数
        // 若不为front_face，则光线从该材质物体进入（一般为）空气，折射率之比ir
        float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Eigen::Vector3f unit_direction = r_in.direction().normalized();
        float cos_theta = fmin(-unit_direction.dot(rec.normal), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        // theta：入射光与向内法线的夹角；theta_prime：折射光与向内法线的夹角
        // sin(theta_prime) = eta * sin(theta) / eta_prime
        // sin(theta_prime) > 0 => total internal reflection 全内反射，折射率大->折射率小
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Eigen::Vector3f direction;

        if (cannot_refract || 
            reflectance(cos_theta, refraction_ratio) > random_float()) // 部分反射，部分折射
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.p, direction, r_in.time());
        Eigen::Vector3f refracted = refract(unit_direction, rec.normal, refraction_ratio);
        return true;
    }

public:
    float ir; // Index of Refraction 材质折射率/光线传输介质折射率

private:
    static float reflectance(float cosine, float ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif