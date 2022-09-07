#ifndef BVH_H
#define BVH_H

#include "../utils/global.h"

#include "../ray/hittable.h"
#include "../ray/hittable_list.h"


class BvhNode : public Hittable {
public:
    BvhNode();

    BvhNode(const HittableList& list, float time0, float time1)
        : BvhNode(list.objects, 0, list.objects.size(), time0, time1)
    {}

    BvhNode(
        const std::vector<shared_ptr<Hittable>>& src_objects,
        size_t start, size_t end, float time0, float time1);

    virtual bool hit(
        const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

    virtual bool bounding_box(float time0, float time1, Aabb& output_box) const override;

public:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    Aabb box;
};

#endif
