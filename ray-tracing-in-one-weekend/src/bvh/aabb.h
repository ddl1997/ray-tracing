#ifndef AABB_H
#define AABB_H

#include "../utils/global.h"
#include "../ray/ray.h"

class Aabb {
public:
    Aabb() {}
    Aabb(const vec3f& min, const vec3f& max) : minimum(min), maximum(max) {}

    vec3f min() const { return minimum; }
    vec3f max() const { return maximum; }

    bool hit(const Ray& r, float t_min, float t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.start()[a]) * invD;
            auto t1 = (max()[a] - r.start()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            // 将当前维度下进出时间与已知的进出时间进行比较
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            // 只要保证任意维度下的进入时间均小于任意维度下的离开时间即可
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    Aabb operator&(const Aabb box)
    {
        vec3f _min, _max;
        _min.x() = fminf(this->min().x(), box.min().x());
        _min.y() = fminf(this->min().y(), box.min().y());
        _min.z() = fminf(this->min().z(), box.min().z());

        _max.x() = fmaxf(this->min().x(), box.min().x());
        _max.y() = fmaxf(this->min().y(), box.min().y());
        _max.z() = fmaxf(this->min().z(), box.min().z());
        return Aabb(_min, _max);
    }

    vec3f minimum;
    vec3f maximum;
};

#endif
