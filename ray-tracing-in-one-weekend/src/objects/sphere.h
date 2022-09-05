#ifndef SPHERE_H
#define SPHERE_H

#include "../utils/global.h"
#include "../ray/hittable.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Eigen::Vector3f cen, float r) : center(cen), radius(r) {};
    Sphere(Eigen::Vector3f cen, float r, shared_ptr<Material> m)
        : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(
        const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

public:
    Eigen::Vector3f center;
    float radius;
    shared_ptr<Material> mat_ptr;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Eigen::Vector3f oc = r.start() - center;
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Eigen::Vector3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif