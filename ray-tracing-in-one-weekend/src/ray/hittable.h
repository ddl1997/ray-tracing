#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "../bvh/aabb.h"

class Material;

struct HitRecord {
    Eigen::Vector3f p;
    Eigen::Vector3f normal;
    std::shared_ptr<Material> mat_ptr;
    float t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Eigen::Vector3f& outward_normal) {
        // 入射光线与表面法线夹角为锐角即为front_face
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(float time0, float time1, Aabb& output_box) const = 0;
};

#endif