#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "../utils/global.h"
#include "../ray/hittable.h"

class MovingSphere : public Hittable {
public:
    MovingSphere() {}
    MovingSphere(
        vec3f cen0, vec3f cen1, float _time0, float _time1, float r, shared_ptr<Material> m)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
    {};

    virtual bool hit(
        const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

    vec3f center(float time) const;

public:
    vec3f center0, center1;
    float time0, time1;
    float radius;
    shared_ptr<Material> mat_ptr;
};

vec3f MovingSphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Eigen::Vector3f oc = r.start() - center(r.time());
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
    Eigen::Vector3f outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif