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
    virtual bool bounding_box(float time0, float time1, Aabb& output_box) const override;

public:
    Eigen::Vector3f center;
    float radius;
    shared_ptr<Material> mat_ptr;

private:
    static void get_sphere_uv(const vec3f& p, float& u, float& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + PI;

        u = phi / (2 * PI);
        v = theta / PI;
    }
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Eigen::Vector3f oc = r.start() - center;
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c; // 判别式

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

bool Sphere::bounding_box(float time0, float time1, Aabb& output_box) const
{
    output_box = Aabb(
        center - vec3f(radius, radius, radius),
        center + vec3f(radius, radius, radius));
    return true;
}

#endif