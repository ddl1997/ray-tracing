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
        scattered = Ray(rec.p, scatter_direction);
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
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return true;
    }

public:
    Eigen::Vector3f albedo;
    float fuzz;
};

#endif