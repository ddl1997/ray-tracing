#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "../ray/ray.h"

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
        auto scatter_direction = rec.normal + random_unit_vector();
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    Eigen::Vector3f albedo;
};

#endif