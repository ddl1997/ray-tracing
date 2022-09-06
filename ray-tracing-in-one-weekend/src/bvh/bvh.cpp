#include "bvh.h"

#include <iostream>

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
    Aabb box_a;
    Aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    if (axis == 0)
        return box_a.min().x() < box_b.min().x();
    else if (axis == 1)
        return box_a.min().y() < box_b.min().y();
    else
        return box_a.min().z() < box_b.min().z();
}


bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
}

BvhNode::BvhNode(
    const std::vector<shared_ptr<Hittable>>& src_objects,
    size_t start, size_t end, float time0, float time1
) {
    std::vector<shared_ptr<Hittable>> objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
        : (axis == 1) ? box_y_compare
        : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        }
        else {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<BvhNode>(objects, start, mid, time0, time1);
        right = make_shared<BvhNode>(objects, mid, end, time0, time1);
    }

    Aabb box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left)
        || !right->bounding_box(time0, time1, box_right)
        )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = box_left & box_right;
}