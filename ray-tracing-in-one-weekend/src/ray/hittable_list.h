#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(
        const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual bool bounding_box(float time0, float time1, Aabb& output_box) const override;

public:
    std::vector<shared_ptr<Hittable>> objects;
};

#endif