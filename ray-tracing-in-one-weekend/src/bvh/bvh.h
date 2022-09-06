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

bool BvhNode::bounding_box(float time0, float time1, Aabb& output_box) const {
    output_box = box;
    return true;
}

bool BvhNode::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);
    // rec.t 指明了在左子树中最早命中的Hittable的命中时间；对于右子树中的Hittable，如果想更新rec，
    // 只能在tmin到rec.t这一时间段中命中

    return hit_left || hit_right;
}

#endif
